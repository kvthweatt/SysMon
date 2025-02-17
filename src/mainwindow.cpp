#include "mainwindow.h"
#include <QGridLayout>
#include <QFrame>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_cpuMonitor(new CPUMonitor(this))
    , m_ramMonitor(new RAMMonitor(this))
    , m_diskMonitor(new DiskMonitor(this))
    , m_networkMonitor(new NetworkMonitor(this))
    , m_centralWidget(new QWidget(this))
    , m_gridLayout(new QGridLayout(m_centralWidget))
{
    setupUi();
}

QFrame* MainWindow::createPanel(QWidget* widget)
{
    QFrame* panel = new QFrame(this);
    panel->setFrameStyle(QFrame::Panel | QFrame::Raised);
    panel->setLineWidth(2);
    
    QVBoxLayout* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(widget);
    
    // Set a minimum size for the panel
    panel->setMinimumSize(350, 250);
    
    return panel;
}

void MainWindow::setupUi()
{
    // Set window properties
    setWindowTitle(tr("System Monitor"));
    resize(800, 600);

    // Configure central widget
    setCentralWidget(m_centralWidget);

    // Configure grid layout
    m_gridLayout->setSpacing(10);
    m_gridLayout->setContentsMargins(10, 10, 10, 10);

    // Add monitors to grid, each in its own panel
    // First row
    m_gridLayout->addWidget(createPanel(m_cpuMonitor), 0, 0);
    m_gridLayout->addWidget(createPanel(m_ramMonitor), 0, 1);
    
    // Second row
    m_gridLayout->addWidget(createPanel(m_diskMonitor), 1, 0);
    m_gridLayout->addWidget(createPanel(m_networkMonitor), 1, 1);

    // Make all columns stretch equally
    m_gridLayout->setColumnStretch(0, 1);
    m_gridLayout->setColumnStretch(1, 1);
}
