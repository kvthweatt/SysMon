#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application metadata
    QApplication::setApplicationName("System Monitor");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("SystemMonitor");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
