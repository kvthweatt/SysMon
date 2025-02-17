#ifndef RAMMONITOR_H
#define RAMMONITOR_H

#include "../widgets/monitorwidget.h"
#include <QFile>
#include <QtCharts/QValueAxis>

class RAMMonitor : public MonitorWidget
{
    Q_OBJECT

public:
    explicit RAMMonitor(QWidget* parent = nullptr);
    ~RAMMonitor() override = default;

protected:
    void updateData() override;

private:
    static const QString PROC_MEMINFO_PATH;
    
    struct MemInfo {
        quint64 total{0};
        quint64 available{0};
        quint64 used{0};
        double percentage{0.0};
    };

    bool readMemInfo(MemInfo& info);
    QString formatMemorySize(quint64 bytes);
};

#endif // RAMMONITOR_H
