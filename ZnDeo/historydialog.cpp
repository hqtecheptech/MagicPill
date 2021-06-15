#include "historydialog.h"
#include "ui_historydialog.h"
#include "dataformat.h"
#include "requestworkthread.h"
#include "global.h"
#include "adduserdialog.h"

#include <QMessageBox>

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QDateTimeAxis>

QT_CHARTS_USE_NAMESPACE

HistoryDialog::HistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoryDialog)
{
    ui->setupUi(this);
    //setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);

    setWindowTitle(QStringLiteral("历史曲线"));
    ui->textEdit->setVisible(false);

    //queryWork = new RequestWorkThread(this);
    //connect(queryWork,SIGNAL(updateParentThread(QByteArray)),this,SLOT(showHistory(QByteArray)));

    queryTcpClient = new TcpClientSocket(this);
    connect(queryTcpClient,SIGNAL(updateClients(QByteArray)),this,SLOT(showHistory(QByteArray)));

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

HistoryDialog::~HistoryDialog()
{
    delete ui;
}

void HistoryDialog::closeEvent(QCloseEvent *)
{

}

void HistoryDialog::on_queryDataButton_clicked()
{
    QDate startDate = ui->startTimeEdit->dateTime().date();
    QDate endDate = ui->endTimeEdit->dateTime().date();

    uint stime = ui->startTimeEdit->dateTime().toTime_t();
    uint etime = ui->endTimeEdit->dateTime().toTime_t();

    if(etime > stime)
    {
        if((endDate.year() - startDate.year()) <=1)
        {
            int deviceIndex = ui->tankComboBox->currentIndex();
            QString cname = ui->comboBox->currentText();
            DeviceNode node = Global::getFermenationNodeInfoByCname(cname);
            DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
            ushort address = node.Offset + 4 * (info.offset + deviceIndex - info.startIndex);
            StreamPack bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,r_HisData,Float,address,(quint16)deviceIndex,1,0,stime,etime};

            queryTcpClient->sendRequest(bpack);
        }
        else
        {
            QMessageBox messageBox;
            messageBox.setText(QStringLiteral("查询间隔不能超过2年！"));
            messageBox.exec();
        }
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText(QStringLiteral("查询结束时间必须大于查询开始时间！"));
        messageBox.exec();
    }
}

void HistoryDialog::showHistory(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QString msg;
    if(bDevice.bErrorCode==1)
    {
        allPage = 0;
        allValueBytes.clear();
        allTimeBytes.clear();

        allValueBytes = data.mid(sizeof(bDevice), bDevice.bDataLength*4);
        allTimeBytes = data.mid(sizeof(bDevice) + bDevice.bDataLength*4);

        if((bDevice.bDataLength % 2000) > 0)
        {
            allPage = bDevice.bDataLength / 2000 + 1;
        }
        else
        {
            allPage = bDevice.bDataLength / 2000;
        }
        ui->allPageNumberLabel->setText(QString::number(allPage));

        showChartAtPage(1);
    }
    else if(bDevice.bErrorCode == 10)
    {
        ui->textEdit->setVisible(true);
        ui->textEdit->setText(QStringLiteral("选定的时间段内无历史数据！"));
        return;
    }

    ui->textEdit->setText(msg);
}

void HistoryDialog::on_prePushButton_clicked()
{
    showChartAtPage(currentPage - 1);
}

void HistoryDialog::on_nextPushButton_clicked()
{
    showChartAtPage(currentPage + 1);
}

