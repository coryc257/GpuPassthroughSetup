#include "XEdit.h"
#include "ui_XEdit.h"

#include <QSignalMapper>
#include <QMessageBox>

#include "../src/exec_container.h"


XEdit::XEdit(QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::XEdit)
{
    m_ui->setupUi(this);

}

XEdit::~XEdit() = default;

QString XEdit::getXConfig()
{
    return m_ui->txtXConf->toPlainText();
}

void XEdit::setDetails(QString output, QString helpText)
{
    m_ui->txtHelpText->setText(helpText);
    m_ui->txtXConf->setText(output);
}

