#include "MsgBox.h"

void MsgBox(QString msg)
{
    QMessageBox xmsg;
    xmsg.setWindowTitle(QStringLiteral("Notification"));
    xmsg.setText(msg);
    xmsg.exec();
}
