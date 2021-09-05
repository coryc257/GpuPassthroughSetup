#include "exec_container.h"
#include <QTextCodec>
#include <QStandardPaths>
#include <QDir>
#include <QThread>
#include <QFile>
#include <QMap>
#include "daemonize.h"
#include "../util/MsgBox.h"
#include "../util/SAFE_RETURN.h"
//QStandardPaths::standardLocations(QStandardPaths::StandardLocation::HomeLocation)[0]
#define CONFIG_FOLDER QStringLiteral("/root/.gpu_passthrough")

// Template macros
template <class T>
static QString join_str(QList<T> &list, QString separator, const std::function<QString (const T &item)> toStr)
{
    QString output;
    for (int j = 0; j < list.size(); j++)
    {
        output.append((j ? separator : QStringLiteral("")) + toStr(list[j]));
    }
    return output;
}

template <class CT>
static bool contains_value(QList<CT> &search, const CT &forV, std::function<bool (const CT &v1, const CT &v2)> cmpOp)
{
    int size = search.count();
    for (int j = 0; j < search.count(); j ++)
        if (cmpOp(search[j], forV))
            return true;
    return false;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//ExecContainer
//{
    ExecContainer::ExecContainer(QString program, QStringList args)
    {
        this->program = QString(program);
        this->args = QStringList(args);
    }

    bool ExecContainer::Run()
    {
        QProcess command;
        command.setProcessChannelMode(QProcess::MergedChannels);
        command.start(this->program, this->args, QProcess::ReadWrite);

        command.waitForFinished();
        this->output = QTextCodec::codecForMib(106)->toUnicode(command.readAll());

        return  (
                    QProcess::ExitStatus::NormalExit == command.exitStatus()
                    && command.exitCode() == 0
                ) ? true : false;
    }

    QString ExecContainer::GetOutputString()
    {
        return this->output;
    }
//}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//Operations
//{
    QString Operations::vmName;
    QString Operations::userName;

    QString Operations::vmConfig;
    QString Operations::normalConfig;
    QStringList Operations::deviceList;
    QStringList Operations::detachedDeviceList;
    QString Operations::evDevKeyboard;
    QString Operations::passthroughMouse;
    QString Operations::IOMMUGroup;
    QString Operations::Ram;
    QString Operations::Cpu;
    bool Operations::revertOnVmExit = false;
    CpuTune Operations::__tuneInfo;
    int Operations::pointOfFailure = 0;
    bool Operations::debugOnly = false;
    bool Operations::reloadNvidia = false;



    static void __CpuGetInfo__AddItem(QString Item, QList<CPU_CORE> *list)
    {
        QStringList columns = Item.replace(QStringLiteral(" "), QStringLiteral("\t")).remove(QStringLiteral("\n")).split(QStringLiteral("\t"), Qt::SplitBehaviorFlags::SkipEmptyParts);
        if (columns[0].compare(QStringLiteral("CPU")) != 0) {
            CPU_CORE newCore;

            if (columns.count() != 6)
                return;
            QStringList cacheItems = columns[4].split(QStringLiteral(":"));
            if (cacheItems.count() != 4)
                return;

            newCore.CPU = columns[0];
            newCore.NODE = columns[1];
            newCore.SOCKET = columns[2];
            newCore.CORE = columns[3];
            newCore.L1d = cacheItems[0];
            newCore.L1i = cacheItems[1];
            newCore.L2 = cacheItems[2];
            newCore.L3 = cacheItems[3];
            newCore.ONLINE = columns[5];
            list->append(newCore);
        }
    }

    static QList<CPU_CORE> __GetCpuInfo()
    {
        QList<CPU_CORE> retVal;
        BashCommandResult res = Operations::BashCommand(QStringLiteral("lscpu -e"));

        if (res.Success) {
            Q_FOREACH(QString Item, res.Output.split(QStringLiteral("\n"))) {
                if (Item.trimmed().length() == 0)
                    continue;
                __CpuGetInfo__AddItem(Item, &retVal);
            }
        }

        return retVal;
    }

    static QString __generation_cpu_tune(QList<CPU_CORE> assignedCores, QList<int> emulatorPin, int ram)
    {
        QString retVal;
        int currentCore = 0;

        retVal.reserve(4096);
        retVal.append(QStringLiteral("  <memory unit='KiB'>")+QString::number(ram)+QStringLiteral("</memory>"));
        retVal.append(QStringLiteral("  <currentMemory unit='KiB'>")+QString::number(ram)+QStringLiteral("</currentMemory>"));
        retVal.append(QStringLiteral("  <vcpu placement='static'>")+QString::number(assignedCores.count())+QStringLiteral("</vcpu>\n"));
        retVal.append(QStringLiteral("  <cpu mode='host-passthrough' check='none' migratable='on'>\n"));
        retVal.append(QStringLiteral("    <topology sockets='1' dies='1' cores='")+QString::number(assignedCores.count()/2)+QStringLiteral("' threads='2'/>\n"));
        retVal.append(QStringLiteral("  </cpu>\n"));
        retVal.append(QStringLiteral("  <cputune>\n"));
        Q_FOREACH(CPU_CORE core, assignedCores) {
            retVal.append(QStringLiteral("    <vcpupin vcpu='")+QString::number(currentCore)+QStringLiteral("' cpuset='")+core.CPU+QStringLiteral("'/>\n"));
            currentCore++;
        }
        retVal.append(QStringLiteral("    <emulatorpin cpuset='"));
        for (int j = 0; j < emulatorPin.count(); j++) {
            retVal.append(QString::number(emulatorPin[j]));
            if (j + 1 != emulatorPin.count()) {
                retVal.append(QStringLiteral(","));
            }
        }
        retVal.append(QStringLiteral("'/>\n"));
        retVal.append(QStringLiteral("  </cputune>\n"));



        return retVal;
    }



    // TODO: Optimize
    static void __init_cpu_tune(CpuTune *ti, QString ramGB, QString cpuCores, QString vmName)
    {
        ti->Success = false;

        QList<CPU_CORE> cpuInfo = __GetCpuInfo();
        auto cpuCoreCmpFunc = [](const CPU_CORE &v1, const CPU_CORE &v2)->bool
                        {
                            //qInfo("CPU CHECK{%s, %s}\n", qPrintable(v1.CPU), qPrintable(v2.CPU));
                            return v1.CPU == v2.CPU;
                        };

        ti->ram = ramGB.split(QStringLiteral(" "))[0].toInt();
        ti->cores = cpuCores.split(QStringLiteral(" "))[0].toInt();
        ti->ramKiB = 1024 * 1024 * ti->ram;

        Q_FOREACH(CPU_CORE core, cpuInfo) {
            int l1 = core.L1d.toInt();
            int l3 = core.L3.toInt();

            if (!ti->l3Groups.contains(l3)) {
                ti->l3Groups[l3] = 1;
            } else {
                ti->l3Groups[l3] += 1;
            }

            if (!ti->l1Groups.contains(l1)) {
                ti->l1Groups[l1] = QList<CPU_CORE>();
            }
            ti->l1Groups[l1].append(core);
        }

        if (ti->l3Groups.count() > 1) { // FORCE A SPLIT on the L3 Cache
            if (ti->cores != ti->l3Groups[1]) {
                QString sv = QString::number(ti->l3Groups[1]);
                ti->ErrorMessage = QStringLiteral("You must assign a number of cores equal to an L3 cache boundary(") + sv + QStringLiteral(")");
                goto error_out;
            } else {
                Q_FOREACH(CPU_CORE core, cpuInfo) {
                    int l3 = core.L3.toInt();
                    if (l3 == 0) {
                        ti->emulatorPin.append(core.CPU.toInt());
                        ti->freeCores.append(core);
                    } else if (l3 == 1) {
                        ti->assignedCores.append(core);
                    }
                }
            }
        } else { // DO WHAT TOLD
            // TODO
            if (ti->cores % 2 != 0) {
                ti->ErrorMessage = QStringLiteral("You must ask for an even number of cores");
                goto error_out;
            }
            int groupCount = ti->l1Groups.count();

            // Start with odds
            for (int j = 1; j < groupCount; j += 2) {
                if (!(ti->assignedCores.count() == ti->cores)) {
                    Q_FOREACH(CPU_CORE core, ti->l1Groups[ti->l1Groups.keys()[j]]) {
                        ti->assignedCores.append(core);
                    }
                }
            }

            // Work backwards
            // Make sure we have at least two cores to pin to the emulator
            for (int j = (groupCount % 2 == 0) ? groupCount - 2 : groupCount - 1; j > 0; j -= 2) {
                if (!(ti->assignedCores.count() == ti->cores)) {
                    Q_FOREACH(CPU_CORE core, ti->l1Groups[ti->l1Groups.keys()[j]]) {
                        ti->assignedCores.append(core);
                    }
                }
            }

            if (ti->assignedCores.count() != ti->cores) {
                ti->ErrorMessage = QStringLiteral("You don't have enough cores");
            }
        }

        Q_FOREACH(CPU_CORE core, ti->l1Groups[0]) {
            ti->emulatorPin.append(core.CPU.toInt());
        }

        Q_FOREACH(CPU_CORE core, cpuInfo) {
            //qInfo("Core:%s\n", qPrintable(core.CPU));
            if (!contains_value<CPU_CORE>(
                    ti->assignedCores,
                    core,
                    cpuCoreCmpFunc
            ))
                ti->freeCores.append(core);
        }

        ti->vmConfig = __generation_cpu_tune(ti->assignedCores, ti->emulatorPin, ti->ramKiB);
        ti->Success = true;
        return;

        error_out:
        ti->Success = false;
        return;
    }

    PciPassthroughInfo __get_pci_info(QString iommuGroup, QString groupInfo)
    {
        PciPassthroughInfo info;
        QStringList lines = groupInfo.split(QStringLiteral("\n"));
        bool inside = false;
        int busStart = 10;

        info.vmConfig.reserve(4096);

        // Find the line
        for (int j = 0; j < lines.count(); j++) {
            if (inside && lines[j].startsWith(QStringLiteral("IOMMU"))) {
                break;
            }

            if ( lines[j].contains(QStringLiteral("PCI bridge")))
                continue;

            if (inside) {
                QStringList identity = lines[j].trimmed().split(QStringLiteral(" "))[0].replace(QStringLiteral("."),QStringLiteral(":")).split(QStringLiteral(":"));

                info.vmConfig.append(QStringLiteral("<hostdev mode='subsystem' type='pci' managed='yes'>\n"));
                info.vmConfig.append(QStringLiteral("  <source>\n"));
                info.vmConfig.append(QStringLiteral("    <address domain='0x0000' bus='0x$1' slot='0x$2' function='0x$3'/>\n")
                                .replace(QStringLiteral("$1"),identity[0])
                                .replace(QStringLiteral("$2"),identity[1])
                                .replace(QStringLiteral("$3"),identity[2])
                            );
                info.vmConfig.append(QStringLiteral("  </source>\n"));
                info.vmConfig.append(QStringLiteral("  <address type='pci' domain='0x0000' bus='0x$1' slot='0x00' function='0x0'/>\n")
                                        .replace(QStringLiteral("$1"),QString::number(busStart))
                );
                info.vmConfig.append(QStringLiteral("</hostdev>\n"));

                busStart++;

                info.devices.append(QStringLiteral("pci_0000_")+identity[0]+QStringLiteral("_")+identity[1]+QStringLiteral("_")+identity[2]);
            } else if (lines[j].startsWith(iommuGroup)) {
                inside = true;
            }
        }


        return info;
    }


    static void __write_file(SAFE_RETURN *sr, QString contents, QString file)
    {
        QFile outputFile(file);
        QString backupcopy = file+QStringLiteral(".swp");

        if (QFile::exists(backupcopy))
            QFile::remove(backupcopy);


        if (QFile::exists(file) && !QFile::copy(file, backupcopy))
        {
            sr->StatusCode = SAFE_RETURN_CODES::FILE_COPY_ERROR;
            sr->Message = QStringLiteral("Cannot create backup file");
            goto __write_file_out;
        }

        if (QFile::exists(file) && !QFile::remove(file))
        {
            sr->StatusCode = SAFE_RETURN_CODES::FILE_DELETE_ERROR;
            sr->Message = QStringLiteral("Cannot delete old file");
            goto __write_file_copy_back;
        }

        if (!outputFile.open(QIODevice::ReadWrite | QIODevice::OpenModeFlag::NewOnly))
        {
            sr->StatusCode = SAFE_RETURN_CODES::FILE_DELETE_ERROR;
            sr->Message = QStringLiteral("Cannot write create file");
            goto __write_file_copy_back;
        }

        if (outputFile.write(contents.toUtf8()) && outputFile.flush())
            goto __write_file_out;
        else {
            sr->StatusCode = SAFE_RETURN_CODES::FILE_WRITE_ERROR;
            sr->Message = QStringLiteral("Cannot write file");
            goto __write_file_copy_back;
        }

        __write_file_copy_back:
            if (QFile::copy(file+QStringLiteral(".swp"), file))
                QFile::remove(file+QStringLiteral(".swp"));
        __write_file_out:
            if (sr->StatusCode == SAFE_RETURN_CODES::OK)
                QFile::remove(file+QStringLiteral(".swp"));
    }

    static void __vm_edit(QString Message, QString vmName)
    {
        QProcess virshTerm;
        virshTerm.startDetached("xterm", QStringList({QStringLiteral("-e"), QStringLiteral("virsh"), QStringLiteral("edit"), vmName}));
        MsgBox(Message);
    }



    void Operations::Init()
    {
        if (!QDir(CONFIG_FOLDER).exists()) {
            QDir().mkdir(CONFIG_FOLDER);
        }
        Operations::__Init();
    }

    void Operations::SaveNonVmXConfig()
    {
        ExecContainer copy_command = ExecContainer(
                QStringLiteral("cp"),
                QStringList({
                    QStringLiteral("/etc/X11/xorg.conf"),
                    CONFIG_FOLDER + QStringLiteral("/non_vm_config.conf")
                }));
        copy_command.Run();
    }

    void Operations::SaveVmXConfig()
    {
        ExecContainer copy_command = ExecContainer(
                QStringLiteral("cp"),
                QStringList({
                    QStringLiteral("/etc/X11/xorg.conf"),
                    CONFIG_FOLDER + QStringLiteral("/vm_config.conf")
                }));
        copy_command.Run();
    }

    void Operations::SaveVmXConfig(QString xConfig)
    {
        BashCommand(QStringLiteral("printf \"%s\" \"")+xConfig.replace(QStringLiteral("\""),QStringLiteral("\\\""))+QStringLiteral("\" > /root/.gpu_passthrough/vm_config.conf"));
        //BashCommand(QStringLiteral("printf \"%s\" \"")+xConfig.replace(QStringLiteral("\""),QStringLiteral("\\\""))+QStringLiteral("\" > ~/vm_config.conf"));
    }

    void Operations::SaveNonVmXConfig(QString xConfig)
    {
        BashCommand(QStringLiteral("printf \"%s\" \"")+xConfig.replace(QStringLiteral("\""),QStringLiteral("\\\""))+QStringLiteral("\" > /root/.gpu_passthrough/non_vm_config.conf"));
        //BashCommand(QStringLiteral("printf \"%s\" \"")+xConfig.replace(QStringLiteral("\""),QStringLiteral("\\\""))+QStringLiteral("\" > ~/vm_config.conf"));
    }

    void Operations::SetQEmuCommandLine(QString vmName, QString device)
    {


        QString qEmuCommandLineXml = QStringLiteral("\n\n\n<qemu:commandline>\n\t<qemu:arg value='-object'/>\n\t<qemu:arg value='input-linux,id=kbd1,evdev=/dev/input/event")+
                                    device+QStringLiteral(",grab_all=on,repeat=on'/>\n</qemu:commandline>");

        BashCommand(QStringLiteral("printf \"%s\" \"")+device+QStringLiteral("\" > ") + CONFIG_FOLDER + QStringLiteral("/keyboard.param"));
        QProcess virshTerm;
        virshTerm.startDetached("xterm", QStringList({QStringLiteral("-e"), QStringLiteral("virsh"), QStringLiteral("edit"), vmName}));
        MsgBox(QStringLiteral("Place after </devices> before </domain>... Middle click to paste\n\n")+qEmuCommandLineXml);
    }

    void Operations::SavePassthroughMouse(QString mouseIdentity)
    {
        QStringList mParts = mouseIdentity.trimmed().split(QStringLiteral(":"));
        if (mParts.count() == 2) {
            QString mouseToggleXML =    QStringLiteral("<hostdev mode='subsystem' type='usb' managed='no'>\n\t<source>\n\t\t<vendor id='0x") +
                                        mParts[0].replace(QStringLiteral("'"), QStringLiteral("\\'")) +
                                        QStringLiteral("' />\n\t\t<product id='0x") +
                                        mParts[1].replace(QStringLiteral("'"), QStringLiteral("\\'")) +
                                        QStringLiteral("' />\n\t</source>\n</hostdev>");
            BashCommand(
                    QStringLiteral("printf \"%s\" \"") + mouseToggleXML.replace(QStringLiteral("\""), QStringLiteral("\\\"")) + QStringLiteral("\" > /root/.gpu_passthrough/toggle_mouse.xml")
            );
            BashCommand(
                    QStringLiteral("printf \"%s\" \"") + mouseIdentity.replace(QStringLiteral("\""), QStringLiteral("\\\"")) + QStringLiteral("\" > /root/.gpu_passthrough/mouse.param")
            );
        }
    }

    void Operations::SaveIOMMU(SAFE_RETURN *retVal, QString iommuGroup, QString vmName)
    {
        QString command;
        BashCommandResult res = Operations::BashCommand(QStringLiteral("./find_groups.sh"));

        __write_file(retVal, iommuGroup, CONFIG_FOLDER + QStringLiteral("/iommu.param"));
        if (IS_OK((*retVal))) {
            PciPassthroughInfo info = __get_pci_info(iommuGroup, res.Output);
            __vm_edit(QStringLiteral("Put this in your config, make sure you don't have duplicate domain,bus,slot,function entries\n\n")+info.vmConfig, vmName);
        } else {
            retVal->returnData[QStringLiteral("Operations::SaveIOMMU~pointOfFailure")] = RETURN_TYPE::fromString("__write_file");
        }
    }


    void Operations::SetVmXConfig(SAFE_RETURN *retVal, QString xConfigFile)
    {
        ExecContainer copy_command = ExecContainer(
                QStringLiteral("cp"),
                QStringList({
                    xConfigFile,
                    CONFIG_FOLDER + QStringLiteral("/vm_config.conf")
                }));
        if (!copy_command.Run()) {
            retVal->StatusCode = SAFE_RETURN_CODES::COMMAND_ERROR;
            retVal->returnData[QStringLiteral("SetVmXConfig::CommandError")] =
                RETURN_TYPE::fromQString(copy_command.GetOutputString());
        }
    }

    void Operations::SetNonVmXConfig(SAFE_RETURN *retVal, QString xConfigFile)
    {
        ExecContainer copy_command = ExecContainer(
                QStringLiteral("cp"),
                QStringList({
                    xConfigFile,
                    CONFIG_FOLDER + QStringLiteral("/non_vm_config.conf")
                }));
        if (!copy_command.Run()) {
            retVal->StatusCode = SAFE_RETURN_CODES::COMMAND_ERROR;
            retVal->returnData[QStringLiteral("SetNonVmXConfig::CommandError")] =
                RETURN_TYPE::fromQString(copy_command.GetOutputString());
        }
    }


    void Operations::SetVMName(SAFE_RETURN *retVal, QString vmName)
    {
        __write_file(retVal, vmName, CONFIG_FOLDER + QStringLiteral("/vm_name.param"));
        if (retVal->StatusCode != SAFE_RETURN_CODES::OK) {
            retVal->returnData["FileName"] = RETURN_TYPE::fromQString(CONFIG_FOLDER + QStringLiteral("/vm_name.param"));
        }
    }

    void Operations::SetUser(SAFE_RETURN *retVal, QString userName)
    {
        __write_file(retVal, userName, CONFIG_FOLDER + QStringLiteral("/user_name.param\""));
        if (retVal->StatusCode != SAFE_RETURN_CODES::OK) {
            retVal->returnData["FileName"] = RETURN_TYPE::fromQString(CONFIG_FOLDER + QStringLiteral("/vm_name.param"));
        }
    }

    void Operations::WipeKScreen(QString userName)
    {
        ExecContainer tp = ExecContainer(
                QStringLiteral("/bin/find"),
                QStringList({
                    QStringLiteral("/home/")+userName+QStringLiteral("/.local/share/kscreen"),
                    QStringLiteral("-type"),
                    QStringLiteral("f"),
                    QStringLiteral("-exec"),
                    QStringLiteral("rm"),
                    QStringLiteral("{}"),
                    QStringLiteral("+")
                }));
        tp.Run();
    }

    BashCommandResult Operations::BashCommandNoLog(QString command)
    {
        BashCommandResult result;
        ExecContainer xcommand = ExecContainer(QStringLiteral("/bin/bash"), QStringList({
                QStringLiteral("-c"),
                command}));
        if(xcommand.Run()) {
            result.Success = true;
            result.Output = xcommand.GetOutputString();
        } else {
            result.Success = false;
            result.Output = xcommand.GetOutputString();
        }
        return result;
    }

    BashCommandResult Operations::BashCommand(QString command)
    {
        BashCommandResult result;
        AppendLog(QStringLiteral("COMMAND:")+command);
        if (Operations::debugOnly) {
            result.Success = true;
            result.Output = command;
            return result;
        }

        ExecContainer xcommand = ExecContainer(QStringLiteral("/bin/bash"), QStringList({
                QStringLiteral("-c"),
                command}));
        if(xcommand.Run()) {
            result.Success = true;
            result.Output = xcommand.GetOutputString();
            Operations::AppendLog(QStringLiteral("SUCCESS:\n") + xcommand.GetOutputString());
        } else {
            result.Success = false;
            result.Output = xcommand.GetOutputString();
            Operations::AppendLog(QStringLiteral("SUCCESS BUT NOT:\n") + xcommand.GetOutputString());
            /*if (command != QStringLiteral("reboot")) {
                Operations::BashCommand(QStringLiteral("reboot"));
            }*/
        }
        return result;
    }

    void Operations::ClearLog()
    {
        Operations::BashCommandNoLog(QStringLiteral("printf \"\" > ") + CONFIG_FOLDER + QStringLiteral("/.log"));
    }


    void Operations::AppendLog(QString logText)
    {
        Operations::BashCommandNoLog(
            QStringLiteral("printf \"%s\\n\" \"" ) +
            (( logText.replace(QStringLiteral("\""),QStringLiteral("\\\"")) ))
            + QStringLiteral("\" >> ") + CONFIG_FOLDER + QStringLiteral("/.log"));
    }

    QString Operations::CatThat(QString fileName)
    {
        ExecContainer command = ExecContainer(QStringLiteral("/bin/cat"), QStringList{fileName});
        if(command.Run()) {
            return command.GetOutputString();
        } else {
            return QStringLiteral("");
        }
    }

    void Operations::StartX()
    {
        ExecContainer command = ExecContainer(QStringLiteral("/bin/systemctl"), QStringList({QStringLiteral("start"), QStringLiteral("display-manager.service")}));
        if(command.Run()) {
            qInfo("%s\n", qPrintable(command.GetOutputString()));
        } else {
            Operations::Reboot();
        }
    }

    void Operations::StopX()
    {
        auto res = Operations::BashCommand(QStringLiteral("systemctl stop display-manager.service"));
        if (!res.Success) {
            Operations::pointOfFailure = 1;
            return;
        }
        QThread::sleep(10);

        Operations::BashCommand(QStringLiteral("killall -9 kwin_x11"));
        QThread::sleep(2);
    }


    void Operations::Reboot()
    {
        //Operations::BashCommand(QStringLiteral("reboot"));
    }

    void Operations::CopyConfig()
    {
        auto res = Operations::BashCommand(
            QStringLiteral("cp \"")+ Operations::vmConfig +QStringLiteral("\" /etc/X11/xorg.conf")
        );
        if (!res.Success)

        Operations::WipeKScreen(Operations::userName);
    }

    void Operations::RevertConfig()
    {
        Operations::BashCommand(
            QStringLiteral("cp \"")+ (Operations::normalConfig) +QStringLiteral("\" /etc/X11/xorg.conf")
        );
        Operations::WipeKScreen(Operations::userName);
    }

    void Operations::SaveRamCpu(QString ramGB, QString cpuCores, QString vmName)
    {
        CpuTune tuneInfo;

        __init_cpu_tune(&tuneInfo, ramGB, cpuCores, vmName);


        if (tuneInfo.Success) {
            Operations::BashCommandNoLog(QStringLiteral("printf \"%s\" \"$1\" > $2")
                .replace(QStringLiteral("$1"), QString::number(tuneInfo.ram))
                .replace(QStringLiteral("$2"), CONFIG_FOLDER + QStringLiteral("/ram.param"))
            );

            Operations::BashCommandNoLog(QStringLiteral("printf \"%s\" \"$1\" > $2")
                .replace(QStringLiteral("$1"), QString::number(tuneInfo.cores))
                .replace(QStringLiteral("$2"), CONFIG_FOLDER + QStringLiteral("/cpu.param"))
            );

            QProcess virshTerm;
            virshTerm.startDetached("xterm", QStringList({QStringLiteral("-e"), QStringLiteral("virsh"), QStringLiteral("edit"), vmName}));

            MsgBox(QStringLiteral("Replace the memory, currentMemory, cputune, cpu and vcpu sections of your config with:\n\n")+tuneInfo.vmConfig);

        } else {
            MsgBox(QStringLiteral("FAILED:\n")+tuneInfo.ErrorMessage);
        }

    }



    void Operations::UnbindGPU()
    {
        QThread::sleep(5);

        Operations::reloadNvidia = false;
        BashCommandResult res;
        if (!(res = Operations::BashCommand(QStringLiteral("rmmod nvidia_uvm"))).Success) {
            if (!res.Output.contains(QStringLiteral("is not currently loaded"))) {
                Operations::pointOfFailure = 2;
                return;
            } else {
                Operations::reloadNvidia = true;
            }
        }

        if (!(res = Operations::BashCommand(QStringLiteral("rmmod nvidia_drm"))).Success) {
            if (!res.Output.contains(QStringLiteral("is not currently loaded"))) {
                Operations::pointOfFailure = 3;
                return;
            } else {
                Operations::reloadNvidia = true;
            }
        }

        if (!(res = Operations::BashCommand(QStringLiteral("rmmod nvidia_modeset"))).Success) {
            if (!res.Output.contains(QStringLiteral("is not currently loaded"))) {
                Operations::pointOfFailure = 4;
                return;
            } else {
                Operations::reloadNvidia = true;
            }
        }

        if (!(res = Operations::BashCommand(QStringLiteral("rmmod nvidia_uvm"))).Success) {
            if (!res.Output.contains(QStringLiteral("is not currently loaded"))) {
                Operations::pointOfFailure = 5;
                return;
            } else {
                Operations::reloadNvidia = true;
            }
        }

        if (!(res = Operations::BashCommand(QStringLiteral("rmmod nvidia"))).Success) {
            if (!res.Output.contains(QStringLiteral("is not currently loaded"))) {
                Operations::pointOfFailure = 6;
                return;
            }
        }

        if (!(res = Operations::BashCommand(QStringLiteral("rmmod vfio-pci"))).Success) {
            /*if (!res.Output.contains(QStringLiteral("is not currently loaded"))) {
                Operations::pointOfFailure = 7;
                return;
            }*/
        }

        for (int j = 0; j < Operations::deviceList.count(); j++) {
            if (!Operations::BashCommand(QStringLiteral("virsh nodedev-detach ")+Operations::deviceList[j]).Success) {
                Operations::pointOfFailure = 8;
            } else {
                Operations::detachedDeviceList.append(Operations::deviceList[j]);
            }
        }

        //__bash_command(QStringLiteral("virsh nodedev-detach pci_0000_01_00_0"));
        //__bash_command(QStringLiteral("virsh nodedev-detach pci_0000_01_00_1"));
    }

    void Operations::BindGPU()
    {
        Operations::BashCommand(QStringLiteral("modprobe vfio-pci"));
        Operations::BashCommand(QStringLiteral("modprobe nvidia"));
    }

    void Operations::RevertGPU()
    {
        for (int j = 0; j < Operations::detachedDeviceList.count(); j++) {
            if (!Operations::BashCommand(QStringLiteral("virsh nodedev-detach ")+Operations::deviceList[j]).Success) {
                Operations::pointOfFailure = 8;
            }
        }
        for (int j = 0; j < Operations::detachedDeviceList.count(); j++) {
            if (!Operations::BashCommand(QStringLiteral("virsh nodedev-reattach ")+Operations::deviceList[j]).Success) {
                Operations::pointOfFailure = 8;
            }
        }
        QThread::sleep(10);
        Operations::AppendLog(QStringLiteral("Reloading Drivers"));
        Operations::BashCommand(QStringLiteral("modprobe nvidia"));
        Operations::BashCommand(QStringLiteral("modprobe nvidia_modeset"));
        Operations::BashCommand(QStringLiteral("modprobe nvidia_uvm"));
        Operations::BashCommand(QStringLiteral("modprobe nvidia_drm"));
        QThread::sleep(10);
        Operations::AppendLog(QStringLiteral("Binding"));
        Operations::BashCommand(QStringLiteral("echo 1 > /sys/class/vtconsole/vtcon0/bind"));
        //Operations::BashCommand(QStringLiteral("echo 0 > /sys/class/vtconsole/vtcon1/bind"));
        Operations::AppendLog(QStringLiteral("x-config"));
        Operations::BashCommand(QStringLiteral("nvidia-xconfig --query-gpu-info > /dev/null 2>&1"));
        Operations::AppendLog(QStringLiteral("Framebuffer"));
        Operations::BashCommand(
            QStringLiteral("echo efi-framebuffer.0 > /sys/bus/platform/drivers/efi-framebuffer/bind")
        );
        QThread::sleep(10);
    }

    void Operations::StartVM()
    {
        //TODO: MAKE THIS REAL


        Operations::BashCommand(QStringLiteral("echo 0 > /sys/class/vtconsole/vtcon0/bind"));
        Operations::BashCommand(QStringLiteral("echo 0 > /sys/class/vtconsole/vtcon1/bind"));
        Operations::BashCommand(
            QStringLiteral("echo efi-framebuffer.0 > /sys/bus/platform/drivers/efi-framebuffer/unbind")
        );
        QThread::sleep(10);


        Operations::BashCommand(QStringLiteral("chown libvirt-qemu ")+Operations::evDevKeyboard);


        Operations::BashCommand(QStringLiteral("virsh start ")+Operations::vmName);
    }

    void Operations::__Init()
    {
        Operations::userName =   Operations::CatThat(
                                        CONFIG_FOLDER + QStringLiteral("/user_name.param")
                                    );
        Operations::vmName =     Operations::CatThat(
                                        CONFIG_FOLDER + QStringLiteral("/vm_name.param")
                                    );

        Operations::evDevKeyboard = Operations::CatThat(
                                        CONFIG_FOLDER + QStringLiteral("/keyboard.param")
                                    );
        Operations::passthroughMouse = Operations::CatThat(
                                        CONFIG_FOLDER + QStringLiteral("/mouse.param")
                                    );

        Operations::IOMMUGroup = Operations::CatThat(
                                        CONFIG_FOLDER + QStringLiteral("/iommu.param")
                                    );

        Operations::Ram = Operations::CatThat(
                                        CONFIG_FOLDER + QStringLiteral("/ram.param")
                                    );

        Operations::Cpu = Operations::CatThat(
                                        CONFIG_FOLDER + QStringLiteral("/cpu.param")
                                    );


        if (QFile::exists(CONFIG_FOLDER + QStringLiteral("/vm_config.conf"))) {
            Operations::vmConfig = CONFIG_FOLDER + QStringLiteral("/vm_config.conf");
        }

        if (QFile::exists(CONFIG_FOLDER + QStringLiteral("/non_vm_config.conf"))) {
            Operations::normalConfig = CONFIG_FOLDER + QStringLiteral("/non_vm_config.conf");
        }


        __init_cpu_tune(&Operations::__tuneInfo, Operations::Ram, Operations::Cpu, Operations::vmName);
        BashCommandResult res = Operations::BashCommand(QStringLiteral("./find_groups.sh"));
        if (res.Success) {
            PciPassthroughInfo info = __get_pci_info(Operations::IOMMUGroup, res.Output);
            Q_FOREACH(QString device, info.devices) {
                Operations::deviceList.append(device);
            }
        }
    }

    void Operations::DEBUG()
    {
        Operations::AppendLog(QStringLiteral("userName: ") + Operations::userName);
        Operations::AppendLog(QStringLiteral("vmName: ") + Operations::vmName);
        Operations::AppendLog(QStringLiteral("vmConfig: ") + Operations::vmConfig);
        Operations::AppendLog(QStringLiteral("normalConfig: ") + Operations::normalConfig);
        Q_FOREACH(QString device, Operations::deviceList) {
            Operations::AppendLog(QStringLiteral("device: ") + device);
        }
    }



    void Operations::PartitionCpu()
    {
        CpuTune tuneInfo;
        QString hostCpus;
        __init_cpu_tune(&tuneInfo, Operations::Ram, Operations::Cpu, Operations::vmName);

        hostCpus = join_str(tuneInfo.freeCores, QStringLiteral(","),
                                          (const std::function<QString (const CPU_CORE &item) >)
                                           []
                                            (const CPU_CORE item)->QString{return item.CPU; });

        Operations::BashCommand(QStringLiteral("systemctl set-property --runtime -- user.slice AllowedCPUs=")+hostCpus);
        Operations::BashCommand(QStringLiteral("systemctl set-property --runtime -- system.slice AllowedCPUs=")+hostCpus);
        Operations::BashCommand(QStringLiteral("systemctl set-property --runtime -- init.scope AllowedCPUs=")+hostCpus);

        Operations::BashCommand(QStringLiteral("systemctl restart libvirtd.service"));
    }

    void Operations::UnPartitionCpu()
    {
        Operations::BashCommand(QStringLiteral("systemctl set-property --runtime -- user.slice AllowedCPUs=0-2000"));
        Operations::BashCommand(QStringLiteral("systemctl set-property --runtime -- system.slice AllowedCPUs=0-2000"));
        Operations::BashCommand(QStringLiteral("systemctl set-property --runtime -- init.scope AllowedCPUs=0-2000"));
    }

    void Operations::SendMouse()
    {
        Operations::Init();
        QString cmd = QStringLiteral("virsh attach-device ")+
            Operations::vmName+
            QStringLiteral(" ")+
            CONFIG_FOLDER + QStringLiteral("/toggle_mouse.xml");
        Operations::BashCommand(
            cmd
        );
    }

    void Operations::ReceiveMouse()
    {
        Operations::Init();
        QString cmd = QStringLiteral("virsh detach-device ")+
            Operations::vmName+
            QStringLiteral(" ")+
            CONFIG_FOLDER + QStringLiteral("/toggle_mouse.xml");
        Operations::BashCommand(
            cmd
        );
    }






    void Operations::GO(GpuWatcherDaemon *angel)
    {
        //qInfo("Status:%d\n", Operations::pointOfFailure);

        // Make sure killing the xserver doesn't kill this process
        //DAEMONIZE(0);

        angel->guard(QStringLiteral("Before: __Init()"));
        Operations::__Init();
        angel->guard(QStringLiteral("After: __Init()"));

        angel->guard(QStringLiteral("Before: ClearLog()"));
        Operations::ClearLog();
        angel->guard(QStringLiteral("After: ClearLog()"));

        angel->guard(QStringLiteral("Before: ClearLog()"));
        Operations::DEBUG();
        angel->guard(QStringLiteral("Before: ClearLog()"));
        //Operations::debugOnly = true;

        //
        // Stop the XServer
        Operations::StopX();
        if (Operations::pointOfFailure != 0) {
            goto recovery;
        }

        QThread::sleep(10); // Race condition brute force fix

        // Unload the drivers, remove the GPU
        Operations::UnbindGPU();
        if (Operations::pointOfFailure != 0) {
            goto recovery;
        }


        // Copy the VM XConfig
        Operations::CopyConfig();
        if (Operations::pointOfFailure != 0) {
            goto recovery;
        }

        // Reload graphics drivers starting with vfio, then regular for possible backup gpu
        Operations::BindGPU();
        if (Operations::pointOfFailure != 0) {
            goto recovery;
        }

        Operations::PartitionCpu();
        if (Operations::pointOfFailure != 0) {
            goto recovery;
        }

        // Start X Server
        Operations::StartX();
        if (Operations::pointOfFailure != 0) {
            goto recovery;
        }
        QThread::sleep(10);

        // Start VM
        Operations::StartVM(); // MAKE THIS SPECIAL FORK/EXEC
        if (Operations::pointOfFailure != 0) {
            goto recovery;
        }
        //
        goto monitor;
recovery:
        // TODO: figure out unwind based upon point of pointOfFailure
        Operations::AppendLog(QString("Point of Failure:") + QString::number(Operations::pointOfFailure));
        goto false_recovery;
        return;

monitor:
        // TODO: wait for vm to exit
        QThread::sleep(20);
        for(;;){
            BashCommandResult res = BashCommandNoLog(QStringLiteral("virsh list --name | grep ")+ Operations::vmName);
            if (!res.Success)
                break;
        }
false_recovery:
        if (Operations::revertOnVmExit) {
            Operations::StopX();
            Operations::UnbindGPU();
            QThread::sleep(10);
            Operations::RevertConfig();
            QThread::sleep(10);
            Operations::WipeKScreen(Operations::userName);
            Operations::RevertGPU();
            QThread::sleep(10);
            Operations::UnPartitionCpu();
            Operations::StartX();
        }
        exit(EXIT_SUCCESS);
    }
//}
///////////////////////////////////////////////////////////////////////////////////////////////////
