#include "exec_container.h"
#include <QTextCodec>
#include <QStandardPaths>
#include <QDir>
#include <QThread>
#include "daemonize.h"
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
    int Operations::pointOfFailure = 0;
    bool Operations::debugOnly = false;
    bool Operations::reloadNvidia = false;

    void Operations::Init()
    {
        if (!QDir(CONFIG_FOLDER).exists()) {
            QDir().mkdir(CONFIG_FOLDER);
        }
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
            Operations::Reboot();
            return QStringLiteral("lol");
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

        Operations::BashCommand(QStringLiteral("killall kwin_x11"));
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
        Operations::vmConfig = CONFIG_FOLDER + QStringLiteral("/vm_config.conf");
        Operations::normalConfig = CONFIG_FOLDER + QStringLiteral("/non_vm_config.conf");

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
