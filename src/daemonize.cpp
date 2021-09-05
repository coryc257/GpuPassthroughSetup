/*************************************************************************\
*                  Copyright (C) Cory Craig 2021                          *
*   Adapted from:                                                         *
* https://man7.org/tlpi/code/online/book/daemons/become_daemon.c.html     *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU Lesser General Public License as published   *
* by the Free Software Foundation, either version 3 or (at your option)   *
* any later version. This program is distributed without any warranty.    *
* See the files COPYING.lgpl-v3 and COPYING.gpl-v3 for details.           *
\*************************************************************************/

/* become_daemon.c

   A function encapsulating the steps in becoming a daemon.
*/

#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE 600
#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/un.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/wait.h>

#include "daemonize.h"
#include "../src/exec_container.h"
#include <QSharedMemory>
#include <QDateTime>



static void *watch_location;

void GpuWatcherDaemon::Exec(SAFE_RETURN* failInfo, QString vmName, bool restartX)
{
    pid_t childPid = 0;
    int err;

    switch (childPid = fork()) {
        case -1:
            failInfo->StatusCode = SAFE_RETURN_CODES::FORK_ERROR;
            err = errno;
            failInfo->returnData[QStringLiteral("ForkErrorNumber")] = RETURN_TYPE::fromQString(QString::number(err));
            return;
        case 0:
            GpuWatcherDaemon::Server(vmName, restartX);
            break;
        default:
            GpuWatcherDaemon::Client(failInfo, vmName, childPid);
            break;
    }

}

GpuWatcherDaemon::GpuWatcherDaemon(bool isDaemon)
{
    this->shm = nullptr;
}

GpuWatcherDaemon::~GpuWatcherDaemon()
{
    if (this->shm != nullptr) {
        this->shm->unlock();
        this->shm->detach();
    }
}

QString GpuWatcherDaemon::GetShmName(QString vmName)
{
    return QStringLiteral("GPUPASSTHROUGH__SHM__FOR__") + vmName;
}


void GpuWatcherDaemon::Client(SAFE_RETURN* failInfo, QString vmName, pid_t child)
{
    GpuWatcherDaemon application(false);
    application.shm = new QSharedMemory();
    INSTANCE_INFO *info;

    application.shm->setKey(GpuWatcherDaemon::GetShmName(vmName));
    QDateTime startDateTime = QDateTime::currentDateTime();
    while (!application.shm->create(sizeof(INSTANCE_INFO))) {
        application.shm->attach();
        application.shm->detach();

        if (startDateTime.msecsTo(QDateTime::currentDateTime()) > 5000) {
            failInfo->StatusCode = SAFE_RETURN_CODES::SHM_CREATE;
            failInfo->returnData["ShmCreateError"] = RETURN_TYPE::fromQString(application.shm->errorString());
            kill(child,SIGKILL);
            waitpid(child, NULL, 0);
            return;
        }
    }

    application.shm->lock();
    info = (INSTANCE_INFO *)application.shm->data();
    info->initialized = 257;
    info->serverPid = child;
    info->daemonPid = 0;
    info->serverFinished = 0;
    info->daemonFinished = 0;
    info->err = 0;
    memset(info->message,0,2000);

    printf("%s\n", "Initialized");
    application.shm->unlock();
    for (;;) {
        application.shm->lock();
        info = (INSTANCE_INFO *)application.shm->data();
        if (info->initialized == 1) {
            break;
        }
        application.shm->unlock();
    }
    printf("%s\n", "GOT CONFIRMATION");
}

void GpuWatcherDaemon::Server(QString vmName, bool restartX)
{
    GpuWatcherDaemon daemon(true);
    daemon.shm = new QSharedMemory();
    INSTANCE_INFO *info;
    pid_t child = 0;
    int maxfd, fd;

    daemon.shm->setKey(GpuWatcherDaemon::GetShmName(vmName));
    for(;;) {
        while(!daemon.shm->attach());
        daemon.shm->lock();
        info = (INSTANCE_INFO *)daemon.shm->data();
        if (info->initialized == 257 && info->serverPid == getpid()) {
            info->initialized = 1;
            break;
        }
        daemon.shm->unlock();
        daemon.shm->detach();
        sleep(5);
    }
    printf("%s\n", "GOT INIT");
    fflush(stdout);
    daemon.shm->unlock();

    if (setsid() == -1) {                 /* Become leader of new session */
        daemon.shm->lock();
        info = (INSTANCE_INFO *)daemon.shm->data();
        info->err = errno;
        strcpy(info->message, "Error at GpuWatcherDaemon::Server::setsid()");

        goto die_with_lock;
    }

    switch (child = fork()) {
    case -1:
        daemon.shm->lock();
        info = (INSTANCE_INFO *)daemon.shm->data();
        info->err = errno;
        strcpy(info->message, "Error at GpuWatcherDaemon::Server::fork()");

        goto die_with_lock;
    case 0:
        daemon.shm->lock();
        info = (INSTANCE_INFO *)daemon.shm->data();
        info->daemonPid = getpid();
        daemon.shm->unlock();
        break;
    default:
        daemon.shm->lock();
        info = (INSTANCE_INFO *)daemon.shm->data();
        info->serverFinished = 1;
        daemon.shm->unlock();
        daemon.shm->detach();
        _exit(EXIT_SUCCESS);
    }

    umask(0);
    chdir("/");

    maxfd = sysconf(_SC_OPEN_MAX);
    if (maxfd == -1)                /* Limit is indeterminate... */
        maxfd = BD_MAX_CLOSE;       /* so take a guess */

    for (fd = 0; fd < maxfd; fd++)
        close(fd);

    close(STDIN_FILENO);
    fd = open("/dev/null", O_RDWR);

    if (fd != STDIN_FILENO) {
        daemon.shm->lock();
        info = (INSTANCE_INFO *)daemon.shm->data();
        info->err = errno;
        strcpy(info->message, "Error at GpuWatcherDaemon::Server::open(STDIN)");

        goto die_with_lock;
    }
    if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO) {
        daemon.shm->lock();
        info = (INSTANCE_INFO *)daemon.shm->data();
        info->err = errno;
        strcpy(info->message, "Error at GpuWatcherDaemon::Server::dup2(STDOUT)");

        goto die_with_lock;

    }
    if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO) {
        daemon.shm->lock();
        info = (INSTANCE_INFO *)daemon.shm->data();
        info->err = errno;
        strcpy(info->message, "Error at GpuWatcherDaemon::Server::fork(STDERR)");

        goto die_with_lock;
    }

    goto exec_container;


die_with_lock:
    daemon.shm->unlock();
    daemon.shm->detach();
    return;

exec_container:
    Operations::revertOnVmExit = true;
    Operations::GO(&daemon);
    return;
}

void GpuWatcherDaemon::guard(QString message)
{
    if (this->shm == nullptr)
        return;
    INSTANCE_INFO *info;
    this->shm->lock();
    info = (INSTANCE_INFO *)this->shm->data();
    if (message.length() < sizeof(info->status)) {
        memset(info->status, 0, sizeof(info->status));
        strcpy(info->status, message.toUtf8().data());
    }
    this->shm->unlock();
}

