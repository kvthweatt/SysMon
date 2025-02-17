#include "rammonitor.h"
#include <QTextStream>

const QString RAMMonitor::PROC_MEMINFO_PATH = "/proc/meminfo";

RAMMonitor::RAMMonitor(QWidget* parent)
    : MonitorWidget(tr("RAM Usage"), parent)
{
    // Configure y-axis for percentage display
    if (auto axisY = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Vertical).first())) {
        axisY->setTitleText("%");
    }
}

void RAMMonitor::updateData()
{
    MemInfo info;
    if (!readMemInfo(info)) {
        setValueText(tr("Error reading memory info"));
        return;
    }

    QString displayText = QString("%1 / %2 (%3%)")
        .arg(formatMemorySize(info.used))
        .arg(formatMemorySize(info.total))
        .arg(QString::number(info.percentage, 'f', 1));

    setValueText(displayText);
    addDataPoint(info.percentage);
}

bool RAMMonitor::readMemInfo(MemInfo& info)
{
    QFile file(PROC_MEMINFO_PATH);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    QString line;
    QMap<QString, quint64> memValues;

    // Read memory information
    while (!(line = in.readLine()).isNull()) {
        QStringList parts = line.split(":", Qt::SkipEmptyParts);
        if (parts.size() != 2) continue;

        QString key = parts[0].trimmed();
        // Convert kB to bytes and remove " kB" suffix
        quint64 value = parts[1].trimmed().split(" ")[0].toULongLong() * 1024;
        memValues[key] = value;
    }

    file.close();

    // Get required values
    info.total = memValues["MemTotal"];
    info.available = memValues["MemAvailable"];
    
    if (info.total == 0) {
        return false;
    }

    // Calculate used memory and percentage
    info.used = info.total - info.available;
    info.percentage = (static_cast<double>(info.used) / info.total) * 100.0;

    return true;
}

QString RAMMonitor::formatMemorySize(quint64 bytes)
{
    const double gb = bytes / (1024.0 * 1024.0 * 1024.0);
    if (gb >= 1.0) {
        return QString::number(gb, 'f', 1) + " GB";
    }

    const double mb = bytes / (1024.0 * 1024.0);
    if (mb >= 1.0) {
        return QString::number(mb, 'f', 1) + " MB";
    }

    const double kb = bytes / 1024.0;
    return QString::number(kb, 'f', 1) + " KB";
}
