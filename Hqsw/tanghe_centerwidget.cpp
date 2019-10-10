#include "tanghe_centerwidget.h"
#include "ui_tanghe_centerwidget.h"

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

TangHe_CenterWidget::TangHe_CenterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TangHe_CenterWidget)
{
    ui->setupUi(this);

    for(int i=0; i < Global::ferDeviceInfo.Device_Number; i++)
    {
        FFTank *tk = new FFTank;
        tk->setTankIndex(i);
        tanks.append(tk);
    }

    ui->tankFrameLayout1->setSpacing(0);
    int i=0;
    for(;i < 10; i++)
    {
        ui->tankFrameLayout1->addWidget(tanks[i]);
    }

    ui->tankFrameLayout2->setSpacing(0);
    for(;i < 20; i++)
    {
        ui->tankFrameLayout2->addWidget(tanks[i]);
    }

    ui->tankFrameLayout3->setSpacing(0);
    for(;i < 30; i++)
    {
        ui->tankFrameLayout3->addWidget(tanks[i]);
    }

    getServerConnectStateTcpClient = new TcpClientSocket(this);
    connect(getServerConnectStateTcpClient, SIGNAL(updateConnectState(bool)), this, SLOT(localServerConnected(bool)));
    getAllPlcDataTcpClient = new TcpClientSocket(this);
    connect(getAllPlcDataTcpClient, SIGNAL(updateClients(QByteArray)), this, SLOT(showAllPlcData(QByteArray)));

    //read_tank_data();

    fancontrolDialog = new FanControlDialog(this);
    fercontrolDialog = new FerControlDialog(this);
    reportDialog = new ReportDialog(this);
    loginDialog = new LoginDialog(this);
    historyDialog = new HistoryDialog(this);
    settingDialog = new SettingDialog(this);
    alertQueryDialog = new AlertQueryDialog(this);
    deoControlDialog = new DeoControlDialog(this);

    QPalette p;
    p = ui->projectNameLabel->palette();
    p.setColor(QPalette::WindowText,QColor(56,126,214));
    ui->projectNameLabel->setPalette(p);
}

TangHe_CenterWidget::~TangHe_CenterWidget()
{
    getServerConnectStateTcpClient->close();
    getAllPlcDataTcpClient->close();
    delete ui;
}

void TangHe_CenterWidget::timerEvent(QTimerEvent *event)
{
    read_tank_data();
}

void TangHe_CenterWidget::showEvent(QShowEvent *event)
{
    myTimerThread = new MyTimerThread(2, this);
    connect(myTimerThread, SIGNAL(timeout()),this,SLOT(read_tank_data()));
    myTimerThread->start();
}

void TangHe_CenterWidget::closeEvent(QCloseEvent *event)
{

}

void TangHe_CenterWidget::showAllPlcData(QByteArray data)
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

        uint tankLocationAddrss = Global::getFermenationNodeInfoByName("FER_LOCATION_UI").Offset;
        uint tankLocationNumber = Global::getFermenationNodeInfoByName("FER_LOCATION_UI").Length;
        for(uint i=0; i < tankLocationNumber; i++)
        {
            float address = float(tankLocationAddrss + 2 * i);
            tanks[i]->setLocation(plcDataMap[address].toUInt());
        }

        parseFermentationData(plcDataMap);
        parseRunCtrData(plcDataMap);
        parseRunTimeData(plcDataMap);
        parseStepData(plcDataMap);
    }
}

void TangHe_CenterWidget::parseFermentationData(QMap<float,QString> dataMap)
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
    //qDebug() << "high point tempture: " << realValues[0];
    for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
    {
        tanks[j]->setupFermentationMapper(realValues, tanks[j]->getLocation());
    }
}

void TangHe_CenterWidget::parseRunTimeData(QMap<float,QString> dataMap)
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
        tanks[j]->setupStepRunningTimeMapper(uintValues, tanks[j]->getLocation());
    }
}

void TangHe_CenterWidget::parseStepData(QMap<float,QString> dataMap)
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
        tanks[j]->setupStepMapper(ushortValues, tanks[j]->getLocation());
    }
}

void TangHe_CenterWidget::parseRunCtrData(QMap<float,QString> dataMap)
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

                Global::alertsModel->appendRow(newItemList);
                Global::currentFermenationDataMap[dictAddress] = dataMap[dictAddress];
            }
        }
    }

    for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
    {
        tanks[j]->setupRunCtrMapper(boolValues, tanks[j]->getLocation());
    }
}

