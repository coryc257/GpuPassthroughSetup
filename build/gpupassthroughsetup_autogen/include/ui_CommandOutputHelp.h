/********************************************************************************
** Form generated from reading UI file 'CommandOutputHelp.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMANDOUTPUTHELP_H
#define UI_COMMANDOUTPUTHELP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_CommandOutputHelp
{
public:
    QDialogButtonBox *buttonBox;
    QTextEdit *txtCommandOutput;
    QLineEdit *txtSelection;
    QLabel *lblMakeSelection;
    QLineEdit *txtHelpText;

    void setupUi(QDialog *CommandOutputHelp)
    {
        if (CommandOutputHelp->objectName().isEmpty())
            CommandOutputHelp->setObjectName(QString::fromUtf8("CommandOutputHelp"));
        CommandOutputHelp->resize(754, 409);
        buttonBox = new QDialogButtonBox(CommandOutputHelp);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(100, 360, 621, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        txtCommandOutput = new QTextEdit(CommandOutputHelp);
        txtCommandOutput->setObjectName(QString::fromUtf8("txtCommandOutput"));
        txtCommandOutput->setGeometry(QRect(23, 60, 711, 241));
        txtCommandOutput->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        txtCommandOutput->setReadOnly(true);
        txtSelection = new QLineEdit(CommandOutputHelp);
        txtSelection->setObjectName(QString::fromUtf8("txtSelection"));
        txtSelection->setGeometry(QRect(200, 310, 521, 36));
        lblMakeSelection = new QLabel(CommandOutputHelp);
        lblMakeSelection->setObjectName(QString::fromUtf8("lblMakeSelection"));
        lblMakeSelection->setGeometry(QRect(73, 318, 111, 22));
        txtHelpText = new QLineEdit(CommandOutputHelp);
        txtHelpText->setObjectName(QString::fromUtf8("txtHelpText"));
        txtHelpText->setGeometry(QRect(20, 10, 721, 36));
        txtHelpText->setReadOnly(true);

        retranslateUi(CommandOutputHelp);
        QObject::connect(buttonBox, SIGNAL(accepted()), CommandOutputHelp, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CommandOutputHelp, SLOT(reject()));

        QMetaObject::connectSlotsByName(CommandOutputHelp);
    } // setupUi

    void retranslateUi(QDialog *CommandOutputHelp)
    {
        CommandOutputHelp->setWindowTitle(QCoreApplication::translate("CommandOutputHelp", "Dialog", nullptr));
        lblMakeSelection->setText(QCoreApplication::translate("CommandOutputHelp", "Make Selection", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CommandOutputHelp: public Ui_CommandOutputHelp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMANDOUTPUTHELP_H
