#include "diskmonitor.h"

DiskMonitor::DiskMonitor(QWidget* parent)
    : MonitorWidget(tr("Disk Usage"), parent)
    , m_rootStorage(QStorageInfo::root())
{
    // Configure y-axis for percentage display
    if (auto axisY = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Vertical).first())) {
        axisY->setTitleText("%");
    }
}

void DiskMonitor::updateData()
{
    DiskInfo info;
    if (!getDiskInfo(info)) {
        setValueText(tr("Error reading disk info"));
        return;
    }

    QString displayText = QString("%1 / %2 (%3%)")
        .arg(formatSize(info.used))
        .arg(formatSize(info.total))
        .arg(QString::number(info.percentage, 'f', 1));

    setValueText(displayText);
    addDataPoint(info.percentage);
}

bool DiskMonitor::getDiskInfo(DiskInfo& info)
{
    m_rootStorage.refresh();

    if (!m_rootStorage.isValid() || !m_rootStorage.isReady()) {
        return false;
    }

    info.total = m_rootStorage.bytesTotal();
    info.available = m_rootStorage.bytesAvailable();
    
    if (info.total == 0) {
        return false;
    }

    info.used = info.total - info.available;
    info.percentage = (static_cast<double>(info.used) / info.total) * 100.0;

    return true;
}

QString DiskMonitor::formatSize(quint64 bytes)
{
    const double tb = bytes / (1024.0 * 1024.0 * 1024.0 * 1024.0);
    if (tb >= 1.0) {
        return QString::number(tb, 'f', 2) + " TB";
    }

    const double gb = bytes / (1024.0 * 1024.0 * 1024.0);
    if (gb >= 1.0) {
        return QString::number(gb, 'f', 2) + " GB";
    }

    const double mb = bytes / (1024.0 * 1024.0);
    if (mb >= 1.0) {
        return QString::number(mb, 'f', 2) + " MB";
    }

    const double kb = bytes / 1024.0;
    return QString::number(kb, 'f', 2) + " KB";
}
