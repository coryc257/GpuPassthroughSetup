#ifndef H_exec_container
#define H_exec_container

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QMap>
#include <QList>
#include "../util/SAFE_RETURN.h"
#include <QSharedMemory>
#include "../src/daemonize.h"

class ExecContainer
{
public:
    ExecContainer(QString program, QStringList args);

    bool Run();
    QString GetOutputString();
private:
    QString program;
    QStringList args;
    QString output;
};

struct __EXEC_INSTANCE {

    };

class BashCommandResult
{
    public:
        QString Output;
        bool Success;
};

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

class CpuTune
{
public:
    bool Success;
    QString ErrorMessage;
    QList<CPU_CORE> assignedCores;
    QList<CPU_CORE> freeCores;
    QList<int> emulatorPin;
    QMap<int, int> l3Groups;
    QMap<int, QList<CPU_CORE>> l1Groups;
    QString vmConfig;
    int ram;
    int ramKiB;
    int cores;
};

class PciPassthroughInfo
{
public:
    QStringList devices;
    QString vmConfig;
};

class Operations
{
public:
    static void Init();
    static void SaveNonVmXConfig();
    static void SaveNonVmXConfig(QString xConfig);
    static void SaveVmXConfig();
    static void SaveVmXConfig(QString xConfig);


    static void SetQEmuCommandLine(QString vmName, QString device);
    static void SavePassthroughMouse(QString mouseIdentity);
    static void SaveRamCpu(QString ramGB, QString cpuCores, QString vmName);

    static void SetNonVmXConfig (SAFE_RETURN *retVal, QString xConfigFile);
    static void SetVmXConfig    (SAFE_RETURN *retVal, QString xConfigFile);
    static void SetUser         (SAFE_RETURN *retVal, QString userName);
    static void SetVMName       (SAFE_RETURN *retVal, QString vmName);
    static void SaveIOMMU       (SAFE_RETURN *retVal, QString iommuGroup, QString vmName);

    static void SendMouse();
    static void ReceiveMouse();

    static void GO(GpuWatcherDaemon *angel);

    static QString vmName;
    static QString userName;
    static QString vmConfig;
    static QString normalConfig;
    static QString evDevKeyboard;
    static QString passthroughMouse;
    static QStringList deviceList;
    static QStringList detachedDeviceList;
    static QString IOMMUGroup;
    static QString Ram;
    static QString Cpu;
    static bool revertOnVmExit;



    static BashCommandResult BashCommand(QString command);
    static BashCommandResult BashCommandNoLog(QString command);
    static QString CatThat(QString fileName);



private:

    static __EXEC_INSTANCE me;
    static int pointOfFailure;
    static CpuTune __tuneInfo;

    static void WipeKScreen(QString userName);
    static void __Init();
    static void ClearLog();
    static void AppendLog(QString logText);
    static void StopX();
    static void StartX();
    static void RevertGPU();
    static void Reboot();
    static void CopyConfig();
    static void RevertConfig();
    static void UnbindGPU();
    static void BindGPU();
    static void StartVM();
    static void PartitionCpu();
    static void UnPartitionCpu();

    static void DEBUG();


    static bool debugOnly;
    static bool reloadNvidia;

};

#endif
