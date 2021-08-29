#include "exec_container.h"
#include <QTextCodec>
#include <QStandardPaths>
#include <QDir>
#include <QThread>
#include <QFile>
#include <QMap>
#include "daemonize.h"
#include "../util/MsgBox.h"
#define CONFIG_FOLDER QStandardPaths::standardLocations(QStandardPaths::StandardLocation::HomeLocation)[0] + QStringLiteral("/.gpu_passthrough")
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
    int Operations::pointOfFailure = 0;
    bool Operations::debugOnly = false;
    bool Operations::reloadNvidia = false;


    class CPU_CORE
    {
    public:
        QString CPU;
        QString NODE;
        QString SOCKET;
        QString CORE;
        QString L1d;
        QString L1i;
        QString L2;
        QString L3;
        QString ONLINE;
    };

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


        QString qEmuCommandLineXml = QStringLiteral("<qemu:commandline>\n\t<qemu:arg value='-object'/>\n\t<qemu:arg value='input-linux,id=kbd1,evdev=/dev/input/event")+
                                    device+QStringLiteral(",grab_all=on,repeat=on'/>\n</qemu:commandline>");

        BashCommand(QStringLiteral("printf \"%s\" \"")+device+QStringLiteral("\" > ") + CONFIG_FOLDER + QStringLiteral("/keyboard.param"));
        QProcess virshTerm;
        virshTerm.startDetached("xterm", QStringList({QStringLiteral("-e"), QStringLiteral("virsh"), QStringLiteral("edit"), vmName}));
        MsgBox(QStringLiteral("Place this under </devices> before </domain>... Middle click to paste\n\n")+qEmuCommandLineXml);
    }

    void Operations::SavePassthroughMouse(QString mouseIdentity)
    {
        QStringList mParts = mouseIdentity.trimmed().split(QStringLiteral(":"));
        if (mParts.count() == 2) {
            QString mouseToggleXML =    QStringLiteral("#")+mouseIdentity+QStringLiteral("\n")+
                                        QStringLiteral("#FirstLineMustbe xxxx:xxxx\n<hostdev mode='subsystem' type='usb' managed='no'>\n\t<source>\n\t\t<vendor id='0x") +
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

    class PciPassthroughInfo
    {
    public:
        QStringList devices;
        QString vmConfig;
    };

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

    void Operations::SaveIOMMU(QString iommuGroup, QString vmName)
    {
        BashCommandResult res = Operations::BashCommand(QStringLiteral("./find_groups.sh"));
        if (res.Success) {
            PciPassthroughInfo info = __get_pci_info(iommuGroup, res.Output);
            Operations::BashCommand(QStringLiteral("printf \"%s\" \"$1\" > $2")
                        .replace(QStringLiteral("$1"), iommuGroup)
                        .replace(QStringLiteral("$2"), CONFIG_FOLDER + QStringLiteral("/iommu.param"))
            );
            QProcess virshTerm;
            virshTerm.startDetached("xterm", QStringList({QStringLiteral("-e"), QStringLiteral("virsh"), QStringLiteral("edit"), vmName}));
            MsgBox(QStringLiteral("Put this in your config, make sure you don't have duplicate domain,bus,slot,function entries\n\n")+info.vmConfig);
        }
    }


    void Operations::SetVmXConfig(QString xConfigFile)
    {
        ExecContainer copy_command = ExecContainer(
                QStringLiteral("cp"),
                QStringList({
                    xConfigFile,
                    CONFIG_FOLDER + QStringLiteral("/vm_config.conf")
                }));
        copy_command.Run();
    }

    void Operations::SetNonVmXConfig(QString xConfigFile)
    {
        ExecContainer copy_command = ExecContainer(
                QStringLiteral("cp"),
                QStringList({
                    xConfigFile,
                    CONFIG_FOLDER + QStringLiteral("/non_vm_config.conf")
                }));
        copy_command.Run();
    }


    void Operations::SetVMName(QString vmName)
    {
        ExecContainer meow = ExecContainer(
                QStringLiteral("/bin/bash"),
                QStringList({
                    QStringLiteral("-c"),
                    QStringLiteral("printf \"")+vmName+QStringLiteral("\" > \"") + CONFIG_FOLDER + QStringLiteral("/vm_name.param\"")
                }));
        meow.Run();
    }

    void Operations::SetUser(QString userName)
    {
        Operations::BashCommand(
            QStringLiteral("printf \"")+userName+QStringLiteral("\" > \"") +
            CONFIG_FOLDER + QStringLiteral("/user_name.param\""));

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

    void Operations::SaveRamCpu(QString ramGB, QString cpuCores, QString vmName)
    {
        BashCommandResult cpuRes;
        QList<CPU_CORE> cpuInfo = __GetCpuInfo();
        QMap<int, int> l3Groups;
        QMap<int, QList<CPU_CORE>> l1Groups;
        QList<CPU_CORE> assignedCores;
        QList<int> emulatorPin;

        int ram = ramGB.split(QStringLiteral(" "))[0].toInt();
        int cores = cpuCores.split(QStringLiteral(" "))[0].toInt();

        Operations::BashCommandNoLog(QStringLiteral("printf \"%s\" \"$1\" > $2")
            .replace(QStringLiteral("$1"), QString::number(ram))
            .replace(QStringLiteral("$2"), CONFIG_FOLDER + QStringLiteral("/ram.param"))
        );

        Operations::BashCommandNoLog(QStringLiteral("printf \"%s\" \"$1\" > $2")
            .replace(QStringLiteral("$1"), QString::number(cores))
            .replace(QStringLiteral("$2"), CONFIG_FOLDER + QStringLiteral("/cpu.param"))
        );


        ram = 1024 * 1024 * ram;


        Q_FOREACH(CPU_CORE core, cpuInfo) {
            int l1 = core.L1d.toInt();
            int l3 = core.L3.toInt();

            if (!l3Groups.contains(l3)) {
                l3Groups[l3] = 1;
            } else {
                l3Groups[l3] += 1;
            }

            if (!l1Groups.contains(l1)) {
                l1Groups[l1] = QList<CPU_CORE>();
            }
            l1Groups[l1].append(core);
        }

        if (l3Groups.count() > 1) { // FORCE A SPLIT on the L3 Cache
            if (cores != l3Groups[1]) {
                QString sv = QString::number(l3Groups[1]);
                MsgBox(QStringLiteral("You must assign a number of cores equal to an L3 cache boundary(") + sv + QStringLiteral(")"));
                return;
            } else {
                Q_FOREACH(CPU_CORE core, cpuInfo) {
                    int l3 = core.L3.toInt();
                    if (l3 == 0) {
                        emulatorPin.append(core.CPU.toInt());
                    } else if (l3 == 1) {
                        assignedCores.append(core);
                    }
                }
            }
        } else { // DO WHAT TOLD
            // TODO
            if (cores % 2 != 0) {
                MsgBox(QStringLiteral("You must ask for an even number of cores"));
            }
            int groupCount = l1Groups.count();

            // Start with odds
            for (int j = 1; j < groupCount; j += 2) {
                Q_FOREACH(CPU_CORE core, l1Groups[l1Groups.keys()[j]]) {
                    assignedCores.append(core);
                }
                if (assignedCores.count() == cores)
                    goto got_cores;
            }

            // Work backwards
            // Make sure we have at least two cores to pin to the emulator
            for (int j = (groupCount % 2 == 0) ? groupCount - 2 : groupCount - 1; j > 0; j -= 2) {
                Q_FOREACH(CPU_CORE core, l1Groups[l1Groups.keys()[j]]) {
                    assignedCores.append(core);
                }
                if (assignedCores.count() == cores)
                    goto got_cores;
            }

            if (assignedCores.count() != cores) {
                MsgBox(QStringLiteral("You don't have enough cores"));
            }
        }

        got_cores:

            Q_FOREACH(CPU_CORE core, l1Groups[0]) {
                emulatorPin.append(core.CPU.toInt());
            }

            QString vmConfig = __generation_cpu_tune(assignedCores, emulatorPin, ram);
            QProcess virshTerm;
            virshTerm.startDetached("xterm", QStringList({QStringLiteral("-e"), QStringLiteral("virsh"), QStringLiteral("edit"), vmName}));

            MsgBox(QStringLiteral("Replace the memory, currentMemory, cputune, cpu and vcpu sections of your config with:\n\n")+vmConfig);

            return;
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

        for (int j = 0; j < Operations::deviceList.count(); j++) {
            if (!Operations::BashCommand(QStringLiteral("virsh nodedev-detach ")+Operations::deviceList[j]).Success) {
                Operations::pointOfFailure = 7;
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

    void Operations::StartVM()
    {
        //TODO: MAKE THIS REAL
        Operations::BashCommand(QStringLiteral("echo 0 > /sys/class/vtconsole/vtcon0/bind"));
        Operations::BashCommand(QStringLiteral("echo 0 > /sys/class/vtconsole/vtcon1/bind"));
        Operations::BashCommand(
            QStringLiteral("echo efi-framebuffer.0 > /sys/bus/platform/drivers/efi-framebuffer/unbind")
        );
        QThread::sleep(10);
        Operations::BashCommand(QStringLiteral("chown libvirt-qemu /dev/input/event3 ")+Operations::vmName);
        Operations::BashCommand(QStringLiteral("chown libvirt-qemu /dev/input/event10 ")+Operations::vmName);
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

        Operations::deviceList.append(QStringLiteral("pci_0000_01_00_0"));
        Operations::deviceList.append(QStringLiteral("pci_0000_01_00_1"));
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



    void Operations::GO()
    {
        qInfo("Status:%d\n", Operations::pointOfFailure);

        // Make sure killing the xserver doesn't kill this process
        DAEMONIZE(0);

        Operations::__Init();

        Operations::ClearLog();
        Operations::DEBUG();
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
        return;

monitor:
        // TODO: wait for vm to exit
        for(;;){

        }
    }
//}
///////////////////////////////////////////////////////////////////////////////////////////////////
