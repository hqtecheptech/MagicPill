#ifndef REALTIMEDIALOG_H
#define REALTIMEDIALOG_H

#include <QDialog>
#include <QDate>
#include "tcpclientsocket.h"
#include "mytimerthread.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QDateTimeAxis>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class RealTimeDialog;
}

class RealTimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RealTimeDialog(QWidget *parent = 0);
    ~RealTimeDialog();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void showRealTime(QByteArray);
    void getRealTimeData();
    void showAllData(QByteArray);
    void getAllData();
    void resetChart();

    void on_tankComboBox_currentIndexChanged(int index);

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::RealTimeDialog *ui;
    QChartView *chartView;
    QChart *chart;
    QLineSeries *series = new QLineSeries();
    QValueAxis *axisY = new QValueAxis;
    QDateTimeAxis *axisX = new QDateTimeAxis;
    MyTimerThread *myTimerThread;
    TcpClientSocket *getAllDataTcpClient;
    TcpClientSocket *getRealTimeDataTcpClient;
    bool resetChartSuccess;
    QDate currentDate;

};

#endif // REALTIMEDIALOG_H
