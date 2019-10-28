#include "centerwidget.h"
#include "ui_centerwidget.h"
#include "fftank.h"
#include "tcpclientsocket.h"
#include "dataformat.h"
#include "global.h"
#include "tankstatebarchart.h"
#include "environment.h"
#include "fercontroldialog.h"
#include "requestworkthread.h"
#include "identity.h"

#include <QListWidget>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QThreadPool>
#include <QMessageBox>
#include <QMap>
#include <QStringList>

CenterWidget::CenterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CenterWidget)
{
    ui->setupUi(this);

    envModel = new QStandardItemModel(4,5,this);

    for(int i=0; i<Global::ferDeviceInfo.Device_Number; i++)
    {
        FFTank *tk = new FFTank;
        tk->setTankIndex(i);
        tanks.append(tk);
    }

    ui->tankFrameLayout1->setSpacing(0);
    ui->tankFrameLayout1->addWidget(tanks[0]);
    ui->tankFrameLayout1->addWidget(tanks[1]);
    ui->tankFrameLayout1->addWidget(tanks[2]);

    ui->tankFrameLayout2->setSpacing(0);
    ui->tankFrameLayout2->addWidget(tanks[3]);
    ui->tankFrameLayout2->addWidget(tanks[4]);
    ui->tankFrameLayout2->addWidget(tanks[5]);

    ui->tankFrameLayout3->setSpacing(0);
    ui->tankFrameLayout3->addWidget(tanks[6]);
    ui->tankFrameLayout3->addWidget(tanks[7]);
    ui->tankFrameLayout3->addWidget(tanks[8]);

    ui->tankFrameLayout4->setSpacing(0);
    ui->tankFrameLayout4->addWidget(tanks[9]);
    ui->tankFrameLayout4->addWidget(tanks[10]);
    ui->tankFrameLayout4->addWidget(tanks[11]);

    ui->tankFrameLayout5->setSpacing(0);
    ui->tankFrameLayout5->addWidget(tanks[12]);
    ui->tankFrameLayout5->addWidget(tanks[13]);
    ui->tankFrameLayout5->addWidget(tanks[14]);

    ui->tankFrameLayout6->setSpacing(0);
    ui->tankFrameLayout6->addWidget(tanks[15]);
    ui->tankFrameLayout6->addWidget(tanks[16]);
    ui->tankFrameLayout6->addWidget(tanks[17]);

    /*getRunTimeTcpClient = new TcpClientSocket(this);
    connect(getRunTimeTcpClient,SIGNAL(updateClients(QByteArray)),this,SLOT(showRunTimeData(QByteArray)));
    getFerTcpClient = new TcpClientSocket(this);
    connect(getFerTcpClient,SIGNAL(updateClients(QByteArray)),this,SLOT(showFermentationData(QByteArray)));
    getStepTcpClient = new TcpClientSocket(this);
    connect(getStepTcpClient,SIGNAL(updateClients(QByteArray)),this,SLOT(showStepData(QByteArray)));
    getRunCtrTcpClient = new TcpClientSocket(this);
    connect(getRunCtrTcpClient,SIGNAL(updateClients(QByteArray)),this,SLOT(showRunCtrData(QByteArray)));*/

    getServerConnectStateTcpClient = new TcpClientSocket(this);
    connect(getServerConnectStateTcpClient, SIGNAL(updateConnectState(bool)), this, SLOT(localServerConnected(bool)));
    getAllPlcDataTcpClient = new TcpClientSocket(this);
    connect(getAllPlcDataTcpClient, SIGNAL(updateClients(QByteArray)), this, SLOT(showAllPlcData(QByteArray)));

    //read_tank_data();

    fancontrolDialog = new FanControlDialog(this);
    fercontrolDialog = new FerControlDialog(this);
    //reportDialog = new ReportDialog(this);
    loginDialog = new LoginDialog(this);
    historyDialog = new HistoryDialog(this);
    settingDialog = new SettingDialog(this);
    alertQueryDialog = new AlertQueryDialog(this);
    deoControlDialog = new DeoControlDialog(this);

    tankChart = new TankStateBarChart(this);
    //ui->tankChartTitleLayout->setContentsMargins(0,0,0,0);
    ui->tankChartLayout->setContentsMargins(0,0,0,0);
    ui->tankChartLayout->addWidget(tankChart);

    QPalette p;
    p = ui->projectNameLabel->palette();
    p.setColor(QPalette::WindowText,QColor(56,126,214));
    ui->projectNameLabel->setPalette(p);

    //envWidget = new Environment;
    //ui->envHorizontalLayout->setSpacing(0);
    //ui->envHorizontalLayout->addWidget(envWidget);
}

