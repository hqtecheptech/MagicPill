#include "fanvalvecontroldialog.h"
#include "ui_fanvalvecontroldialog.h"

#include <QMessageBox>

#include "global.h"
#include "identity.h"

FanValveControlDialog::FanValveControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FanValveControlDialog)
{
    ui->setupUi(this);
    //setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    icoGreen.load("://image/old/FerLEDG.bmp");
    icoYellow.load("://image/old/FerLEDY.bmp");
    icoRed.load("://image/old/FerLEDY.bmp");
    valveAutoControlBg.load("://image/old/Auto.bmp");
    valveManualControlBg.load("://image/old/Mause.bmp");

    for(int i=0; i<Global::fanValveDeviceInfo.Device_Number;i++)
    {
        ui->valveGruopIndexComboBox->addItem(QStringLiteral("风机阀门组") + QString::number(i+1));
    }

    getServerConnectStateTcpClient = new TcpClientSocket(this);
    getValveGroupDataTcpClient = new TcpClientSocket(this);
    connect(getServerConnectStateTcpClient, SIGNAL(updateConnectState(bool)), this, SLOT(localServerConnected(bool)));
    connect(getValveGroupDataTcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showValveGroupData(QByteArray)));
    actionTcpclient = new TcpClientSocket(this);
}

FanValveControlDialog::~FanValveControlDialog()
{
    delete ui;
}

void FanValveControlDialog::closeEvent(QCloseEvent *event)
{

}

void FanValveControlDialog::showEvent(QShowEvent *)
{
    myTimerThread = new MyTimerThread(1, this);
    connect(myTimerThread, SIGNAL(timeout()),this,SLOT(read_valve_group_data()));
    myTimerThread->start();
}

void FanValveControlDialog::localServerConnected(bool isConnected)
{
    isServerConnected = isConnected;
    if(isConnected)
    {
        StreamPack bpack;
        bpack = {sizeof(StreamPack),3,0,r_AllCacheData,String,0,0,0,0,0,0};
        getValveGroupDataTcpClient->sendRequest(bpack);
    }
}

void FanValveControlDialog::read_valve_group_data()
{
    getServerConnectStateTcpClient->sendTestConnectRequest();
}

void FanValveControlDialog::showValveGroupData(QByteArray data)
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
            qDebug() << "address" << addressArray[i];
            qDebug() << "value" << strArray[i];
        }

        parseRunCtrData(plcDataMap);
    }
}

