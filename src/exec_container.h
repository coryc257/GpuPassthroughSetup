#ifndef H_exec_container
#define H_exec_container

#include <QString>
#include <QStringList>
#include <QProcess>

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

class Operations
{
public:
    static void Init();
    static void SaveNonVmXConfig();
    static void SaveNonVmXConfig(QString xConfig);
    static void SaveVmXConfig();
    static void SaveVmXConfig(QString xConfig);
    static void SetVmXConfig(QString xConfigFile);
    static void SetNonVmXConfig(QString xConfigFile);
    static void SetQEmuCommandLine(QString vmName, QString device);
    static void SetVMName(QString vmName);
    static void SetUser(QString userName);
    static void SavePassthroughMouse(QString mouseIdentity);
    static void GO();

    static QString vmName;
    static QString userName;
    static QString vmConfig;
    static QString normalConfig;
    static QString evDevKeyboard;
    static QString passthroughMouse;
    static QStringList deviceList;
    static QStringList detachedDeviceList;

    static BashCommandResult BashCommand(QString command);
    static BashCommandResult BashCommandNoLog(QString command);
    static QString CatThat(QString fileName);

private:

    static __EXEC_INSTANCE me;
    static int pointOfFailure;

    static void WipeKScreen(QString userName);
    static void __Init();
    static void ClearLog();
    static void AppendLog(QString logText);
    static void StopX();
    static void StartX();

    static void Reboot();
    static void CopyConfig();
    static void RevertConfig();
    static void UnbindGPU();
    static void BindGPU();
    static void StartVM();

    static void DEBUG();


    static bool debugOnly;
    static bool reloadNvidia;

};

#endif
