#include "singelfertankvalvecontroldialog.h"
#include "ui_singelfertankvalvecontroldialog.h"

#include <QMessageBox>

#include "global.h"
#include "identity.h"

SingelFerTankValveControlDialog::SingelFerTankValveControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingelFerTankValveControlDialog)
{
    ui->setupUi(this);
    //setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);

    icoGreen.load("://image/old/FerLEDG.bmp");
    icoYellow.load("://image/old/FerLEDY.bmp");
    icoRed.load("://image/old/FerLEDY.bmp");
    valveAutoControlBg.load("://image/old/Auto.bmp");
    valveManualControlBg.load("://image/old/Mause.bmp");

    for(int i=0; i<Global::ferDeviceInfo.Device_Number;i++)
    {
        ui->ferTankValveIndexComboBox->addItem(QStringLiteral("发酵槽曝气阀门") + QString::number(i+1));
    }

    getServerConnectStateTcpClient = new TcpClientSocket(this);
    getValveDataTcpClient = new TcpClientSocket(this);
    connect(getServerConnectStateTcpClient, SIGNAL(updateConnectState(bool)), this, SLOT(localServerConnected(bool)));
    connect(getValveDataTcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showValveData(QByteArray)));
    actionTcpclient = new TcpClientSocket(this);
}

SingelFerTankValveControlDialog::~SingelFerTankValveControlDialog()
{
    delete ui;
}

int SingelFerTankValveControlDialog::getValveIndex() const
{
    return valveIndex;
}

void SingelFerTankValveControlDialog::setValveIndex(int value)
{
    valveIndex = value;
    ui->ferTankValveIndexComboBox->setCurrentIndex(valveIndex);
    ui->ferTankValveIndexLabel->setText(QString::number(value+1));
}

void SingelFerTankValveControlDialog::closeEvent(QCloseEvent *event)
{

}

void SingelFerTankValveControlDialog::showEvent(QShowEvent *)
{
    myTimerThread = new MyTimerThread(1, this);
    connect(myTimerThread, SIGNAL(timeout()),this,SLOT(read_valve_data()));
    myTimerThread->start();
}

void SingelFerTankValveControlDialog::localServerConnected(bool isConnected)
{
    isServerConnected = isConnected;
    if(isConnected)
    {
        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,0,r_AllCacheData,String,0,0,0,0,0,0};
        getValveDataTcpClient->sendRequest(bpack);
    }
}

void SingelFerTankValveControlDialog::read_valve_data()
{
    start = QDateTime::currentDateTime().toMSecsSinceEpoch();
    getServerConnectStateTcpClient->sendTestConnectRequest();
}

void SingelFerTankValveControlDialog::showValveData(QByteArray data)
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

        parseRunCtrData(plcDataMap);
    }
}

