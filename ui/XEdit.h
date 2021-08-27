#ifndef XEDIT_H
#define XEDIT_H

#include <QDialog>
#include <QScopedPointer>
#include <QAction>
#include <QResizeEvent>

namespace Ui {
class XEdit;
}

class XEdit : public QDialog
{
    Q_OBJECT

public:
    explicit XEdit(QWidget *parent = nullptr);
    ~XEdit() override;

    QString getXConfig();
    void setDetails(QString output, QString helpText);

private:
    QScopedPointer<Ui::XEdit> m_ui;

private slots:


};

#endif
