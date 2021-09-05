/********************************************************************************
** Form generated from reading UI file 'Main.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Main
{
public:
    QAction *actionExit;
    QAction *actionHelp;
    QAction *actionReload;
    QWidget *centralwidget;
    QPushButton *btnMouseToggle;
    QPushButton *btnGo;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *widget;
    QLineEdit *txtVmName;
    QLabel *lblVMName;
    QPushButton *btnVmNameFind;
    QPushButton *btnVmNameSave;
    QWidget *widget_7;
    QLineEdit *txtIOMMUGroup;
    QLabel *lblIOMMUGroup;
    QPushButton *btnIOMMUGroupFind;
    QPushButton *btnIOMMUGroupSave;
    QWidget *widget_3;
    QLineEdit *txtVmXConfig;
    QLabel *lblVMXConfig;
    QPushButton *btnVmXConfigFind;
    QPushButton *btnVMXConfigPick;
    QWidget *widget_4;
    QLineEdit *txtNormalXConfig;
    QLabel *lblNormalXConfig;
    QPushButton *btnNormalXConfigFind;
    QPushButton *btnNormalXConfigPick;
    QWidget *widget_5;
    QLineEdit *txtEvDevKeyboard;
    QLabel *lblEvDevKeyboard;
    QPushButton *btnEvDevKeyboardFind;
    QPushButton *btnEvDevKeyboardEdit;
    QWidget *widget_6;
    QLineEdit *txtPassthroughMouse;
    QLabel *lblPassthroughMouse;
    QPushButton *btnPassthroughMouseFind;
    QPushButton *btnPassthroughMouseSave;
    QWidget *widget_2;
    QLineEdit *txtUsername;
    QLabel *lblUsername;
    QPushButton *btnUsernameFind;
    QPushButton *btnUsernameSave;
    QLabel *lblCores;
    QLabel *lblRam;
    QLineEdit *txtCores;
    QLineEdit *txtRam;
    QPushButton *btnEditRamCPU;
    QCheckBox *chkRestartX;
    QTextEdit *textEdit;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuAbout;

    void setupUi(QMainWindow *Main)
    {
        if (Main->objectName().isEmpty())
            Main->setObjectName(QString::fromUtf8("Main"));
        Main->resize(756, 639);
        Main->setMinimumSize(QSize(756, 0));
        Main->setMaximumSize(QSize(756, 16777215));
        Main->setTabShape(QTabWidget::Rounded);
        Main->setDockNestingEnabled(false);
        actionExit = new QAction(Main);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionHelp = new QAction(Main);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionReload = new QAction(Main);
        actionReload->setObjectName(QString::fromUtf8("actionReload"));
        centralwidget = new QWidget(Main);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        btnMouseToggle = new QPushButton(centralwidget);
        btnMouseToggle->setObjectName(QString::fromUtf8("btnMouseToggle"));
        btnMouseToggle->setGeometry(QRect(550, 0, 201, 71));
        btnGo = new QPushButton(centralwidget);
        btnGo->setObjectName(QString::fromUtf8("btnGo"));
        btnGo->setGeometry(QRect(10, 0, 531, 31));
        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(3, 90, 751, 511));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setMinimumSize(QSize(751, 0));
        scrollArea->setMaximumSize(QSize(751, 16777215));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        scrollArea->setWidgetResizable(false);
        scrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 726, 500));
        widget = new QWidget(scrollAreaWidgetContents);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(20, 10, 711, 71));
        txtVmName = new QLineEdit(widget);
        txtVmName->setObjectName(QString::fromUtf8("txtVmName"));
        txtVmName->setGeometry(QRect(10, 30, 481, 36));
        lblVMName = new QLabel(widget);
        lblVMName->setObjectName(QString::fromUtf8("lblVMName"));
        lblVMName->setGeometry(QRect(10, 10, 81, 22));
        btnVmNameFind = new QPushButton(widget);
        btnVmNameFind->setObjectName(QString::fromUtf8("btnVmNameFind"));
        btnVmNameFind->setGeometry(QRect(500, 30, 99, 38));
        btnVmNameSave = new QPushButton(widget);
        btnVmNameSave->setObjectName(QString::fromUtf8("btnVmNameSave"));
        btnVmNameSave->setGeometry(QRect(600, 30, 99, 38));
        widget_7 = new QWidget(scrollAreaWidgetContents);
        widget_7->setObjectName(QString::fromUtf8("widget_7"));
        widget_7->setGeometry(QRect(20, 430, 711, 71));
        txtIOMMUGroup = new QLineEdit(widget_7);
        txtIOMMUGroup->setObjectName(QString::fromUtf8("txtIOMMUGroup"));
        txtIOMMUGroup->setGeometry(QRect(10, 30, 481, 36));
        lblIOMMUGroup = new QLabel(widget_7);
        lblIOMMUGroup->setObjectName(QString::fromUtf8("lblIOMMUGroup"));
        lblIOMMUGroup->setGeometry(QRect(10, 10, 191, 22));
        btnIOMMUGroupFind = new QPushButton(widget_7);
        btnIOMMUGroupFind->setObjectName(QString::fromUtf8("btnIOMMUGroupFind"));
        btnIOMMUGroupFind->setGeometry(QRect(500, 30, 99, 38));
        btnIOMMUGroupSave = new QPushButton(widget_7);
        btnIOMMUGroupSave->setObjectName(QString::fromUtf8("btnIOMMUGroupSave"));
        btnIOMMUGroupSave->setGeometry(QRect(600, 30, 99, 38));
        widget_3 = new QWidget(scrollAreaWidgetContents);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(20, 150, 711, 71));
        txtVmXConfig = new QLineEdit(widget_3);
        txtVmXConfig->setObjectName(QString::fromUtf8("txtVmXConfig"));
        txtVmXConfig->setGeometry(QRect(10, 30, 481, 36));
        txtVmXConfig->setReadOnly(true);
        lblVMXConfig = new QLabel(widget_3);
        lblVMXConfig->setObjectName(QString::fromUtf8("lblVMXConfig"));
        lblVMXConfig->setGeometry(QRect(10, 10, 141, 22));
        btnVmXConfigFind = new QPushButton(widget_3);
        btnVmXConfigFind->setObjectName(QString::fromUtf8("btnVmXConfigFind"));
        btnVmXConfigFind->setGeometry(QRect(500, 30, 99, 38));
        btnVMXConfigPick = new QPushButton(widget_3);
        btnVMXConfigPick->setObjectName(QString::fromUtf8("btnVMXConfigPick"));
        btnVMXConfigPick->setGeometry(QRect(600, 30, 99, 38));
        widget_4 = new QWidget(scrollAreaWidgetContents);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        widget_4->setGeometry(QRect(20, 220, 711, 71));
        txtNormalXConfig = new QLineEdit(widget_4);
        txtNormalXConfig->setObjectName(QString::fromUtf8("txtNormalXConfig"));
        txtNormalXConfig->setGeometry(QRect(10, 30, 481, 36));
        txtNormalXConfig->setReadOnly(true);
        lblNormalXConfig = new QLabel(widget_4);
        lblNormalXConfig->setObjectName(QString::fromUtf8("lblNormalXConfig"));
        lblNormalXConfig->setGeometry(QRect(10, 10, 141, 22));
        btnNormalXConfigFind = new QPushButton(widget_4);
        btnNormalXConfigFind->setObjectName(QString::fromUtf8("btnNormalXConfigFind"));
        btnNormalXConfigFind->setGeometry(QRect(500, 30, 99, 38));
        btnNormalXConfigPick = new QPushButton(widget_4);
        btnNormalXConfigPick->setObjectName(QString::fromUtf8("btnNormalXConfigPick"));
        btnNormalXConfigPick->setGeometry(QRect(600, 30, 99, 38));
        widget_5 = new QWidget(scrollAreaWidgetContents);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        widget_5->setGeometry(QRect(20, 290, 711, 71));
        txtEvDevKeyboard = new QLineEdit(widget_5);
        txtEvDevKeyboard->setObjectName(QString::fromUtf8("txtEvDevKeyboard"));
        txtEvDevKeyboard->setGeometry(QRect(10, 30, 481, 36));
        lblEvDevKeyboard = new QLabel(widget_5);
        lblEvDevKeyboard->setObjectName(QString::fromUtf8("lblEvDevKeyboard"));
        lblEvDevKeyboard->setGeometry(QRect(10, 10, 131, 22));
        btnEvDevKeyboardFind = new QPushButton(widget_5);
        btnEvDevKeyboardFind->setObjectName(QString::fromUtf8("btnEvDevKeyboardFind"));
        btnEvDevKeyboardFind->setGeometry(QRect(500, 30, 99, 38));
        btnEvDevKeyboardEdit = new QPushButton(widget_5);
        btnEvDevKeyboardEdit->setObjectName(QString::fromUtf8("btnEvDevKeyboardEdit"));
        btnEvDevKeyboardEdit->setGeometry(QRect(600, 30, 99, 38));
        widget_6 = new QWidget(scrollAreaWidgetContents);
        widget_6->setObjectName(QString::fromUtf8("widget_6"));
        widget_6->setGeometry(QRect(20, 360, 711, 71));
        txtPassthroughMouse = new QLineEdit(widget_6);
        txtPassthroughMouse->setObjectName(QString::fromUtf8("txtPassthroughMouse"));
        txtPassthroughMouse->setGeometry(QRect(10, 30, 481, 36));
        lblPassthroughMouse = new QLabel(widget_6);
        lblPassthroughMouse->setObjectName(QString::fromUtf8("lblPassthroughMouse"));
        lblPassthroughMouse->setGeometry(QRect(10, 10, 151, 22));
        btnPassthroughMouseFind = new QPushButton(widget_6);
        btnPassthroughMouseFind->setObjectName(QString::fromUtf8("btnPassthroughMouseFind"));
        btnPassthroughMouseFind->setGeometry(QRect(500, 30, 99, 38));
        btnPassthroughMouseSave = new QPushButton(widget_6);
        btnPassthroughMouseSave->setObjectName(QString::fromUtf8("btnPassthroughMouseSave"));
        btnPassthroughMouseSave->setGeometry(QRect(600, 30, 99, 38));
        widget_2 = new QWidget(scrollAreaWidgetContents);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(20, 80, 711, 71));
        txtUsername = new QLineEdit(widget_2);
        txtUsername->setObjectName(QString::fromUtf8("txtUsername"));
        txtUsername->setGeometry(QRect(10, 30, 481, 36));
        lblUsername = new QLabel(widget_2);
        lblUsername->setObjectName(QString::fromUtf8("lblUsername"));
        lblUsername->setGeometry(QRect(10, 10, 91, 22));
        btnUsernameFind = new QPushButton(widget_2);
        btnUsernameFind->setObjectName(QString::fromUtf8("btnUsernameFind"));
        btnUsernameFind->setGeometry(QRect(500, 30, 99, 38));
        btnUsernameSave = new QPushButton(widget_2);
        btnUsernameSave->setObjectName(QString::fromUtf8("btnUsernameSave"));
        btnUsernameSave->setGeometry(QRect(600, 30, 99, 38));
        scrollArea->setWidget(scrollAreaWidgetContents);
        lblCores = new QLabel(centralwidget);
        lblCores->setObjectName(QString::fromUtf8("lblCores"));
        lblCores->setGeometry(QRect(20, 30, 121, 22));
        lblRam = new QLabel(centralwidget);
        lblRam->setObjectName(QString::fromUtf8("lblRam"));
        lblRam->setGeometry(QRect(150, 30, 141, 22));
        txtCores = new QLineEdit(centralwidget);
        txtCores->setObjectName(QString::fromUtf8("txtCores"));
        txtCores->setGeometry(QRect(20, 50, 113, 36));
        txtRam = new QLineEdit(centralwidget);
        txtRam->setObjectName(QString::fromUtf8("txtRam"));
        txtRam->setGeometry(QRect(150, 50, 113, 36));
        btnEditRamCPU = new QPushButton(centralwidget);
        btnEditRamCPU->setObjectName(QString::fromUtf8("btnEditRamCPU"));
        btnEditRamCPU->setGeometry(QRect(289, 44, 99, 38));
        chkRestartX = new QCheckBox(centralwidget);
        chkRestartX->setObjectName(QString::fromUtf8("chkRestartX"));
        chkRestartX->setGeometry(QRect(492, 47, 41, 31));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(412, 41, 121, 41));
        QFont font;
        font.setPointSize(8);
        textEdit->setFont(font);
        textEdit->setReadOnly(true);
        Main->setCentralWidget(centralwidget);
        textEdit->raise();
        btnMouseToggle->raise();
        btnGo->raise();
        scrollArea->raise();
        lblCores->raise();
        lblRam->raise();
        txtCores->raise();
        txtRam->raise();
        btnEditRamCPU->raise();
        chkRestartX->raise();
        menubar = new QMenuBar(Main);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 756, 34));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuAbout = new QMenu(menubar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        Main->setMenuBar(menubar);
        QWidget::setTabOrder(btnGo, btnMouseToggle);
        QWidget::setTabOrder(btnMouseToggle, txtCores);
        QWidget::setTabOrder(txtCores, txtRam);
        QWidget::setTabOrder(txtRam, btnEditRamCPU);
        QWidget::setTabOrder(btnEditRamCPU, textEdit);
        QWidget::setTabOrder(textEdit, chkRestartX);
        QWidget::setTabOrder(chkRestartX, scrollArea);
        QWidget::setTabOrder(scrollArea, txtVmName);
        QWidget::setTabOrder(txtVmName, btnVmNameFind);
        QWidget::setTabOrder(btnVmNameFind, btnVmNameSave);
        QWidget::setTabOrder(btnVmNameSave, txtUsername);
        QWidget::setTabOrder(txtUsername, btnUsernameFind);
        QWidget::setTabOrder(btnUsernameFind, btnUsernameSave);
        QWidget::setTabOrder(btnUsernameSave, txtVmXConfig);
        QWidget::setTabOrder(txtVmXConfig, btnVmXConfigFind);
        QWidget::setTabOrder(btnVmXConfigFind, btnVMXConfigPick);
        QWidget::setTabOrder(btnVMXConfigPick, txtNormalXConfig);
        QWidget::setTabOrder(txtNormalXConfig, btnNormalXConfigFind);
        QWidget::setTabOrder(btnNormalXConfigFind, btnNormalXConfigPick);
        QWidget::setTabOrder(btnNormalXConfigPick, txtEvDevKeyboard);
        QWidget::setTabOrder(txtEvDevKeyboard, btnEvDevKeyboardFind);
        QWidget::setTabOrder(btnEvDevKeyboardFind, btnEvDevKeyboardEdit);
        QWidget::setTabOrder(btnEvDevKeyboardEdit, txtPassthroughMouse);
        QWidget::setTabOrder(txtPassthroughMouse, btnPassthroughMouseFind);
        QWidget::setTabOrder(btnPassthroughMouseFind, btnPassthroughMouseSave);
        QWidget::setTabOrder(btnPassthroughMouseSave, txtIOMMUGroup);
        QWidget::setTabOrder(txtIOMMUGroup, btnIOMMUGroupFind);
        QWidget::setTabOrder(btnIOMMUGroupFind, btnIOMMUGroupSave);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuAbout->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionReload);
        menuFile->addAction(actionExit);
        menuAbout->addAction(actionHelp);

        retranslateUi(Main);

        QMetaObject::connectSlotsByName(Main);
    } // setupUi

    void retranslateUi(QMainWindow *Main)
    {
        Main->setWindowTitle(QCoreApplication::translate("Main", "GpuPassthroughSetup", nullptr));
        actionExit->setText(QCoreApplication::translate("Main", "Exit", nullptr));
        actionHelp->setText(QCoreApplication::translate("Main", "Help", nullptr));
        actionReload->setText(QCoreApplication::translate("Main", "Reload", nullptr));
        btnMouseToggle->setText(QCoreApplication::translate("Main", "Mouse Toggle", nullptr));
        btnGo->setText(QCoreApplication::translate("Main", "GO : WARNING HAVE EVERYTHING SAVED THIS WILL LOG YOU OUT", nullptr));
        lblVMName->setText(QCoreApplication::translate("Main", "VM Name", nullptr));
        btnVmNameFind->setText(QCoreApplication::translate("Main", "Help Find", nullptr));
        btnVmNameSave->setText(QCoreApplication::translate("Main", "Save", nullptr));
        lblIOMMUGroup->setText(QCoreApplication::translate("Main", "VideoCard IOMMU Group", nullptr));
        btnIOMMUGroupFind->setText(QCoreApplication::translate("Main", "Help Find", nullptr));
        btnIOMMUGroupSave->setText(QCoreApplication::translate("Main", "Save", nullptr));
        lblVMXConfig->setText(QCoreApplication::translate("Main", "VM X Config", nullptr));
        btnVmXConfigFind->setText(QCoreApplication::translate("Main", "Edit Current", nullptr));
        btnVMXConfigPick->setText(QCoreApplication::translate("Main", "Pick", nullptr));
        lblNormalXConfig->setText(QCoreApplication::translate("Main", "Normal X Config", nullptr));
        btnNormalXConfigFind->setText(QCoreApplication::translate("Main", "Edit Current", nullptr));
        btnNormalXConfigPick->setText(QCoreApplication::translate("Main", "Pick", nullptr));
        lblEvDevKeyboard->setText(QCoreApplication::translate("Main", "EvDev Keyboard", nullptr));
        btnEvDevKeyboardFind->setText(QCoreApplication::translate("Main", "Help Find", nullptr));
        btnEvDevKeyboardEdit->setText(QCoreApplication::translate("Main", "Edit", nullptr));
        lblPassthroughMouse->setText(QCoreApplication::translate("Main", "Passthrough Mouse", nullptr));
        btnPassthroughMouseFind->setText(QCoreApplication::translate("Main", "Help Find", nullptr));
        btnPassthroughMouseSave->setText(QCoreApplication::translate("Main", "Save", nullptr));
        lblUsername->setText(QCoreApplication::translate("Main", "Username", nullptr));
        btnUsernameFind->setText(QCoreApplication::translate("Main", "Help Find", nullptr));
        btnUsernameSave->setText(QCoreApplication::translate("Main", "Save", nullptr));
        lblCores->setText(QCoreApplication::translate("Main", "# Cores For VM", nullptr));
        lblRam->setText(QCoreApplication::translate("Main", "Ram In GB for VM", nullptr));
        btnEditRamCPU->setText(QCoreApplication::translate("Main", "Edit cpu/ram", nullptr));
        chkRestartX->setText(QString());
        textEdit->setHtml(QCoreApplication::translate("Main", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Noto Sans'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:6pt;\">Restart X Server</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:6pt;\">On Vm Exit</span></p></body></html>", nullptr));
        menuFile->setTitle(QCoreApplication::translate("Main", "File", nullptr));
        menuAbout->setTitle(QCoreApplication::translate("Main", "About", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Main: public Ui_Main {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_H
