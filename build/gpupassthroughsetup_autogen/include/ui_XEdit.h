/********************************************************************************
** Form generated from reading UI file 'XEdit.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XEDIT_H
#define UI_XEDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_XEdit
{
public:
    QDialogButtonBox *buttonBox;
    QTextEdit *txtXConf;
    QLineEdit *txtHelpText;

    void setupUi(QDialog *XEdit)
    {
        if (XEdit->objectName().isEmpty())
            XEdit->setObjectName(QString::fromUtf8("XEdit"));
        XEdit->resize(754, 535);
        buttonBox = new QDialogButtonBox(XEdit);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(80, 480, 621, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        txtXConf = new QTextEdit(XEdit);
        txtXConf->setObjectName(QString::fromUtf8("txtXConf"));
        txtXConf->setGeometry(QRect(23, 60, 711, 401));
        txtXConf->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        txtXConf->setReadOnly(false);
        txtXConf->setAcceptRichText(false);
        txtHelpText = new QLineEdit(XEdit);
        txtHelpText->setObjectName(QString::fromUtf8("txtHelpText"));
        txtHelpText->setGeometry(QRect(20, 10, 721, 36));
        txtHelpText->setReadOnly(true);

        retranslateUi(XEdit);
        QObject::connect(buttonBox, SIGNAL(accepted()), XEdit, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), XEdit, SLOT(reject()));

        QMetaObject::connectSlotsByName(XEdit);
    } // setupUi

    void retranslateUi(QDialog *XEdit)
    {
        XEdit->setWindowTitle(QCoreApplication::translate("XEdit", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XEdit: public Ui_XEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XEDIT_H
