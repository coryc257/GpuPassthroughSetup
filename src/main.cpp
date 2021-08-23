#include "gpupassthroughsetup.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GpuPassthroughSetup w;
    w.show();

    return app.exec();
}

