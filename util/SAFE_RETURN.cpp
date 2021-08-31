
#include "SAFE_RETURN.h"

SAFE_RETURN::SAFE_RETURN()
{
    this->StatusCode = OK;
}

RETURN_TYPE::RETURN_TYPE(const char *typeName, QObject *ObjectData)
{
    this->typeId = QMetaType::type(typeName);
    this->objectData = ObjectData;
}

RETURN_TYPE* RETURN_TYPE::fromQString(QString string)
{
    return new RETURN_TYPE("QString", (QObject*)new QString(string));
}

RETURN_TYPE* RETURN_TYPE::fromString(const char* string)
{
    return new RETURN_TYPE("QString", (QObject*)new QString(string));
}

SAFE_RETURN::~SAFE_RETURN()
{
    Q_FOREACH(QString key, this->returnData.keys()) {
        QObject *obj = this->returnData[key]->objectData;
        delete obj;
    }
    this->returnData.clear();
}


