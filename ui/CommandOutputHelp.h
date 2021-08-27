#ifndef COMMOUTPUTHELP_H
#define COMMOUTPUTHELP_H

#include <QDialog>
#include <QScopedPointer>
#include <QAction>
#include <QResizeEvent>

namespace Ui {
class CommandOutputHelp;
}

class CommandOutputHelp : public QDialog
{
    Q_OBJECT

public:
    explicit CommandOutputHelp(QWidget *parent = nullptr);
    ~CommandOutputHelp() override;

    QString getSelection();
    void setDetails(QString output, QString helpText);
    void setDefault(QString defaultValue);

private:
    QScopedPointer<Ui::CommandOutputHelp> m_ui;

private slots:


};

#endif