CenterWidget::~CenterWidget()
{
    //getRunTimeTcpClient->close();
    //getFerTcpClient->close();
    //getStepTcpClient->close();
    //getRunCtrTcpClient->close();
    getServerConnectStateTcpClient->close();
    getAllPlcDataTcpClient->close();
    delete ui;
}

void CenterWidget::timerEvent(QTimerEvent *event)
{
    read_tank_data();
}

void CenterWidget::testRunCode()
{
    QMap<float,QString> plcDataMap;

    QFile dataFile("ferdatafile.txt");
    if(!dataFile.open(QIODevice::ReadOnly))
    {

    }
    QTextStream in(&dataFile);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList fields = line.split(' ');
        float address = fields.takeFirst().toFloat();
        QString value = fields.takeFirst();
        plcDataMap.insert(address, value);
    }

    /*QFile dataFile("ferdatafile.txt");
    if(!dataFile.open(QIODevice::WriteOnly))
    {

    }

    QTextStream out(&dataFile);
    for(int row = 0; row < addressArray.length(); row++)
    {
        out << addressArray[row] << " " << strArray[row] << endl;
    }*/

    parseFermentationData(plcDataMap);
    parseRunCtrData(plcDataMap);
    parseRunTimeData(plcDataMap);
    parseStepData(plcDataMap);
}

void CenterWidget::showEvent(QShowEvent *event)
{
    //testRunCode();

    myTimerThread = new MyTimerThread(2, this);
    connect(myTimerThread, SIGNAL(timeout()),this,SLOT(read_tank_data()));
    myTimerThread->start();
}

void CenterWidget::closeEvent(QCloseEvent *event)
{

}

void CenterWidget::showAllPlcData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QString msg;
    if(bDevice.bErrorCode==1)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice), data.length() - sizeof(bDevice) - 4 * bDevice.bDataLength);
        QString strValues(byteValues);
        QStringList strValueList = strValues.split(",");
        QVector<QString> strArray = strValueList.toVector();

        byteValues = data.mid(data.length() - 4 * bDevice.bDataLength, 4 * bDevice.bDataLength);
        QVector<float> addressArray;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            float temp = 0;
            memcpy(&temp,value,4);
            addressArray.append(temp);
            msg.append(QString::number(temp));
            msg.append(",");
        }

        QMap<float,QString> plcDataMap;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            plcDataMap.insert(addressArray[i],strArray[i]);
        }

        for(uint i=0; i < tanks.length(); i++)
        {
            tanks[i]->setLocation(i);
        }

        parseFermentationData(plcDataMap);
        parseRunCtrData(plcDataMap);
        parseRunTimeData(plcDataMap);
        parseStepData(plcDataMap);
    }
}

void CenterWidget::parseFermentationData(QMap<float,QString> dataMap)
{
    uint startAddrss = Global::ferDeviceInfo.Fermentation_Address;
    uint valueNumber = Global::ferDeviceInfo.Fermentation_Num;
    QVector<float> realValues;
    for(uint i=0; i < valueNumber; i++)
    {
        float address = float(startAddrss + 4 * i);
        realValues.append(dataMap[address].toFloat());

        if(!Global::currentFermenationDataMap.contains(address))
        {
            Global::currentFermenationDataMap.insert(address,dataMap[address]);
        }
        else
        {
            Global::currentFermenationDataMap[address] = dataMap[address];
        }
    }

    for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
    {
        //tanks[j]->setupFermentationMapper(realValues, j);
    }
}

void CenterWidget::parseRunTimeData(QMap<float,QString> dataMap)
{
    uint startAddrss = Global::ferDeviceInfo.Runtime_Address;
    uint valueNumber = Global::ferDeviceInfo.Runtime_Num;
    QVector<uint> uintValues;
    for(uint i=0; i < valueNumber; i++)
    {
        float address = float(startAddrss + 4 * i);
        uintValues.append(dataMap[address].toUInt());

        if(!Global::currentFermenationDataMap.contains(address))
        {
            Global::currentFermenationDataMap.insert(address,dataMap[address]);
        }
        else
        {
            Global::currentFermenationDataMap[address] = dataMap[address];
        }
    }
    for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
    {
        //tanks[j]->setupStepRunningTimeMapper(uintValues, j);
    }
}

