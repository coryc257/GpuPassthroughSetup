#include "CommandOutputHelp.h"
#include "ui_CommandOutputHelp.h"

#include <QSignalMapper>
#include <QMessageBox>

#include "../src/exec_container.h"


CommandOutputHelp::CommandOutputHelp(QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::CommandOutputHelp)
{
    m_ui->setupUi(this);

}

CommandOutputHelp::~CommandOutputHelp() = default;

QString CommandOutputHelp::getSelection()
{
    return m_ui->txtSelection->text();
}

void CommandOutputHelp::setDetails(QString output, QString helpText)
{
    m_ui->txtHelpText->setText(helpText);
    m_ui->txtCommandOutput->setText(output);
}

void CommandOutputHelp::setDefault(QString defaultValue)
{
    m_ui->txtSelection->setText(defaultValue);
}