void FanValveControlDialog::parseRunCtrData(QMap<float,QString> dataMap)
{
    uint startAddrss = Global::fanValveDeviceInfo.Runctr_Address;
    uint valueNumber = Global::fanValveDeviceInfo.Runctr_Num;
    QVector<bool> boolValues;
    for(uint i=0; i < valueNumber; i++)
    {
        uint step = i / 8;
        uint temp = i % 8;
        float index = float(temp) / 10;
        float dictAddress = index + startAddrss + step;
        QVariant tempValue = dataMap[dictAddress];
        boolValues.append(tempValue.toBool());

        if(!Global::currentFanValveGroupDataMap.contains(dictAddress))
        {
            Global::currentFanValveGroupDataMap.insert(dictAddress,dataMap[dictAddress]);
        }
        else
        {
            if(Global::currentFanValveGroupDataMap[dictAddress] != dataMap[dictAddress])
            {
                uint tankIndex = i / Global::fanValveDeviceInfo.RunCtr_Block_Size;
                QList<QStandardItem *> newItemList;
                QList<QStandardItem *> newSimpleAlertItemList;
                Global::alertIndex += 1;
                QString simpleAlert;

                newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                newItemList.append(new QStandardItem(QString::number(tankIndex+1)));
                if(tempValue.toBool())
                {
                    newItemList.append(new QStandardItem(Global::fanValveRunCtrDeviceNodes[i % Global::fanValveDeviceInfo.RunCtr_Block_Size].Alert1));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            QString::number(tankIndex+1) + "#" +
                            Global::fanValveRunCtrDeviceNodes[i % Global::fanValveDeviceInfo.RunCtr_Block_Size].Alert1 +
                            " " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                }
                else
                {
                    newItemList.append(new QStandardItem(Global::fanValveRunCtrDeviceNodes[i % Global::fanValveDeviceInfo.RunCtr_Block_Size].Alert0));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            QString::number(tankIndex+1) + "#" +
                            Global::fanValveRunCtrDeviceNodes[i % Global::fanValveDeviceInfo.RunCtr_Block_Size].Alert0 +
                            " " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

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
                Global::currentFanValveGroupDataMap[dictAddress] = dataMap[dictAddress];
            }
        }
    }

    int deviceIndex = ui->valveGruopIndexComboBox->currentIndex();
    DeviceGroupInfo info = Global::getFanValveDeviceGroupInfo(deviceIndex);
    int runctrBlockSize = (info.offset + deviceIndex - info.startIndex)*Global::fanValveDeviceInfo.RunCtr_Block_Size;

    bool isOpened = false;
    bool isClosed = false;
    bool isOpening = false;
    bool isClosing = false;
    bool isFault = false;
    isOpening = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_VALVE_Opening").Offset];
    isClosing = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_VALVE_Closing").Offset];
    isOpened = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_VALVE_Opened").Offset];
    isClosed = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_VALVE_Closed").Offset];
    isFault = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_VALVE_Fault").Offset];

    if(isOpened)
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::green)));
    }
    else
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
    }
    if(isOpening)
    {
        if(switchFlag)
        {
            p.setBrush(QPalette::Background,QBrush(QColor(Qt::green)));
        }
        else
        {
            p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
        }
    }
    ui->valveOpenedStateLabel->setPalette(p);

    if(isClosed)
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::red)));
    }
    else
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
    }
    if(isClosing)
    {
        if(switchFlag)
        {
            p.setBrush(QPalette::Background,QBrush(QColor(Qt::red)));
        }
        else
        {
            p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
        }
    }
    ui->valveClosedStateLabel->setPalette(p);

    if(isFault)
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::yellow)));
    }
    else
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
    }
    ui->valvefaultStateLabel->setPalette(p);

    isOpening = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_Opening").Offset];
    isClosing = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_Closing").Offset];
    isOpened = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_Opened").Offset];
    isClosed = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_Closed").Offset];
    isFault = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_Fault").Offset];

    if(isOpened)
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::green)));
    }
    else
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
    }
    if(isOpening)
    {
        if(switchFlag)
        {
            p.setBrush(QPalette::Background,QBrush(QColor(Qt::green)));
        }
        else
        {
            p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
        }
    }
    ui->backupValveOpenedStateLabel->setPalette(p);

    if(isClosed)
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::red)));
    }
    else
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
    }
    if(isClosing)
    {
        if(switchFlag)
        {
            p.setBrush(QPalette::Background,QBrush(QColor(Qt::red)));
        }
        else
        {
            p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
        }
    }
    ui->backupValveClosedStateLabel->setPalette(p);

    if(isFault)
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::yellow)));
    }
    else
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
    }
    ui->backupValvefaultStateLabel->setPalette(p);

    bool isRemote = false;
    isValveControlAuto = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_VALVE_Auto_BOOL").Offset];
    qDebug() << "offset = " << Global::getFanValveNodeInfoByName("FAN_VALVE_Auto_BOOL").Offset;
    qDebug() << "isValveControlAuto = " << isValveControlAuto;
    isRemote = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_VALVE_Remote").Offset];
    if(isValveControlAuto)
    {
        ui->switchValveControlModePushButton->setIcon(QIcon(valveAutoControlBg));
    }
    else
    {
        ui->switchValveControlModePushButton->setIcon(QIcon(valveManualControlBg));
    }
    if(isRemote)
    {
        ui->valveOpenPushButton->setEnabled(true);
        ui->valveStopPushButton->setEnabled(true);
    }
    else
    {
        ui->valveOpenPushButton->setEnabled(false);
        ui->valveStopPushButton->setEnabled(false);
    }

    isBackupValveControlAuto = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_Auto_BOOL").Offset];
    qDebug() << "offset = " << Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_Auto_BOOL").Offset;
    qDebug() << "isValveControlAuto = " << isValveControlAuto;
    isRemote = boolValues[runctrBlockSize + Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_Remote").Offset];
    if(isBackupValveControlAuto)
    {
        ui->switchBackupValveControlModePushButton->setIcon(QIcon(valveAutoControlBg));
    }
    else
    {
        ui->switchBackupValveControlModePushButton->setIcon(QIcon(valveManualControlBg));
    }
    if(isRemote)
    {
        ui->backupValveOpenPushButton->setEnabled(true);
        ui->backupValveStopPushButton->setEnabled(true);
    }
    else
    {
        ui->backupValveOpenPushButton->setEnabled(false);
        ui->backupValveStopPushButton->setEnabled(false);
    }

    switchFlag = !switchFlag;
}

