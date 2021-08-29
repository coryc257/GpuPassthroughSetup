#include "SAFE_RETURN.h"
#include "HANDLE_UI_RETURN.h"
#include "MsgBox.h"

void HANDLE_UI_RETURN::HANDLE(SAFE_RETURN* status)
{
    QString output;
    output.reserve(4096);
    output.append(QStringLiteral("The Operation Failed: ") + status->Message + QStringLiteral("\n\n"));

    Q_FOREACH(QString x, status->returnData.keys()) {
        if (status->returnData[x]->typeId == QMetaType::type("QString")) {
            output.append(QStringLiteral("  "));
            output.append(x);
            output.append(QStringLiteral(":"));
            output.append(*((QString*)(status->returnData[x]->objectData)));
        }
    }
    MsgBox(output);
}