void CenterWidget::parseStepData(QMap<float,QString> dataMap)
{
    uint startAddrss = Global::ferDeviceInfo.Tank_State_Address;
    uint valueNumber = Global::ferDeviceInfo.Tank_State_Num;
    QVector<ushort> ushortValues;
    for(uint i=0; i < valueNumber; i++)
    {
        float address = float(startAddrss + 2 * i);
        ushortValues.append(dataMap[address].toUShort());

        if(!Global::currentFermenationDataMap.contains(address))
        {
            Global::currentFermenationDataMap.insert(address,dataMap[address]);
        }
        else
        {
            Global::currentFermenationDataMap[address] = dataMap[address];
        }
    }

    for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
    {
        //tanks[j]->setupStepMapper(ushortValues, j);
    }
}

void CenterWidget::parseRunCtrData(QMap<float,QString> dataMap)
{
    uint startAddrss = Global::ferDeviceInfo.Runctr_Address;
    uint valueNumber = Global::ferDeviceInfo.Runctr_Num;
    QVector<bool> boolValues;
    for(uint i=0; i < valueNumber; i++)
    {
        uint step = i / 8;
        uint temp = i % 8;
        float index = float(temp) / 10;
        float dictAddress = index + startAddrss + step;
        QVariant tempValue = dataMap[dictAddress];
        boolValues.append(tempValue.toBool());

        if(!Global::currentFermenationDataMap.contains(dictAddress))
        {
            Global::currentFermenationDataMap.insert(dictAddress,dataMap[dictAddress]);
        }
        else
        {
            if(Global::currentFermenationDataMap[dictAddress] != dataMap[dictAddress])
            {
                uint tankIndex = i / Global::ferDeviceInfo.RunCtr_Block_Size;
                QList<QStandardItem *> newItemList;

                newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                newItemList.append(new QStandardItem(QString::number(tankIndex+1)));
                if(tempValue.toBool())
                {
                    newItemList.append(new QStandardItem(Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert1));
                }
                else
                {
                    newItemList.append(new QStandardItem(Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert0));
                }
                if(Identity::getInstance()->getUser() != Q_NULLPTR)
                {
                    newItemList.append(new QStandardItem(Identity::getInstance()->getUser()->getUsername()));
                }
                else
                {
                    newItemList.append(new QStandardItem(""));
                }

                Global::alertsModel->insertRow(0, newItemList);
                Global::currentFermenationDataMap[dictAddress] = dataMap[dictAddress];
            }
        }
    }

    for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
    {
        //tanks[j]->setupRunCtrMapper(boolValues, j);
    }
}

void CenterWidget::showFermentationData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    qDebug() << "showFermentationData: " << QDateTime::currentDateTime().toMSecsSinceEpoch() - readStartTime;
    qDebug() << "showFermentationData bErroCode: " << bDevice.bErrorCode;

    QString msg;
    if(bDevice.bErrorCode==1)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice));
        QVector<float> realValues;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            float temp = 0;
            memcpy(&temp,value,4);
            realValues.append(temp);
            msg.append(QString::number(temp));
            msg.append(",");
        }
        for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
        {
            //tanks[j]->setupFermentationMapper(realValues, j);
        }
    }
}

void CenterWidget::showRunTimeData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    qDebug() << "ShowRunTimeData: " << QDateTime::currentDateTime().toMSecsSinceEpoch() - readStartTime;
    qDebug() << "ShowRunTimeData bErroCode: " << bDevice.bErrorCode;

    QString msg;
    if(bDevice.bErrorCode==1)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice));
        QVector<uint> values;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            uint temp = 0;
            memcpy(&temp,value,4);
            values.append(temp);
            msg.append(QString::number(temp));
            msg.append(",");
        }
        for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
        {
            //tanks[j]->setupStepRunningTimeMapper(values, j);
        }
    }
}

void CenterWidget::showStepData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    qDebug() << "showStepData: " << QDateTime::currentDateTime().toMSecsSinceEpoch() - readStartTime;
    qDebug() << "showStepData bErroCode: " << bDevice.bErrorCode;

    QString msg;
    if(bDevice.bErrorCode==1)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice));
        QVector<ushort> values;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            QByteArray value = byteValues.mid(i*2,2);
            ushort temp = 0;
            memcpy(&temp,value,2);
            values.append(temp);
            msg.append(QString::number(temp));
            msg.append(",");
        }
        for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
        {
            //tanks[j]->setupStepMapper(values, j);
        }
    }
}

// /Todo
void CenterWidget::showRunCtrData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    qDebug() << "showRunCtrData: " << QDateTime::currentDateTime().toMSecsSinceEpoch() - readStartTime;
    qDebug() << "showRunCtrData bErroCode: " << bDevice.bErrorCode;

    QString msg;
    if(bDevice.bErrorCode==1)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice));
        QVector<bool> values;
        for(uint i=0; i<(uint)bDevice.bDataLength; ++i)
        {
            bool temp = false;
            int bSize = sizeof(temp);
            QByteArray value = byteValues.mid(i*bSize,bSize);
            memcpy(&temp,value,sizeof(value));
            values.append(temp);
            msg.append(QString::number(temp));
            msg.append(",");
        }
        for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
        {
            //tanks[j]->setupRunCtrMapper(values, j);
        }
    }
}

