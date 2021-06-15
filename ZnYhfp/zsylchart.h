#ifndef ZSYLCHART_H
#define ZSYLCHART_H

#include <QWidget>

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QDateTimeAxis>

#include "data.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class ZsylChart;
}

class ZsylChart : public QWidget
{
    Q_OBJECT

public:
    explicit ZsylChart(QWidget *parent = 0);
    ~ZsylChart();

    void setRange(int r1, int r2);

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void showRealTime(QByteArray);
    void showAllData();
    void resetChart();

public slots:
    void updateUI(int rsValue, int prsValue);

private:
    Ui::ZsylChart *ui;

    QChartView *chartView;
    QChart *chart;
    QLineSeries *rsSeries;
    QLineSeries *prsSeries;
    QValueAxis *axisY;
    QValueAxis *axisX;
    bool resetChartSuccess;
    int serieValuesCount = 0;
    // History one page values
    int rsValues[CP];
    int prsValues[CP];

    int range1, range2;
};

#endif // ZSYLCHART_H
