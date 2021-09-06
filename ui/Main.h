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
    bool mouseState;


private slots:
    /*
     * Close application
     */
    void CloseMe();

    /*
     * Display help information
     */
    void Help();

    /*
     * Start the VM
     */
    void Go();

    // Finders
    /*
     * This will list all the vms virt-manager knows about so that the
     * rest of the code will know how what vm to use/start
     */
    void finderVmName();

    /*
     * This will say you are silly for needing help with this
     */
    void finderUserName();

    /*
     * This will help you set up your vm xorg.conf to use when in the vm
     * in the event you have multiple gpus and want to be able to use linux
     * side by side with windows
     */
    void finderVmXConfig();

    /*
     * This will help you set up your non vm xorg.conf for reverting
     */
    void finderNormalXConfig();

    /*
     * This will use a pipeline of commands to find the event
     * files that might be your keyboard that will be used
     * to generate qemu command xml to put in your vm
     * config
     */
    void finderEvDevKeyboard();

    /*
     * This will use lsusb to the find mouse to create an xml
     * file that will be used by the toggle button to send
     * or take the mouse fromt he vm */
    void finderPassthroughMouse();

    /*
     * This will help find your videocard to pass trough
     * It will show the IOMMU groups */
    void findIOMMU();

    // Savers
    void saveVmName();
    void saveUsername();
    void saveEvDevKeyboard();
    void savePassthroughMouse();
    void saveCpuRam();
    void saveIOMMU();

    // Pickers
    void saveVmXConfigChooser();
    void saveNormalXConfigChooser();

    // Toggles
    void toggleMouse();






};

#endif
