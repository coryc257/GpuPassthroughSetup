#ifndef GPUMAIN_H
#define GPUMAIN_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QAction>
#include <QResizeEvent>

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
    void resizeEvent(QResizeEvent *e) override;
    int originalHeight;
    int scrollerOriginalHeight;

private slots:
    void CloseMe();
    void Help();
    void Go();

    // Finders
    void finderVmName();

    // Savers
    void saveVmName();

};

#endif