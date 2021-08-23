#ifndef GPUPASSTHROUGHSETUP_H
#define GPUPASSTHROUGHSETUP_H

#include <QMainWindow>
#include <QScopedPointer>

namespace Ui {
class GpuPassthroughSetup;
}

class GpuPassthroughSetup : public QMainWindow
{
    Q_OBJECT

public:
    explicit GpuPassthroughSetup(QWidget *parent = nullptr);
    ~GpuPassthroughSetup() override;

private:
    QScopedPointer<Ui::GpuPassthroughSetup> m_ui;
};

#endif // GPUPASSTHROUGHSETUP_H