void HistoryDialog::showChartAtPage(int page)
{
    ui->horizontalLayout->removeWidget(chartView);

    currentPage = page;
    ui->pageLineEdit->setText(QString::number(currentPage));

    QString msg;

    if(page == 1)
    {
        ui->prePushButton->setEnabled(false);
    }
    else
    {
        ui->prePushButton->setEnabled(true);
    }

    if(page == allPage)
    {
        ui->nextPushButton->setEnabled(false);
    }
    else
    {
        ui->nextPushButton->setEnabled(true);
    }

    QVector<qreal> values;
    QVector<QDateTime> times;

    qDebug() << "page: " << page;

    if(page > 0 && page < allPage)
    {
        QByteArray byteValues = allValueBytes.mid(2000 * (page - 1) * 4, 2000 * 4);
        qDebug() << "value bytes: " << byteValues.length();
        qDebug() << "value byteValues.length()/4: " << byteValues.length()/4;

        for(quint16 i=0; i<2000; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            float temp = 0;
            memcpy(&temp,value,4);
            values.append(temp);
            msg.append(QString::number(temp));
            msg.append(",");
        }

        byteValues = allTimeBytes.mid(2000 * (page - 1) * 4, 2000 * 4);
        qDebug() << "time bytes: " << byteValues.length();
        qDebug() << "time byteValues.length()/4: " << byteValues.length()/4;

        for(quint16 i=0; i<2000; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            int temp = 0;
            memcpy(&temp,value,4);
            times.append(QDateTime::fromTime_t(temp));
            msg.append(QString::number(temp));
            msg.append(",");
        }
    }
    else if(page == allPage)
    {
        QByteArray byteValues = allValueBytes.mid(2000 * (page - 1) * 4);
        qDebug() << "value bytes: " << byteValues.length();
        qDebug() << "value byteValues.length()/4: " << byteValues.length()/4;

        for(quint16 i=0; i<byteValues.length()/4; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            float temp = 0;
            memcpy(&temp,value,4);
            values.append(temp);
            msg.append(QString::number(temp));
            msg.append(",");
        }

        byteValues = allTimeBytes.mid(2000 * (page - 1) * 4);
        qDebug() << "time bytes: " << byteValues.length();
        qDebug() << "time byteValues.length()/4: " << byteValues.length()/4;

        for(quint16 i=0; i<byteValues.length()/4; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            int temp = 0;
            memcpy(&temp,value,4);
            times.append(QDateTime::fromTime_t(temp));
            msg.append(QString::number(temp));
            msg.append(",");
        }
    }
    else
    {
        return;
    }

    QDateTime startTime = times.at(0);
    QDateTime endTime = times.at(times.length()-1);

    if(values.length() > 0)
    {
        QLineSeries *series = new QLineSeries();
        series->setPen(QPen(Qt::blue,1,Qt::SolidLine));

        for(quint16 i=0; i<values.length(); ++i)
        {
            qreal x = times[i].toMSecsSinceEpoch();
            qreal y = values[i];
            *series << QPointF(x, y);
        }

        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle(ui->comboBox->currentText());
        chart->setAnimationOptions(QChart::SeriesAnimations);//设置曲线呈动画显示

        QValueAxis *axisY = new QValueAxis; //定义X轴
        axisY->setRange(0, 100); //设置范围
        axisY->setLabelFormat("%g"); //设置刻度的格式
        axisY->setTitleText("Y Axis"); //设置X轴的标题
        axisY->setGridLineVisible(true); //设置是否显示网格线
        //axisX->setMinorTickCount(5); //设置小刻度线的数目
        //axisX->setLabelsVisible(false); //设置刻度是否显示

        QDateTimeAxis *axisX = new QDateTimeAxis;
        axisX->setFormat("yy/MM/dd h:mm");
        axisX->setRange(startTime, endTime);
        axisX->setGridLineVisible(true);
        axisX->setTickCount(5);

        chart->setAxisX(axisX, series);
        chart->setAxisY(axisY, series);
        chart->legend()->hide();

        chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        ui->horizontalLayout->addWidget(chartView);
    }
}

void HistoryDialog::on_gotoPushButton_clicked()
{
    int page = ui->pageLineEdit->text().toInt();
    if(page > 0 && page <= allPage)
    {
        showChartAtPage(page);
    }
}
