#include "Main.h"
#include "ui_Main.h"

#include <QSignalMapper>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

#include "../src/exec_container.h"
#include "CommandOutputHelp.h"
#include "XEdit.h"
#include "../util/MsgBox.h"
#include "../util/SAFE_RETURN.h"
#include "../util/HANDLE_UI_RETURN.h"
#include "../src/daemonize.h"


Main::Main(QWidget* parent) :
    QMainWindow(parent),
    m_ui(new Ui::Main)
{
    m_ui->setupUi(this);

    this->mouseState = false;
    this->originalHeight = this->height();
    this->scrollerOriginalHeight = m_ui->scrollArea->height();

    Operations::Init();

    m_ui->txtVmName->setText(Operations::vmName);
    m_ui->txtVmXConfig->setText(Operations::vmConfig);
    m_ui->txtNormalXConfig->setText(Operations::normalConfig);
    m_ui->txtEvDevKeyboard->setText(Operations::evDevKeyboard);
    m_ui->txtPassthroughMouse->setText(Operations::passthroughMouse);
    m_ui->txtUsername->setText(Operations::userName);
    m_ui->txtIOMMUGroup->setText(Operations::IOMMUGroup);
    m_ui->txtCores->setText(Operations::Cpu);
    m_ui->txtRam->setText(Operations::Ram);
    m_ui->chkRestartX->setCheckState(Qt::CheckState::Unchecked);

    connect(m_ui->actionExit,               SIGNAL(triggered()),    this, SLOT(CloseMe()));
    connect(m_ui->actionHelp,               SIGNAL(triggered()),    this, SLOT(Help()));
    connect(m_ui->btnGo,                    SIGNAL(clicked()),      this, SLOT(Go()));
    connect(m_ui->btnVmNameFind,            SIGNAL(clicked()),      this, SLOT(finderVmName()));
    connect(m_ui->btnVmNameSave,            SIGNAL(clicked()),      this, SLOT(saveVmName()));
    connect(m_ui->btnVmXConfigFind,         SIGNAL(clicked()),      this, SLOT(finderVmXConfig()));
    connect(m_ui->btnVMXConfigPick,         SIGNAL(clicked()),      this, SLOT(saveVmXConfigChooser()));
    connect(m_ui->btnEvDevKeyboardFind,     SIGNAL(clicked()),      this, SLOT(finderEvDevKeyboard()));
    connect(m_ui->btnPassthroughMouseFind,  SIGNAL(clicked()),      this, SLOT(finderPassthroughMouse()));
    connect(m_ui->btnPassthroughMouseSave,  SIGNAL(clicked()),      this, SLOT(savePassthroughMouse()));
    connect(m_ui->btnUsernameSave,          SIGNAL(clicked()),      this, SLOT(saveUsername()));
    connect(m_ui->btnUsernameFind,          SIGNAL(clicked()),      this, SLOT(finderUserName()));
    connect(m_ui->btnEvDevKeyboardEdit,     SIGNAL(clicked()),      this, SLOT(finderEvDevKeyboard()));
    connect(m_ui->btnNormalXConfigFind,     SIGNAL(clicked()),      this, SLOT(finderNormalXConfig()));
    connect(m_ui->btnNormalXConfigPick,     SIGNAL(clicked()),      this, SLOT(saveNormalXConfigChooser()));
    connect(m_ui->btnIOMMUGroupFind,        SIGNAL(clicked()),      this, SLOT(findIOMMU()));
    connect(m_ui->btnEditRamCPU,            SIGNAL(clicked()),      this, SLOT(saveCpuRam()));
    connect(m_ui->btnIOMMUGroupSave,        SIGNAL(clicked()),      this, SLOT(saveIOMMU()));
    connect(m_ui->btnMouseToggle,           SIGNAL(clicked()),      this, SLOT(toggleMouse()));
}

void Main::Go()
{
    SAFE_RETURN status;
    GpuWatcherDaemon::Exec(&status,
                           m_ui->txtVmName->text(),
                           m_ui->chkRestartX->checkState() == Qt::CheckState::Checked);
    // This method will only return on error
    if (!IS_OK(status))
        HANDLE_UI_RETURN::HANDLE(&status);
}

void Main::CloseMe()
{
    this->close();
}

void Main::Help()
{
    QMessageBox msg;
    msg.setWindowTitle(QStringLiteral("No help for you."));
    msg.setText(QStringLiteral("Maybe some help for you"));
    msg.exec();
}

void Main::resizeEvent(QResizeEvent *e)
{
    int h = m_ui->centralwidget->height();
    m_ui->scrollArea->setFixedHeight(h - 95);
}


Main::~Main() = default;


// Finders

void Main::finderVmName()
{
    CommandOutputHelp dialog;
    dialog.setWindowTitle(QStringLiteral("VM Name Helper"));
    BashCommandResult res = Operations::BashCommand("virsh list --all");

    if (res.Success) {
        dialog.setDetails(res.Output,QStringLiteral("Copy Paste the name you want."));
        dialog.exec();

        if (dialog.result() == QDialog::DialogCode::Accepted) {
            m_ui->txtVmName->setText(dialog.getSelection());
        }
    } else {
        MsgBox(QStringLiteral("Failed"));
    }
}

void Main::finderUserName()
{
    MsgBox(QStringLiteral("The user you log into that isnt root"));
}

