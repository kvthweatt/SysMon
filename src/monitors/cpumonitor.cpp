#include "cpumonitor.h"
#include <QTextStream>

const QString CPUMonitor::PROC_STAT_PATH = "/proc/stat";

CPUMonitor::CPUMonitor(QWidget* parent)
    : MonitorWidget(tr("CPU Usage"), parent)
{
    // Configure y-axis for percentage display
    if (auto axisY = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Vertical).first())) {
        axisY->setTitleText("%");
    }
}

void CPUMonitor::updateData()
{
    quint64 idle, total;
    if (!readCPUStats(idle, total)) {
        setValueText(tr("Error reading CPU stats"));
        return;
    }

    double cpuPercentage = calculateCPUPercentage(idle, total);
    
    // Update previous values for next calculation
    m_prevIdle = idle;
    m_prevTotal = total;

    // Update display
    setValueText(QString::number(cpuPercentage, 'f', 1) + "%");
    addDataPoint(cpuPercentage);
}

bool CPUMonitor::readCPUStats(quint64& idle, quint64& total)
{
    QFile file(PROC_STAT_PATH);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    file.close();

    if (!line.startsWith("cpu ")) {
        return false;
    }

    // Split the line into values
    QStringList values = line.split(" ", Qt::SkipEmptyParts);
    if (values.size() < 8) {
        return false;
    }

    // CPU times are in USER_HZ (usually 100Hz)
    quint64 user = values[1].toULongLong();      // Normal processes in user mode
    quint64 nice = values[2].toULongLong();      // Niced processes in user mode
    quint64 system = values[3].toULongLong();    // Processes in kernel mode
    idle = values[4].toULongLong();              // Idle time
    quint64 iowait = values[5].toULongLong();    // I/O wait time
    quint64 irq = values[6].toULongLong();       // Hardware IRQ time
    quint64 softirq = values[7].toULongLong();   // Software IRQ time
    quint64 steal = values[8].toULongLong();     // Stolen time by other VMs

    // Calculate total CPU time
    total = user + nice + system + idle + iowait + irq + softirq + steal;

    return true;
}

double CPUMonitor::calculateCPUPercentage(quint64 idle, quint64 total)
{
    if (m_prevTotal == 0) {
        return 0.0;  // First reading
    }

    // Calculate deltas
    quint64 totalDelta = total - m_prevTotal;
    quint64 idleDelta = idle - m_prevIdle;

    if (totalDelta == 0) {
        return 0.0;  // Avoid division by zero
    }

    // Calculate CPU usage percentage
    return 100.0 * (1.0 - static_cast<double>(idleDelta) / totalDelta);
}
