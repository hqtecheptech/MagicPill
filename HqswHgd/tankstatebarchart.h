#ifndef TANKSTATEBARCHART_H
#define TANKSTATEBARCHART_H

#include <QWidget>
#include "mytimerthread.h"
#include "tcpclientsocket.h"

#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class TankStateBarChart;
}

class TankStateBarChart : public QWidget
{
    Q_OBJECT

public:
    explicit TankStateBarChart(QWidget *parent = 0);
    ~TankStateBarChart();

private:
    Ui::TankStateBarChart *ui;
    QPixmap barBgImg;
    MyTimerThread *myTimerThread;
    TcpClientSocket *testConnectionTcpClient;
    TcpClientSocket *getTotalRunTimeTcpClient;

    QChart *oldChart = Q_NULLPTR;
    QChartView *chartView;

    bool dataFileSaved = false;

protected:
    void showEvent(QShowEvent *event);

private slots:
    void showRunTimeData(QByteArray data);
    void requestRunTimeData();
    void localServerConnected(bool);

    void testRunCode();

public:
    void setData(QVector<qreal> values);
};

#endif // TANKSTATEBARCHART_H
