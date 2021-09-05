#ifndef LIB_DAEMONIZE_H_
#define LIB_DAEMONIZE_H_


#include <QSharedMemory>
#include "../util/SAFE_RETURN.h"

#define BD_NO_CHDIR 01
#define BD_NO_CLOSE_FILES 02
#define BD_NO_REOPEN_STD_FDS 04
#define BD_NO_UMASK0 010
#define BD_MAX_CLOSE 819

typedef struct __INSTANCE_INFO {
    int initialized;
    pid_t serverPid;
    int serverFinished;
    pid_t daemonPid;
    int daemonFinished;
    int err;
    char message[2000];
    char status[2000];
} INSTANCE_INFO;


class GpuWatcherDaemon
{
public:
    static void Exec(SAFE_RETURN *failInfo, QString vmName, bool restartX);
    void guard(QString message);
    static QString GetShmName(QString vmName);

    GpuWatcherDaemon(bool isDaemon);
    ~GpuWatcherDaemon();
private:

    QSharedMemory *shm;

    static void Client(SAFE_RETURN *failInfo, QString vmName, pid_t child);
    static void Server(QString vmName, bool restartX);
};


#endif /* LIB_DAEMONIZE_H_ */