void SingelFerTankValveControlDialog::parseRunCtrData(QMap<float,QString> dataMap)
{
    stop = QDateTime::currentDateTime().toMSecsSinceEpoch();
    qDebug() << "Looping Space is " << stop - start << "; Stop is " << stop;
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
                QList<QStandardItem *> newSimpleAlertItemList;
                Global::alertIndex += 1;
                QString simpleAlert;

                newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                newItemList.append(new QStandardItem(QString::number(tankIndex+1)));
                if(tempValue.toBool())
                {
                    newItemList.append(new QStandardItem(Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert1));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            QString::number(tankIndex+1) + "#" +
                            Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert1 + " " +
                            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                }
                else
                {
                    newItemList.append(new QStandardItem(Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert0));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            QString::number(tankIndex+1) + "#" +
                            Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert0 + " " +
                            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

                }
                QStandardItem *simpleAlertItem = new QStandardItem(simpleAlert);
                newSimpleAlertItemList.append(simpleAlertItem);

                if(Identity::getInstance()->getUser() != Q_NULLPTR)
                {
                    newItemList.append(new QStandardItem(Identity::getInstance()->getUser()->getUsername()));
                }
                else
                {
                    newItemList.append(new QStandardItem(""));
                }

                Global::simpleAlertsModel->insertRow(0, newSimpleAlertItemList);
                Global::alertsModel->insertRow(0, newItemList);
                Global::currentFermenationDataMap[dictAddress] = dataMap[dictAddress];
            }
        }
    }

    int deviceIndex = ui->ferTankValveIndexComboBox->currentIndex();
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
    int runctrBlockSize = (info.offset + deviceIndex - info.startIndex)*Global::ferDeviceInfo.RunCtr_Block_Size;

    bool isOpening = false;
    bool isClosing = false;
    bool isOpened = false;
    bool isClosed = false;
    bool isFault = false;
    isOpening = boolValues[runctrBlockSize + Global::getFermenationNodeInfoByName("VALVE_Opening_BOOL").Offset];
    isClosing = boolValues[runctrBlockSize + Global::getFermenationNodeInfoByName("VALVE_Closing_BOOL").Offset];
    isOpened = boolValues[runctrBlockSize + Global::getFermenationNodeInfoByName("VALVE_Opened_BOOL").Offset];
    isClosed = boolValues[runctrBlockSize + Global::getFermenationNodeInfoByName("VALVE_Closed_BOOL").Offset];
    isFault = boolValues[runctrBlockSize + Global::getFermenationNodeInfoByName("VALVE_Fault_BOOL").Offset];

    ui->valveOpenedStateLabel->setObjectName("openstate");
    if(isOpened)
    {
        ui->valveOpenedStateLabel->setStyleSheet("QLabel#openstate{background-color: rgb(0, 255, 0);}");
        //p.setBrush(QPalette::Background,QBrush(QColor(Qt::green)));
    }
    else
    {
        ui->valveOpenedStateLabel->setStyleSheet("QLabel#openstate{background-color: rgb(255, 255, 255);}");
        //p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
    }
    if(isOpening)
    {
        if(switchFlag)
        {
            ui->valveOpenedStateLabel->setStyleSheet("QLabel#openstate{background-color: rgb(0, 255, 0);}");
            //p.setBrush(QPalette::Background,QBrush(QColor(Qt::green)));
        }
        else
        {
            ui->valveOpenedStateLabel->setStyleSheet("QLabel#openstate{background-color: rgb(255, 255, 255);}");
            //p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
        }
    }
    //ui->valveOpenedStateLabel->setPalette(p);

    ui->valveClosedStateLabel->setObjectName("closedstate");
    if(isClosed)
    {
        ui->valveClosedStateLabel->setStyleSheet("QLabel#closedstate{background-color: rgb(255, 0, 0);}");
        //p.setBrush(QPalette::Background,QBrush(QColor(Qt::red)));
    }
    else
    {
        ui->valveClosedStateLabel->setStyleSheet("QLabel#closedstate{background-color: rgb(255, 255, 255);}");
        //p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
    }
    if(isClosing)
    {
        if(switchFlag)
        {
            ui->valveClosedStateLabel->setStyleSheet("QLabel#closedstate{background-color: rgb(255, 0, 0);}");
            //p.setBrush(QPalette::Background,QBrush(QColor(Qt::red)));
        }
        else
        {
            ui->valveClosedStateLabel->setStyleSheet("QLabel#closedstate{background-color: rgb(255, 255, 255);}");
            //p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
        }
    }
    //ui->valveClosedStateLabel->setPalette(p);

    ui->valvefaultStateLabel->setObjectName("faultstate");
    if(isFault)
    {
        if(switchFlag)
        {
            ui->valvefaultStateLabel->setStyleSheet("QLabel#faultstate{background-color: rgb(255, 255, 0);}");
            //p.setBrush(QPalette::Background,QBrush(QColor(Qt::yellow)));
        }
        else
        {
            ui->valvefaultStateLabel->setStyleSheet("QLabel#faultstate{background-color: rgb(255, 255, 255);}");
            //p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
        }
    }
    else
    {
        ui->valvefaultStateLabel->setStyleSheet("QLabel#faultstate{background-color: rgb(255, 255, 255);}");
        //p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
    }
    //ui->valvefaultStateLabel->setPalette(p);

    isAuto = boolValues[runctrBlockSize + Global::getFermenationNodeInfoByName("Valve_Auto_BOOL").Offset];
    if(isAuto)
    {
        ui->switchValveControlModePushButton->setIcon(QIcon(valveAutoControlBg));
    }
    else
    {
        ui->switchValveControlModePushButton->setIcon(QIcon(valveManualControlBg));
    }

    bool isRemote = false;
    isRemote = boolValues[runctrBlockSize + Global::getFermenationNodeInfoByName("VALVE_Remote_BOOL").Offset];
    if(!isRemote)
    {
        ui->valveOpenPushButton->setEnabled(false);
        ui->valveStopPushButton->setEnabled(false);
    }
    else
    {
        ui->valveOpenPushButton->setEnabled(true);
        ui->valveStopPushButton->setEnabled(true);
    }

    switchFlag = !switchFlag;
}

void SingelFerTankValveControlDialog::on_valveOpenPushButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFermenationNodeInfoByName("VALVE_HAND_OPEN_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("VALVE_HAND_OPEN_BOOL").Offset % 8;

        int deviceIndex = ui->ferTankValveIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = true;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SingelFerTankValveControlDialog::on_valveStopPushButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFermenationNodeInfoByName("VALVE_HAND_CLOSE_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("VALVE_HAND_CLOSE_BOOL").Offset % 8;

        int deviceIndex = ui->ferTankValveIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = true;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SingelFerTankValveControlDialog::on_valveOpenPushButton_released()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFermenationNodeInfoByName("VALVE_HAND_OPEN_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("VALVE_HAND_OPEN_BOOL").Offset % 8;

        int deviceIndex = ui->ferTankValveIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = false;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
}

void SingelFerTankValveControlDialog::on_valveStopPushButton_released()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFermenationNodeInfoByName("VALVE_HAND_CLOSE_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("VALVE_HAND_CLOSE_BOOL").Offset % 8;

        int deviceIndex = ui->ferTankValveIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = false;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
}

void SingelFerTankValveControlDialog::on_switchValveControlModePushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFermenationNodeInfoByName("Valve_Auto_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("Valve_Auto_BOOL").Offset % 8;

        int deviceIndex = ui->ferTankValveIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = !isAuto;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}
