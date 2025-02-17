#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QTimer>

QT_CHARTS_USE_NAMESPACE

class MonitorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorWidget(const QString& title, QWidget* parent = nullptr);
    virtual ~MonitorWidget();

    // Pure virtual function that derived classes must implement
    virtual void updateData() = 0;

protected:
    // UI elements that derived classes will use
    QVBoxLayout* m_mainLayout;
    QLabel* m_titleLabel;
    QLabel* m_valueLabel;
    QChart* m_chart;
    QChartView* m_chartView;
    QLineSeries* m_series;
    
    // Timer for periodic updates
    QTimer* m_updateTimer;
    
    // Configuration
    static const int MAX_DATA_POINTS = 60;  // 1 minute of data at 1 second intervals
    static const int UPDATE_INTERVAL = 1000; // 1 second in milliseconds

    // Helper functions for derived classes
    void addDataPoint(qreal value);
    void setValueText(const QString& text);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void onUpdateTimeout();
    void toggleExpanded();

private:
    int m_normalRow;
    int m_normalColumn;
    bool m_isExpanded;
    QRect m_normalGeometry;
};

#endif // MONITORWIDGET_H
