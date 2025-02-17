#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include "../widgets/monitorwidget.h"
#include <QFile>
#include <QtCharts/QValueAxis>

class CPUMonitor : public MonitorWidget
{
    Q_OBJECT

public:
    explicit CPUMonitor(QWidget* parent = nullptr);
    ~CPUMonitor() override = default;

protected:
    void updateData() override;

private:
    static const QString PROC_STAT_PATH;
    
    // Previous CPU time values for delta calculation
    quint64 m_prevIdle{0};
    quint64 m_prevTotal{0};
    
    // Helper methods
    bool readCPUStats(quint64& idle, quint64& total);
    double calculateCPUPercentage(quint64 idle, quint64 total);
};

#endif // CPUMONITOR_H
