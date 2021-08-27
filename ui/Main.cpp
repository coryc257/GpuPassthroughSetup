#include "Main.h"
#include "ui_Main.h"

#include <QSignalMapper>
#include <QMessageBox>

#include "../src/exec_container.h"
#include "CommandOutputHelp.h"


Main::Main(QWidget* parent) :
    QMainWindow(parent),
    m_ui(new Ui::Main)
{
    m_ui->setupUi(this);

    this->originalHeight = this->height();
    this->scrollerOriginalHeight = m_ui->scrollArea->height();

    Operations::Init();

    m_ui->txtVmName->setText(Operations::vmName);

    connect(m_ui->actionExit, SIGNAL(triggered()), this, SLOT(CloseMe()));
    connect(m_ui->actionHelp, SIGNAL(triggered()), this, SLOT(Help()));
    connect(m_ui->btnGo, SIGNAL(clicked()), this, SLOT(Go()));
    connect(m_ui->btnVmNameFind, SIGNAL(clicked()), this, SLOT(finderVmName()));
    connect(m_ui->btnVmNameSave, SIGNAL(clicked()), this, SLOT(saveVmName()));
}

void Main::Go()
{
    Operations::GO();
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
    dialog.exec();

    if (dialog.result() == QDialog::DialogCode::Accepted) {
        m_ui->txtVmName->setText(dialog.getSelection());
    }
}

// Savers
void Main::saveVmName()
{
    Operations::SetVMName(m_ui->txtVmName->text());
}


