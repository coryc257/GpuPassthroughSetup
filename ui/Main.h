#ifndef GPUMAIN_H
#define GPUMAIN_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QAction>

namespace Ui {
class Main;
}

class Main : public QMainWindow
{
    Q_OBJECT

public:
    explicit Main(QWidget *parent = nullptr);
    ~Main() override;

    QAction actionExit;

private:
    QScopedPointer<Ui::Main> m_ui;

private slots:
    void CloseMe();
    void Help();
    void Go();
};

#endif
