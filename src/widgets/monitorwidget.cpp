#include "monitorwidget.h"
#include <QMouseEvent>
#include <QMainWindow>
#include <QtCharts/QValueAxis>

MonitorWidget::MonitorWidget(const QString& title, QWidget* parent)
    : QWidget(parent)
    , m_mainLayout(new QVBoxLayout(this))
    , m_titleLabel(new QLabel(title, this))
    , m_valueLabel(new QLabel(this))
    , m_chart(new QChart())
    , m_chartView(new QChartView(m_chart, this))
    , m_series(new QLineSeries())
    , m_updateTimer(new QTimer(this))
    , m_isExpanded(false)
{
    // Configure title label
    QFont titleFont = m_titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(12);
    m_titleLabel->setFont(titleFont);

    // Configure value label
    m_valueLabel->setAlignment(Qt::AlignCenter);
    QFont valueFont = m_valueLabel->font();
    valueFont.setPointSize(11);
    m_valueLabel->setFont(valueFont);

    // Configure chart
    m_chart->addSeries(m_series);
    m_chart->createDefaultAxes();
    m_chart->legend()->hide();
    m_chart->setTitle("");
    m_chart->setMargins(QMargins(0, 0, 0, 0));
    
    // Configure Y axis
    QValueAxis* axisY = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Vertical).first());
    if (axisY) {
        axisY->setRange(0, 100);
        axisY->setTickCount(6);
        axisY->setLabelFormat("%.0f");
    }

    // Configure X axis
    QValueAxis* axisX = qobject_cast<QValueAxis*>(m_chart->axes(Qt::Horizontal).first());
    if (axisX) {
        axisX->setRange(0, MAX_DATA_POINTS);
        axisX->setTickCount(7);
        axisX->setLabelFormat("%.0f");
    }

    // Configure chart view
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setMinimumHeight(150);
    m_chartView->installEventFilter(this);

    // Set up layout
    m_mainLayout->addWidget(m_titleLabel);
    m_mainLayout->addWidget(m_valueLabel);
    m_mainLayout->addWidget(m_chartView);
    m_mainLayout->setStretchFactor(m_chartView, 1);

    // Configure and start timer
    m_updateTimer->setInterval(UPDATE_INTERVAL);
    connect(m_updateTimer, &QTimer::timeout, this, &MonitorWidget::onUpdateTimeout);
    m_updateTimer->start();

    // Initialize series with zero values
    for (int i = 0; i < MAX_DATA_POINTS; ++i) {
        m_series->append(i, 0);
    }
}

MonitorWidget::~MonitorWidget()
{
    m_updateTimer->stop();
    delete m_chart;
}

bool MonitorWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == m_chartView && event->type() == QEvent::MouseButtonDblClick) {
        toggleExpanded();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void MonitorWidget::toggleExpanded()
{
    QMainWindow* mainWindow = qobject_cast<QMainWindow*>(window());
    if (!mainWindow) return;

    QWidget* centralWidget = mainWindow->centralWidget();
    if (!centralWidget) return;

    QGridLayout* gridLayout = qobject_cast<QGridLayout*>(centralWidget->layout());
    if (!gridLayout) return;

    if (!m_isExpanded) {
        // Save our parent panel's position in the grid
        m_normalRow = gridLayout->indexOf(parentWidget()) / gridLayout->columnCount();
        m_normalColumn = gridLayout->indexOf(parentWidget()) % gridLayout->columnCount();

        // Temporarily remove our parent panel from the grid
        gridLayout->removeWidget(parentWidget());

        // Add it back spanning all rows and columns
        gridLayout->addWidget(parentWidget(), 0, 0, 2, 2);

        // Hide other panels
        for (int i = 0; i < gridLayout->count(); ++i) {
            QWidget* widget = gridLayout->itemAt(i)->widget();
            if (widget && widget != parentWidget()) {
                widget->hide();
            }
        }
    } else {
        // Remove our parent panel from the full-size position
        gridLayout->removeWidget(parentWidget());

        // Restore it to its original position
        gridLayout->addWidget(parentWidget(), m_normalRow, m_normalColumn);

        // Show all panels
        for (int i = 0; i < gridLayout->count(); ++i) {
            QWidget* widget = gridLayout->itemAt(i)->widget();
            if (widget) {
                widget->show();
            }
        }
    }

    m_isExpanded = !m_isExpanded;
}

void MonitorWidget::addDataPoint(qreal value)
{
    // Remove first point and shift all points left
    QList<QPointF> points = m_series->points();
    points.removeFirst();
    
    // Add new point at the end
    points.append(QPointF(MAX_DATA_POINTS - 1, value));
    
    // Update X coordinates
    for (int i = 0; i < points.size(); ++i) {
        points[i].setX(i);
    }
    
    // Replace all points in the series
    m_series->replace(points);
}

void MonitorWidget::setValueText(const QString& text)
{
    m_valueLabel->setText(text);
}

void MonitorWidget::onUpdateTimeout()
{
    updateData();
}