void TangHe_CenterWidget::showFermentationData(QByteArray data)
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
            tanks[j]->setupFermentationMapper(realValues, j);
        }
    }
}

void TangHe_CenterWidget::showRunTimeData(QByteArray data)
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
            tanks[j]->setupStepRunningTimeMapper(values, j);
        }
    }
}

void TangHe_CenterWidget::showStepData(QByteArray data)
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
            tanks[j]->setupStepMapper(values, j);
        }
    }
}

// /Todo
void TangHe_CenterWidget::showRunCtrData(QByteArray data)
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
            tanks[j]->setupRunCtrMapper(values, j);
        }
    }
}

void TangHe_CenterWidget::requestFermentationData(StreamPack requestPack)
{
    getFerTcpClient->sendRequest(requestPack);
}

void TangHe_CenterWidget::requestRunTimeData(StreamPack requestPack)
{
    getRunTimeTcpClient->sendRequest(requestPack);
}

void TangHe_CenterWidget::requestStepData(StreamPack requestPack)
{
    getStepTcpClient->sendRequest(requestPack);
}

void TangHe_CenterWidget::requestRunCtrData(StreamPack requestPack)
{
    getRunCtrTcpClient->sendRequest(requestPack);
}

void TangHe_CenterWidget::localServerConnected(bool isConnected)
{
    readStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    isServerConnected = isConnected;
    if(isConnected)
    {
        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,0,r_AllCacheData,String,0,0,0,0,0,0};
        getAllPlcDataTcpClient->sendRequest(bpack);

        /*qDebug() << "Connected : " << readNumber;
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        bpack = {sizeof(StreamPack),108,r_RealData,UShort,(quint16)Global::deviceInfo.Step_Address,0,(quint16)Global::deviceInfo.Step_Num,0,stime,etime};
        requestStepData(bpack);
        qDebug() << "Send Step Request : " << readNumber;
        bpack = {sizeof(StreamPack),108,r_RealData,Float,(quint16)Global::deviceInfo.Fermentation_Address,0,(quint16)Global::deviceInfo.Fermentation_Num,0,stime,etime};
        requestFermentationData(bpack);
        qDebug() << "Send Fermentation Request : " << readNumber;
        bpack = {sizeof(StreamPack),108,r_RealData,Bool,(quint16)Global::deviceInfo.Runctr_Address,0,(quint16)Global::deviceInfo.Runctr_Num,0,stime,etime};
        requestRunCtrData(bpack);
        qDebug() << "Send Runctr Request : " << readNumber;
        bpack = {sizeof(StreamPack),108,r_RealData,UInt,(quint16)Global::deviceInfo.Runtime_Address,0,(quint16)Global::deviceInfo.Runtime_Num,0,stime,etime};
        requestRunTimeData(bpack);
        qDebug() << "Send Run time Request : " << readNumber;*/
    }
}

void TangHe_CenterWidget::read_tank_data()
{
    //qDebug() << "Read_tank_data ***************************************************************** : " << readNumber++;
    getServerConnectStateTcpClient->sendTestConnectRequest();
}

QVector<FFTank *> TangHe_CenterWidget::getTanks() const
{
    return tanks;
}

void TangHe_CenterWidget::on_fanControlButton_clicked()
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

void TangHe_CenterWidget::on_ferControlButton_clicked()
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

void TangHe_CenterWidget::on_HistoryQueryButton_clicked()
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

void TangHe_CenterWidget::on_ReportButton_clicked()
{
    if(isServerConnected)
    {
        reportDialog->show();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText(QStringLiteral("服务器连接异常，请稍后重试，或检查网络状态和服务器软件状态！"));
        messageBox.exec();
    }
}

void TangHe_CenterWidget::on_toolButton_2_clicked()
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

void TangHe_CenterWidget::on_realTimeChartButton_clicked()
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

void TangHe_CenterWidget::on_exitButton_clicked()
{
    QApplication::quit();
}

void TangHe_CenterWidget::on_SettingButton_clicked()
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

void TangHe_CenterWidget::on_AlertQueryButton_clicked()
{
    alertQueryDialog->show();
}


void TangHe_CenterWidget::on_deoControlButton_clicked()
{
    deoControlDialog->show();
}
