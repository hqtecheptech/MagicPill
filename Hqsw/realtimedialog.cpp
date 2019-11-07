#include "realtimedialog.h"
#include "ui_realtimedialog.h"
#include "dataformat.h"
#include "requestworkthread.h"
#include "global.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QDateTimeAxis>

QT_CHARTS_USE_NAMESPACE

RealTimeDialog::RealTimeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RealTimeDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("实时曲线"));

    chart = new QChart;
    chart->setTitle(ui->comboBox->currentText());
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
    chartView = new QChartView(this);
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->horizontalLayout->addWidget(chartView);

    series = new QLineSeries();
    series->setPen(QPen(Qt::blue,1,Qt::SolidLine));

    getAllDataTcpClient = new TcpClientSocket(this);
    connect(getAllDataTcpClient,SIGNAL(updateClients(QByteArray)),this,SLOT(showAllData(QByteArray)));

    myTimerThread = new MyTimerThread(10, this);
    connect(myTimerThread, SIGNAL(timeout()),this,SLOT(getRealTimeData()));

    getRealTimeDataTcpClient = new TcpClientSocket(this);
    connect(getRealTimeDataTcpClient,SIGNAL(updateClients(QByteArray)),this,SLOT(showRealTime(QByteArray)));

    QVector<DeviceNode> nodes = Global::ferDeviceNodes;
    foreach (DeviceNode node, nodes) {
        if(node.DataType == "r" && node.Name.contains("FER"))
        {
            ui->comboBox->addItem(node.Cname);
        }
    }

    for(int i=0; i < Global::ferDeviceInfo.Device_Number; i++)
    {
        ui->tankComboBox->addItem( QString::number(i+1) + QStringLiteral("#槽"));
    }
}

void RealTimeDialog::resetChart()
{
    chart->removeAxis(axisX);
    chart->removeAxis(axisY);
    chart->removeAllSeries();
    resetChartSuccess = false;
    getAllData();
}

void RealTimeDialog::showEvent(QShowEvent *)
{
    getAllData();
}

void RealTimeDialog::closeEvent(QCloseEvent *)
{
    getRealTimeDataTcpClient->close();
    myTimerThread->terminate();
}

void RealTimeDialog::getRealTimeData()
{
    if(resetChartSuccess)
    {
        QDateTime todayTime = QDateTime::currentDateTime();
        if(currentDate < todayTime.date())
        {
            resetChart();
            return;
        }
        StreamPack bpack;
        int deviceIndex = ui->tankComboBox->currentIndex();
        if(deviceIndex >= 0)
        {
            QString cname = ui->comboBox->currentText();
            DeviceNode node = Global::getFermenationNodeInfoByCname(cname);
            DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
            ushort address = node.Offset + 4 * (info.offset + deviceIndex - info.startIndex);
            bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,r_RealData,Float,address,0,1,0,0,0};
            getRealTimeDataTcpClient->sendRequest(bpack);
        }
    }
}

void RealTimeDialog::getAllData()
{
    StreamPack bpack;
    QDateTime todayTime = QDateTime::currentDateTime();
    currentDate = QDate(todayTime.date());
    QDateTime startTime = QDateTime(QDate(todayTime.date()), QTime(0,0,0,0));
    //QDateTime endTime = QDateTime(QDate(todayTime.date()), QTime(23,59,59,999));
    QDateTime endTime = QDateTime(QDate(todayTime.date()).addDays(1), QTime(0,0,0,0));
    uint stime = startTime.toTime_t();
    uint etime = endTime.toTime_t();
    int deviceIndex = ui->tankComboBox->currentIndex();
    if(deviceIndex >= 0)
    {
        QString cname = ui->comboBox->currentText();
        DeviceNode node = Global::getFermenationNodeInfoByCname(cname);
        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
        ushort address = node.Offset + 4 * (info.offset + deviceIndex - info.startIndex);
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,r_HisData,Float,address,(quint16)deviceIndex,0,0,stime,etime};
        getAllDataTcpClient->sendRequest(bpack);
    }
}

void RealTimeDialog::showAllData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QString msg;
    if(bDevice.bErrorCode==1)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice));
        QVector<qreal> realValues;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            float temp = 0;
            memcpy(&temp,value,4);
            realValues.append(temp);
            msg.append(QString::number(temp));
            msg.append(",");
        }

        QVector<QDateTime> valueTimes;
        for(quint16 i=bDevice.bDataLength; i<2*bDevice.bDataLength; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            int temp = 0;
            memcpy(&temp,value,4);
            valueTimes.append(QDateTime::fromTime_t(temp));
            msg.append(QString::number(temp));
            msg.append(",");
        }

        QDateTime todayTime = QDateTime::currentDateTime();
        QDateTime startTime = QDateTime(QDate(todayTime.date()), QTime(0,0,0,0));
        //QDateTime endTime = QDateTime(QDate(todayTime.date()), QTime(23,59,59,999));
        QDateTime endTime = QDateTime(QDate(todayTime.date()).addDays(1), QTime(0,0,0,0));

        series = new QLineSeries();
        series->setPen(QPen(Qt::blue,1,Qt::SolidLine));

        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            qreal x = valueTimes[i].toMSecsSinceEpoch();
            qreal y = realValues[i];
            *series << QPointF(x, y);
        }

        chart->addSeries(series);
        //chart->setTitle(ui->comboBox->currentText());
        //chart->setAnimationOptions(QChart::SeriesAnimations);//设置曲线呈动画显示

        axisY = new QValueAxis; //定义Y轴
        axisY->setRange(0, 200); //设置范围
        axisY->setLabelFormat("%g"); //设置刻度的格式
        axisY->setTitleText("Y Axis"); //设置X轴的标题
        axisY->setGridLineVisible(true); //设置是否显示网格线
        //axisX->setMinorTickCount(5); //设置小刻度线的数目
        //axisX->setLabelsVisible(false); //设置刻度是否显示

        axisX = new QDateTimeAxis;        
        axisX->setFormat(QStringLiteral("h时"));
        axisX->setRange(startTime, endTime);
        axisX->setGridLineVisible(true);
        axisX->setTickCount(25);

        chart->setAxisX(axisX, series);
        chart->setAxisY(axisY, series);
        chart->legend()->hide();

        resetChartSuccess = true;
        qDebug() << "Reset chart success!";

        myTimerThread->start();
    }
}

void RealTimeDialog::showRealTime(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QString msg;
    if(bDevice.bErrorCode==1 && resetChartSuccess)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice));
        QVector<qreal> realValues;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            float temp = 0;
            memcpy(&temp,value,4);
            realValues.append(temp);
            msg.append(QString::number(temp));
            msg.append(",");
        }

        qDebug() << "Real time value:" << msg;

        qDebug() << QDateTime::currentDateTime().toString();
        quint64 t1 = QDateTime::currentDateTime().toMSecsSinceEpoch();
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            qreal x = QDateTime::currentDateTime().toMSecsSinceEpoch();            
            qreal y = realValues[i];
            series->append(QPointF(x, y));
        }
        quint64 t2 = QDateTime::currentDateTime().toMSecsSinceEpoch();
        qDebug() << t2 - t1;
    }
}

RealTimeDialog::~RealTimeDialog()
{
    delete ui;
}

void RealTimeDialog::on_tankComboBox_currentIndexChanged(int index)
{
    resetChart();
}

void RealTimeDialog::on_comboBox_currentIndexChanged(int index)
{
    resetChart();
}
