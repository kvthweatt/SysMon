#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "monitors/cpumonitor.h"
#include "monitors/rammonitor.h"
#include "monitors/diskmonitor.h"
#include "monitors/networkmonitor.h"

class QGridLayout;
class QWidget;
class QFrame;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

private:
    void setupUi();
    QFrame* createPanel(QWidget* widget);

    // Monitor widgets
    CPUMonitor* m_cpuMonitor;
    RAMMonitor* m_ramMonitor;
    DiskMonitor* m_diskMonitor;
    NetworkMonitor* m_networkMonitor;

    // Layout widgets
    QWidget* m_centralWidget;
    QGridLayout* m_gridLayout;
};

#endif // MAINWINDOW_H
