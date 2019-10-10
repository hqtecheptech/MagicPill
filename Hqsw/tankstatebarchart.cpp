#include "tankstatebarchart.h"
#include "ui_tankstatebarchart.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QPixmap>
#include <QStandardItemModel>
#include <QModelIndex>

#include "centerwidget.h"
#include "tanghe_centerwidget.h"

QT_CHARTS_USE_NAMESPACE

TankStateBarChart::TankStateBarChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TankStateBarChart)
{
    ui->setupUi(this);
    barBgImg.load("://image/old/barBg.png");

    testConnectionTcpClient = new TcpClientSocket(this);
    connect(testConnectionTcpClient, SIGNAL(updateConnectState(bool)), this, SLOT(localServerConnected(bool)));
    getTotalRunTimeTcpClient = new TcpClientSocket(this);
    connect(getTotalRunTimeTcpClient,SIGNAL(updateClients(QByteArray)),this,SLOT(showRunTimeData(QByteArray)));

    testRunCode();

    myTimerThread = new MyTimerThread(3600, this);
    connect(myTimerThread, SIGNAL(timeout()),this, SLOT(requestRunTimeData()));
}

void TankStateBarChart::testRunCode()
{
    QFile dataFile("ferStatedDatafile.txt");
    /*if(!dataFile.open(QIODevice::WriteOnly))
    {

    }*/
    if(!dataFile.open(QIODevice::ReadOnly))
    {

    }
    QTextStream in(&dataFile);

    QVector<uint> values;
    QVector<qreal> totalDays;

    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList fields = line.split(' ');
        uint value = fields.takeFirst().toUInt();
        qreal days = fields.takeFirst().toFloat();
        values.append(value);
        totalDays.append(days);
    }

    setData(totalDays);
}

void TankStateBarChart::showEvent(QShowEvent *event)
{
    //QTimer::singleShot(10000, this, SLOT(requestRunTimeData()));
    //myTimerThread->start();
}

void TankStateBarChart::showRunTimeData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QString msg;
    if(bDevice.bErrorCode==1)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice));
        QVector<uint> values;
        QVector<qreal> totalDays;

        for(quint16 i=0; i<Global::ferDeviceInfo.Device_Number; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            uint temp = 0;
            memcpy(&temp,value,4);
            values.append(temp);
            qreal rTotalRunTime = (qreal)temp;
            qreal days = rTotalRunTime / (qreal)86400;
            totalDays.append(days);
        }

        // TestCode
        /*if(!dataFileSaved)
        {
            QFile dataFile("ferStatedDatafile.txt");
            if(!dataFile.open(QIODevice::WriteOnly))
            {

            }
            QTextStream out(&dataFile);


            for(int row = 0; row < values.length(); row++)
            {
                out << values[row] << " " << totalDays[row] << endl;
            }

            dataFileSaved = true;
        }*/
        // TestCode

        setData(totalDays);
    }
}

void TankStateBarChart::requestRunTimeData()
{
    testConnectionTcpClient->sendTestConnectRequest();
}

void TankStateBarChart::localServerConnected(bool isConnected)
{
    if(isConnected)
    {
        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(0);
        ushort address = Global::getFermenationNodeInfoByName("FER_TOT_UDI").Offset +
                (info.offset + 0 - info.startIndex) * 4;
        StreamPack bpack = {sizeof(StreamPack),1,0,r_RealData,UInt,address,0,(quint16)Global::ferDeviceInfo.Device_Number,0,0,0};
        getTotalRunTimeTcpClient->sendRequest(bpack);
    }
}

void TankStateBarChart::setData(QVector<qreal> values)
{
    ui->chartLayout->removeWidget(chartView);

    if(oldChart != Q_NULLPTR)
    {
        oldChart->deleteLater();
        chartView->deleteLater();
        oldChart = Q_NULLPTR;
    }

    QStringList categoriesX;
    QLineSeries *lineseries = new QLineSeries();
    QBarSet *barSet = new QBarSet("发酵时间");
    barSet->setColor(QColor(0,200,0,255));
    //barSet->setBrush(QBrush(barBgImg.scaled(size(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    //![1]
    for(int i=0; i<Global::ferDeviceInfo.Device_Number; i++)
    {
        *barSet << values[i];
        categoriesX << QString::number(i+1);
        lineseries->append(QPoint(i, values[i]));
    }
    //![1]

    //![2]
    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet);
    //![2]

    //![3]
    QChart *chart = new QChart();
    oldChart = chart;
    chart->setBackgroundBrush(QBrush(QColor(240,240,240)));
    chart->addSeries(barSeries);
    //chart->setTitle("发酵时间");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    //![3]

    //![4]
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categoriesX);
    axisX->setGridLineVisible(false);
    chart->setAxisX(axisX, barSeries);
    chart->setAxisX(axisX,lineseries);
    //![4]

    //![4]
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 25);
    //axisY->setTitleText("Y Axis");
    axisY->setLabelFormat("%d/d");
    axisY->setGridLineVisible(true);
    axisY->setTickCount(6);
    //axisY->setMinorTickCount(4);
    chart->setAxisY(axisY,barSeries);
    chart->setAxisY(axisY,lineseries);
    //![4]

    //![5]
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);
    //![5]

    //![6]
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    //![6]

    ui->chartLayout->addWidget(chartView);
}

TankStateBarChart::~TankStateBarChart()
{
    delete ui;
}