void FanValveControlDialog::on_switchValveControlModePushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanValveNodeInfoByName("FAN_Auto_BOOL").Offset / 8;
        ushort index = Global::getFanValveNodeInfoByName("FAN_Auto_BOOL").Offset % 8;

        int deviceIndex = ui->valveGruopIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanValveDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanValveDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanValveDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = !isBackupValveControlAuto;
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

void FanValveControlDialog::on_switchBackupValveControlModePushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_Auto_BOOL").Offset / 8;
        ushort index = Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_Auto_BOOL").Offset % 8;

        int deviceIndex = ui->valveGruopIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanValveDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanValveDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanValveDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = !isBackupValveControlAuto;
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

void FanValveControlDialog::on_valveOpenPushButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanValveNodeInfoByName("FAN_VALVE_HAND_OPEN").Offset / 8;
        ushort index = Global::getFanValveNodeInfoByName("FAN_VALVE_HAND_OPEN").Offset % 8;

        int deviceIndex = ui->valveGruopIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanValveDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanValveDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanValveDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
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

void FanValveControlDialog::on_valveOpenPushButton_released()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanValveNodeInfoByName("FAN_VALVE_HAND_OPEN").Offset / 8;
        ushort index = Global::getFanValveNodeInfoByName("FAN_VALVE_HAND_OPEN").Offset % 8;

        int deviceIndex = ui->valveGruopIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanValveDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanValveDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanValveDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = false;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
}

void FanValveControlDialog::on_valveStopPushButton_released()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanValveNodeInfoByName("FAN_VALVE_HAND_CLOSE").Offset / 8;
        ushort index = Global::getFanValveNodeInfoByName("FAN_VALVE_HAND_CLOSE").Offset % 8;

        int deviceIndex = ui->valveGruopIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanValveDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanValveDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanValveDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = false;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
}

void FanValveControlDialog::on_valveStopPushButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanValveNodeInfoByName("FAN_VALVE_HAND_CLOSE").Offset / 8;
        ushort index = Global::getFanValveNodeInfoByName("FAN_VALVE_HAND_CLOSE").Offset % 8;

        int deviceIndex = ui->valveGruopIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanValveDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanValveDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanValveDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
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

void FanValveControlDialog::on_backupValveOpenPushButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_HAND_OPEN").Offset / 8;
        ushort index = Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_HAND_OPEN").Offset % 8;

        int deviceIndex = ui->valveGruopIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanValveDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanValveDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanValveDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
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

void FanValveControlDialog::on_backupValveOpenPushButton_released()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_HAND_OPEN").Offset / 8;
        ushort index = Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_HAND_OPEN").Offset % 8;

        int deviceIndex = ui->valveGruopIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanValveDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanValveDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanValveDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = false;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
}

void FanValveControlDialog::on_backupValveStopPushButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_HAND_CLOSE").Offset / 8;
        ushort index = Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_HAND_CLOSE").Offset % 8;

        int deviceIndex = ui->valveGruopIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanValveDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanValveDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanValveDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
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

void FanValveControlDialog::on_backupValveStopPushButton_released()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_HAND_CLOSE").Offset / 8;
        ushort index = Global::getFanValveNodeInfoByName("FAN_SPAREVALVE_HAND_CLOSE").Offset % 8;

        int deviceIndex = ui->valveGruopIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanValveDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanValveDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanValveDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = false;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
}
