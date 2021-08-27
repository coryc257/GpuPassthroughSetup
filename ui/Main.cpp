#include "Main.h"
#include "ui_Main.h"

#include <QSignalMapper>
#include <QMessageBox>

#include "../src/exec_container.h"

QAction actionExit;

Main::Main(QWidget* parent) :
    QMainWindow(parent),
    m_ui(new Ui::Main)
{
    m_ui->setupUi(this);

    connect(m_ui->actionExit, SIGNAL(triggered()), this, SLOT(CloseMe()));
    connect(m_ui->actionHelp, SIGNAL(triggered()), this, SLOT(Help()));
    connect(m_ui->btnGo, SIGNAL(clicked()), this, SLOT(Go()));
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




Main::~Main() = default;
