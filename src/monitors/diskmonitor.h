#ifndef DISKMONITOR_H
#define DISKMONITOR_H

#include "../widgets/monitorwidget.h"
#include <QStorageInfo>
#include <QtCharts/QValueAxis>

class DiskMonitor : public MonitorWidget
{
    Q_OBJECT

public:
    explicit DiskMonitor(QWidget* parent = nullptr);
    ~DiskMonitor() override = default;

protected:
    void updateData() override;

private:
    QStorageInfo m_rootStorage;
    
    struct DiskInfo {
        quint64 total{0};
        quint64 used{0};
        quint64 available{0};
        double percentage{0.0};
    };

    bool getDiskInfo(DiskInfo& info);
    QString formatSize(quint64 bytes);
};

#endif // DISKMONITOR_H
