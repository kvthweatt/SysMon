#ifndef NETWORKMONITOR_H
#define NETWORKMONITOR_H

#include "../widgets/monitorwidget.h"
#include <QFile>
#include <QNetworkInterface>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

class NetworkMonitor : public MonitorWidget
{
    Q_OBJECT

public:
    explicit NetworkMonitor(QWidget* parent = nullptr);
    ~NetworkMonitor() override = default;

protected:
    void updateData() override;

private:
    static const QString PROC_NET_DEV_PATH;
    
    struct NetworkStats {
        quint64 bytesReceived{0};
        quint64 bytesSent{0};
        double receivedSpeed{0.0};  // bytes per second
        double sentSpeed{0.0};      // bytes per second
    };

    QString m_primaryInterface;
    NetworkStats m_prevStats;
    qint64 m_lastUpdate{0};

    bool readNetworkStats(NetworkStats& stats);
    QString formatSpeed(double bytesPerSecond);
    void findPrimaryInterface();
};

#endif // NETWORKMONITOR_H
