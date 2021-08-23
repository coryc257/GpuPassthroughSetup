#include "gpupassthroughsetup.h"
#include "ui_gpupassthroughsetup.h"

GpuPassthroughSetup::GpuPassthroughSetup(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::GpuPassthroughSetup)
{
    m_ui->setupUi(this);
}

GpuPassthroughSetup::~GpuPassthroughSetup() = default;