void Main::finderVmXConfig()
{
    QString xConfig;

    if (QFile::exists(QStringLiteral("/root/.gpu_passthrough/vm_config.conf"))) {
        xConfig = Operations::CatThat(QStringLiteral("/root/.gpu_passthrough/vm_config.conf"));
    } else {
        xConfig = Operations::CatThat(QStringLiteral("/etc/X11/xorg.conf"));
    }

    XEdit edit;
    edit.setDetails(xConfig, QStringLiteral("Set up your XConfig how you want it then come back here and click ok or edit it right here right now"));
    edit.exec();
    if (edit.result() == QDialog::DialogCode::Accepted) {
        Operations::SaveVmXConfig(edit.getXConfig());
    }
}

void Main::finderNormalXConfig()
{
    QString xConfig;

    if (QFile::exists(QStringLiteral("/root/.gpu_passthrough/non_vm_config.conf"))) {
        xConfig = Operations::CatThat(QStringLiteral("/root/.gpu_passthrough/non_vm_config.conf"));
    } else {
        xConfig = Operations::CatThat(QStringLiteral("/etc/X11/xorg.conf"));
    }

    XEdit edit;
    edit.setDetails(xConfig, QStringLiteral("Set up your XConfig how you want it then come back here and click ok or edit it right here right now"));
    edit.exec();
    if (edit.result() == QDialog::DialogCode::Accepted) {
        Operations::SaveNonVmXConfig(edit.getXConfig());
    }
}

void Main::finderEvDevKeyboard()
{
    CommandOutputHelp helper;
    BashCommandResult res = Operations::BashCommand(QStringLiteral("ls -n /dev/input/by-id/ | grep event-kbd | awk '{ print $9 \" | \" $11;}'"));
    helper.setDetails(res.Output, QStringLiteral("Find your keyboard and put: /dev/input/event<#>"));
    helper.setDefault(QStringLiteral("/dev/input/event"));
    helper.exec();
    if (helper.result() == QDialog::DialogCode::Accepted){
        this->m_ui->txtEvDevKeyboard->setText(helper.getSelection());
        this->saveEvDevKeyboard();
    }
}

void Main::finderPassthroughMouse()
{
    CommandOutputHelp helper;
    BashCommandResult res = Operations::BashCommand((QStringLiteral("lsusb")));
    if (res.Success) {
        helper.setDetails(res.Output, QStringLiteral("Find your mouse and paste in xxxx:xxxx"));
        helper.exec();
        if (helper.result() == QDialog::DialogCode::Accepted) {
            m_ui->txtPassthroughMouse->setText(helper.getSelection());
        }
    }
}

void Main::findIOMMU()
{
    CommandOutputHelp helper;
    BashCommandResult res = Operations::BashCommand(QStringLiteral("./find_groups.sh"));
    if (res.Success) {
        helper.setDetails(res.Output, QStringLiteral("Find you IOMMU Group and paste in 'IOMMU GROUP <#>'"));
        helper.exec();
        if (helper.result() == QDialog::DialogCode::Accepted) {
            m_ui->txtIOMMUGroup->setText(helper.getSelection());
        }
    }
}


// Savers
void Main::saveVmName()
{
    SAFE_RETURN status;
    Operations::SetVMName(&status, m_ui->txtVmName->text());
    if (!IS_OK(status)) {
        HANDLE_UI_RETURN::HANDLE(&status);
    }
}

void Main::saveUsername()
{
    SAFE_RETURN status;
    Operations::SetUser(&status, m_ui->txtUsername->text());
    if (!IS_OK(status)) {
        HANDLE_UI_RETURN::HANDLE(&status);
    }
}

void Main::saveVmXConfigChooser()
{
    QFileDialog diag;
    diag.exec();

    if (diag.result() == QFileDialog::DialogCode::Accepted) {
        SAFE_RETURN status;
        Operations::SetVmXConfig(&status, diag.selectedFiles()[0]);
        if (!IS_OK(status)) {
            HANDLE_UI_RETURN::HANDLE(&status);
        }
    }
}

void Main::saveNormalXConfigChooser()
{
    QFileDialog diag;
    diag.exec();

    if (diag.result() == QFileDialog::DialogCode::Accepted) {
        SAFE_RETURN status;
        Operations::SetNonVmXConfig(&status, diag.selectedFiles()[0]);
        if (!IS_OK(status)) {
            HANDLE_UI_RETURN::HANDLE(&status);
        }
    }
}

void Main::saveEvDevKeyboard()
{

    Operations::SetQEmuCommandLine(m_ui->txtVmName->text(), m_ui->txtEvDevKeyboard->text());

}

void Main::saveIOMMU()
{
    SAFE_RETURN status;
    Operations::SaveIOMMU(&status, m_ui->txtIOMMUGroup->text(), m_ui->txtVmName->text());
    if (!IS_OK(status)) {
        HANDLE_UI_RETURN::HANDLE(&status);
    }
}


void Main::savePassthroughMouse()
{
    Operations::SavePassthroughMouse(m_ui->txtPassthroughMouse->text());
}

void Main::saveCpuRam()
{
    Operations::SaveRamCpu(m_ui->txtRam->text(), m_ui->txtCores->text(), m_ui->txtVmName->text());
}

void Main::toggleMouse()
{
    if (this->mouseState) {
        Operations::SendMouse();
    } else {
        Operations::ReceiveMouse();
    }
    this->mouseState = !this->mouseState;
}









