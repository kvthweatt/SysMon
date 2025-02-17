#include "networkmonitor.h"
#include <QDateTime>
#include <QTextStream>

const QString NetworkMonitor::PROC_NET_DEV_PATH = "/proc/net/dev";

NetworkMonitor::NetworkMonitor(QWidget* parent)
    : MonitorWidget(tr("Network Usage"), parent)
{
    // Configure y-axis for speed display
    if (auto axisY = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Vertical).first())) {
        axisY->setTitleText("MB/s");
        axisY->setRange(0, 10);  // Initial range, will auto-adjust based on traffic
    }

    findPrimaryInterface();
    m_lastUpdate = QDateTime::currentMSecsSinceEpoch();
}

void NetworkMonitor::findPrimaryInterface()
{
    // Find the first active non-loopback interface
    for (const QNetworkInterface& interface : QNetworkInterface::allInterfaces()) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            m_primaryInterface = interface.name();
            break;
        }
    }

    // Fallback to 'eth0' if no suitable interface found
    if (m_primaryInterface.isEmpty()) {
        m_primaryInterface = "eth0";
    }
}

void NetworkMonitor::updateData()
{
    NetworkStats currentStats;
    if (!readNetworkStats(currentStats)) {
        setValueText(tr("Error reading network stats"));
        return;
    }

    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    double timeElapsed = (currentTime - m_lastUpdate) / 1000.0; // Convert to seconds

    if (timeElapsed > 0 && m_prevStats.bytesReceived > 0) {
        // Calculate speeds
        currentStats.receivedSpeed = (currentStats.bytesReceived - m_prevStats.bytesReceived) / timeElapsed;
        currentStats.sentSpeed = (currentStats.bytesSent - m_prevStats.bytesSent) / timeElapsed;

        // Update display
        QString displayText = tr("↓ %1 ↑ %2")
            .arg(formatSpeed(currentStats.receivedSpeed))
            .arg(formatSpeed(currentStats.sentSpeed));
        setValueText(displayText);

        // Add total speed to chart (received + sent)
        double totalMBps = (currentStats.receivedSpeed + currentStats.sentSpeed) / (1024.0 * 1024.0);
        addDataPoint(totalMBps);

        // Adjust y-axis range if needed
        if (auto axisY = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Vertical).first())) {
            if (totalMBps > axisY->max()) {
                axisY->setRange(0, totalMBps * 1.2); // Add 20% headroom
            }
        }
    }

    // Update previous values for next calculation
    m_prevStats = currentStats;
    m_lastUpdate = currentTime;
}

bool NetworkMonitor::readNetworkStats(NetworkStats& stats)
{
    QFile file(PROC_NET_DEV_PATH);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    QString line;

    // Skip header lines
    in.readLine();
    in.readLine();

    // Find and parse the line for our interface
    while (!(line = in.readLine()).isNull()) {
        if (line.contains(m_primaryInterface)) {
            QStringList fields = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
            if (fields.size() >= 10) {
                stats.bytesReceived = fields[1].toULongLong(); // Received bytes
                stats.bytesSent = fields[9].toULongLong();     // Transmitted bytes
                file.close();
                return true;
            }
        }
    }

    file.close();
    return false;
}

QString NetworkMonitor::formatSpeed(double bytesPerSecond)
{
    const double gbps = bytesPerSecond / (1024.0 * 1024.0 * 1024.0);
    if (gbps >= 1.0) {
        return QString::number(gbps, 'f', 2) + " GB/s";
    }

    const double mbps = bytesPerSecond / (1024.0 * 1024.0);
    if (mbps >= 1.0) {
        return QString::number(mbps, 'f', 2) + " MB/s";
    }

    const double kbps = bytesPerSecond / 1024.0;
    if (kbps >= 1.0) {
        return QString::number(kbps, 'f', 2) + " KB/s";
    }

    return QString::number(bytesPerSecond, 'f', 0) + " B/s";
}
