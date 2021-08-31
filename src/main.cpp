#include "../ui/Main.h"
#include <QApplication>
#include <QThread>
#include "exec_container.h"
#include "daemonize.h"
#include "../util/SAFE_RETURN.h"
#include "exec_container.h"

int main(int argc, char *argv[])
{
    //QApplication app(argc, argv);
    //GpuPassthroughSetup w;

    /*
    DAEMONIZE(0);
    x_DOWN();
    QThread::sleep(5);
    gpu_unbind();
    gpu_rebind();
    x_UP();
    */
    SAFE_RETURN snake;
    //GpuWatcherDaemon::Exec(&snake, QStringLiteral("win10"));
    GpuWatcherDaemon daemon(true);
    Operations::SetVMName(&snake, QStringLiteral("Itunes"));
    Operations::GO(&daemon);
    return 0;

    // TODO: PCI Device Selection
    if (argc == 1 ) { //GUI
        QApplication app(argc, argv);
        Main mainWindow;

        mainWindow.show();
        return app.exec();

    } else if (strcmp(argv[1],"--help") == 0) {
        qInfo("GPU PASSTHROUGH SETUP USAGE GUIDE\nALL AS ROOT\n");
        qInfo("!!! WHEN YOU RUN GO HAVE NOTHING IMPORTANT OPEN !!!\n");
        qInfo("Step 1: Set up X11 how you want it for normal linux only usage with no passthrough and run:\n\t%s NONVM\n", argv[0]);
        qInfo("Step 2: Set up X11 how you want it for passthrough usage and run:\n\t%s VMCFG\n", argv[0]);
        qInfo("\t\t*This might be with a secondary gpu or just the same config if you are going to consume the single video card\n");
        qInfo("Step 3: Set up the VM Name\n\t%s VMNAME <vm name here>\n", argv[0]);
        qInfo("Step 4: Set up the User Name Running the Desktop Environment\n\t%s USER <user name here>\n", argv[0]);
        qInfo("Step 5: Hold on to your butts\n\t%s GO", argv[0]);
    } else {
        // DEPRECATED
        /*Operations::Init();
        if (strcmp(argv[1], "VMNAME") == 0) {
            if (argc == 3) {
                Operations::SetVMName(QLatin1String(argv[2]));
            } else {
                qInfo("You did not provide a name\n");
            }
        } else if (strcmp(argv[1], "USER") == 0) {
            if (argc == 3) {
                Operations::SetUser(QLatin1String(argv[2]));
            } else {
                qInfo("You did not provide a username\n");
            }
        }
        else if (strcmp(argv[1],"NONVM") == 0) {
            Operations::SaveNonVmXConfig();
        } else if (strcmp(argv[1],"VMCFG") == 0) {
            Operations::SaveVmXConfig();
        } else if (strcmp(argv[1],"GO") == 0) {
            // It had to be done
            Operations::GO();
            //Operations::WipeKScreen(QStringLiteral("el"));
        }*/
    }
}

