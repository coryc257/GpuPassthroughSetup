#ifndef SAFE_RETURN_H
#define SAFE_RETURN_H
#include <QString>
#include <QBuffer>
#include <QMap>
#include <QObject>

#define IS_OK(x) (x.StatusCode == SAFE_RETURN_CODES::OK)

enum SAFE_RETURN_CODES
{
    OK,
    FILE_COPY_ERROR,
    FILE_DELETE_ERROR,
    FILE_WRITE_ERROR
};

class RETURN_TYPE
{
public:
    int typeId;
    QObject *objectData;
    RETURN_TYPE(const char *typeName, QObject *ObjectData);

    static RETURN_TYPE* fromString(const char *string);
    static RETURN_TYPE* fromQString(QString string);
};

class SAFE_RETURN
{
public:
    bool Status;
    SAFE_RETURN_CODES StatusCode;
    QString Message;
    QMap<QString, RETURN_TYPE*> returnData;

    SAFE_RETURN();
    ~SAFE_RETURN();
};
#endif