void CenterWidget::requestFermentationData(StreamPack requestPack)
{
    getFerTcpClient->sendRequest(requestPack);
}

void CenterWidget::requestRunTimeData(StreamPack requestPack)
{
    getRunTimeTcpClient->sendRequest(requestPack);
}

void CenterWidget::requestStepData(StreamPack requestPack)
{
    getStepTcpClient->sendRequest(requestPack);
}

void CenterWidget::requestRunCtrData(StreamPack requestPack)
{
    getRunCtrTcpClient->sendRequest(requestPack);
}

void CenterWidget::localServerConnected(bool isConnected)
{
    readStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    isServerConnected = isConnected;
    if(isConnected)
    {
        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,0,r_AllCacheData,String,0,0,0,0,0,0};
        getAllPlcDataTcpClient->sendRequest(bpack);
    }
}

void CenterWidget::read_tank_data()
{
    //qDebug() << "Read_tank_data ***************************************************************** : " << readNumber++;
    getServerConnectStateTcpClient->sendTestConnectRequest();
}

QVector<FFTank *> CenterWidget::getTanks() const
{
    return tanks;
}

/*@brief 重载自定义对象的输入*/
inline QDataStream &operator<<(QDataStream &out,const StreamPack &per)
{
    out<<per.bStreamLength<<per.bDeviceId<<per.bCommandType;
    out<<per.bDataType<<per.bAddress<<per.bIndex<<per.bDataLength<<per.bErrorCode;
    out<<per.bStartTime<<per.bEndTime;
    return out;
}
/*@brief 重载自定义对象的输出 */
inline QDataStream &operator>>(QDataStream &in,StreamPack &per)
{
    in>>per.bStreamLength>>per.bDeviceId>>per.bCommandType;
    in>>per.bDataType>>per.bAddress>>per.bIndex>>per.bDataLength>>per.bErrorCode;
    in>>per.bStartTime>>per.bEndTime;
    return in;
}

void CenterWidget::on_fanControlButton_clicked()
{
    if(isServerConnected)
    {
        fancontrolDialog->show();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText(QStringLiteral("服务器连接异常，请稍后重试，或检查网络状态和服务器软件状态！"));
        messageBox.exec();
    }
}

void CenterWidget::on_ferControlButton_clicked()
{
    if(isServerConnected)
    {
        fercontrolDialog->show();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText(QStringLiteral("服务器连接异常，请稍后重试，或检查网络状态和服务器软件状态！"));
        messageBox.exec();
    }
}

void CenterWidget::on_HistoryQueryButton_clicked()
{
    if(isServerConnected)
    {
        historyDialog->show();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText(QStringLiteral("服务器连接异常，请稍后重试，或检查网络状态和服务器软件状态！"));
        messageBox.exec();
    }
}

void CenterWidget::on_ReportButton_clicked()
{
    /*if(isServerConnected)
    {
        reportDialog->show();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText(QStringLiteral("服务器连接异常，请稍后重试，或检查网络状态和服务器软件状态！"));
        messageBox.exec();
    }*/
}

void CenterWidget::on_toolButton_2_clicked()
{
    if(isServerConnected)
    {
        loginDialog->show();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText(QStringLiteral("服务器连接异常，请稍后重试，或检查网络状态和服务器软件状态！"));
        messageBox.exec();
    }
}

void CenterWidget::on_realTimeChartButton_clicked()
{
    if(isServerConnected)
    {
        RealTimeDialog *realTimeDialog= new RealTimeDialog(this);
        realTimeDialog->show();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText(QStringLiteral("服务器连接异常，请稍后重试，或检查网络状态和服务器软件状态！"));
        messageBox.exec();
    }
}

void CenterWidget::on_exitButton_clicked()
{
    QApplication::quit();
}

void CenterWidget::on_SettingButton_clicked()
{
    if(isServerConnected)
    {
        settingDialog->show();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText(QStringLiteral("服务器连接异常，请稍后重试，或检查网络状态和服务器软件状态！"));
        messageBox.exec();
    }
}

void CenterWidget::on_AlertQueryButton_clicked()
{
    alertQueryDialog->show();
}

void CenterWidget::on_deoControlButton_clicked()
{
    deoControlDialog->show();
}
