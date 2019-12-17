#include "sharedfancontroldialog.h"
#include "ui_sharedfancontroldialog.h"

#include "global.h"
#include "identity.h"

SharedFanControlDialog::SharedFanControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SharedFanControlDialog)
{
    ui->setupUi(this);
    //setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);

    icoGreen.load("://image/old/FerLEDG.bmp");
    icoYellow.load("://image/old/FerLEDY.bmp");
    icoRed.load("://image/old/ICO_Red.ico");
    fanAutoControlBg.load("://image/old/Auto.bmp");
    fanManualControlBg.load("://image/old/Mause.bmp");

    for(int i=0; i<Global::fanDeviceInfo.Device_Number;i++)
    {
        ui->fanIndexComboBox->addItem(QStringLiteral("风机组") + QString::number(i+1));
    }

    getServerConnectStateTcpClient = new TcpClientSocket(this);
    getFanGroupDataTcpClient = new TcpClientSocket(this);
    connect(getServerConnectStateTcpClient, SIGNAL(updateConnectState(bool)), this, SLOT(localServerConnected(bool)));
    connect(getFanGroupDataTcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showFanGroupData(QByteArray)));
    actionTcpclient = new TcpClientSocket(this);
    setFrequencyTcpClient = new TcpClientSocket(this);
}

SharedFanControlDialog::~SharedFanControlDialog()
{
    delete ui;
}

void SharedFanControlDialog::closeEvent(QCloseEvent *event)
{

}

void SharedFanControlDialog::showEvent(QShowEvent *)
{
    myTimerThread = new MyTimerThread(1, this);
    connect(myTimerThread, SIGNAL(timeout()),this,SLOT(read_fan_group_data()));
    myTimerThread->start();
}

int SharedFanControlDialog::getFanIndex() const
{
    return fanIndex;
}

void SharedFanControlDialog::setFanIndex(int value)
{
    fanIndex = value;
    ui->fanIndexComboBox->setCurrentIndex(fanIndex);
}

void SharedFanControlDialog::localServerConnected(bool isConnected)
{
    isServerConnected = isConnected;
    if(isConnected)
    {
        StreamPack bpack;
        bpack = {sizeof(StreamPack),4,0,r_AllCacheData,String,0,0,0,0,0,0};
        getFanGroupDataTcpClient->sendRequest(bpack);
    }
}

void SharedFanControlDialog::read_fan_group_data()
{
    getServerConnectStateTcpClient->sendTestConnectRequest();
}

void SharedFanControlDialog::showFanGroupData(QByteArray data)
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

        parseHzSettingData(plcDataMap);
        parseRunCtrData(plcDataMap);
    }
}

void SharedFanControlDialog::parseHzSettingData(QMap<float,QString> dataMap)
{
    uint startAddrss = Global::fanDeviceInfo.Aeration_Fan_Hz_Address;
    uint valueNumber = Global::fanDeviceInfo.Aeration_Fan_Hz_Num;
    QVector<uint> uintValues;
    for(uint i=0; i < valueNumber; i++)
    {
        float address = float(startAddrss + 4 * i);
        uintValues.append(dataMap[address].toUInt());

        if(!Global::currentFanGroupDataMap.contains(address))
        {
            Global::currentFanGroupDataMap.insert(address,dataMap[address]);
        }
        else
        {
            Global::currentFanGroupDataMap[address] = dataMap[address];
        }
    }

    int deviceIndex = ui->fanIndexComboBox->currentIndex();
    DeviceGroupInfo info = Global::getFanDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getFanNodeInfoByName("FER_RF_R");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->outputFreqLabel->setText(dataMap[address]);
}

void SharedFanControlDialog::parseRunCtrData(QMap<float,QString> dataMap)
{
    uint startAddrss = Global::fanDeviceInfo.Runctr_Address;
    uint valueNumber = Global::fanDeviceInfo.Runctr_Num;
    QVector<bool> boolValues;
    for(uint i=0; i < valueNumber; i++)
    {
        uint step = i / 8;
        uint temp = i % 8;
        float index = float(temp) / 10;
        float dictAddress = index + startAddrss + step;
        QVariant tempValue = dataMap[dictAddress];
        boolValues.append(tempValue.toBool());

        if(!Global::currentFanGroupDataMap.contains(dictAddress))
        {
            Global::currentFanGroupDataMap.insert(dictAddress,dataMap[dictAddress]);
        }
        else
        {
            if(Global::currentFanGroupDataMap[dictAddress] != dataMap[dictAddress])
            {
                uint tankIndex = i / Global::fanDeviceInfo.RunCtr_Block_Size;
                QList<QStandardItem *> newItemList;
                QList<QStandardItem *> newSimpleItemList;
                Global::alertIndex += 1;
                QString simpleAlert;

                newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                newItemList.append(new QStandardItem(QString::number(tankIndex+1)));
                if(tempValue.toBool())
                {
                    newItemList.append(new QStandardItem(Global::fanRunCtrDeviceNodes[i % Global::fanValveDeviceInfo.RunCtr_Block_Size].Alert1));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            QString::number(tankIndex+1) + "#" +
                            Global::fanRunCtrDeviceNodes[i % Global::fanValveDeviceInfo.RunCtr_Block_Size].Alert1 + " " +
                            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                }
                else
                {
                    newItemList.append(new QStandardItem(Global::fanRunCtrDeviceNodes[i % Global::fanDeviceInfo.RunCtr_Block_Size].Alert0));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            QString::number(tankIndex+1) + "#" +
                            Global::fanRunCtrDeviceNodes[i % Global::fanDeviceInfo.RunCtr_Block_Size].Alert0 + " " +
                            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

                }
                QStandardItem *simpleAlertItem = new QStandardItem(simpleAlert);
                newSimpleItemList.append(simpleAlertItem);

                if(Identity::getInstance()->getUser() != Q_NULLPTR)
                {
                    newItemList.append(new QStandardItem(Identity::getInstance()->getUser()->getUsername()));
                }
                else
                {
                    newItemList.append(new QStandardItem(""));
                }

                Global::simpleAlertsModel->insertRow(0, newSimpleItemList);
                Global::alertsModel->insertRow(0, newItemList);
                Global::currentFanGroupDataMap[dictAddress] = dataMap[dictAddress];
            }
        }
    }

    int deviceIndex = ui->fanIndexComboBox->currentIndex();
    DeviceGroupInfo info = Global::getFanDeviceGroupInfo(deviceIndex);
    int runctrBlockSize = (info.offset + deviceIndex - info.startIndex)*Global::fanDeviceInfo.RunCtr_Block_Size;

    bool isRunning = false;
    bool isFault = false;
    isRunning = boolValues[runctrBlockSize + Global::getFanNodeInfoByName("FAN_Run_SIGNAL").Offset];
    isFault = boolValues[runctrBlockSize + Global::getFanNodeInfoByName("FAN_Fault_SIGNAL").Offset];
    if(isRunning)
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::green)));
        ui->fanOpenPushButton->setEnabled(false);
        ui->fanStopPushButton->setEnabled(true);
    }
    else
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::red)));
        ui->fanOpenPushButton->setEnabled(true);
        ui->fanStopPushButton->setEnabled(false);
    }
    ui->fanRunStateLabel->setPalette(p);

    if(isFault)
    {
        if(switchFlag)
        {
            p.setBrush(QPalette::Background,QBrush(QColor(Qt::yellow)));
        }
        else
        {
            p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
        }
    }
    else
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
    }
    ui->fanFaultStateLabel->setPalette(p);

    isRunning = boolValues[runctrBlockSize + Global::getFanNodeInfoByName("FAN_SPARE_INUSE").Offset];
    if(isRunning)
    {
       p.setBrush(QPalette::Background,QBrush(QColor(Qt::green)));
       ui->backupFanOpenPushButton->setEnabled(false);
       ui->backupFanStopPushButton->setEnabled(true);
    }
    else
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::red)));
        ui->backupFanOpenPushButton->setEnabled(true);
        ui->backupFanStopPushButton->setEnabled(false);
    }
    ui->backupFanRunStateLabel->setPalette(p);

    fanMode = boolValues[runctrBlockSize + Global::getFanNodeInfoByName("FAN_Auto_BOOL").Offset];
    if(fanMode)
    {
        ui->switchFanControlModePushButton->setIcon(fanAutoControlBg);
    }
    else
    {
        ui->switchFanControlModePushButton->setIcon(fanManualControlBg);
    }

    bool isRemote = boolValues[runctrBlockSize + Global::getFanNodeInfoByName("FAN_Remote_SIGNAL").Offset];
    if(!isRemote)
    {
        ui->fanOpenPushButton->setEnabled(false);
        ui->fanStopPushButton->setEnabled(false);
        ui->backupFanOpenPushButton->setEnabled(false);
        ui->backupFanStopPushButton->setEnabled(false);
    }

    switchFlag = !switchFlag;
}

void SharedFanControlDialog::on_fanOpenPushButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        int deviceIndex = ui->fanIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanDeviceGroupInfo(deviceIndex);

        ushort address = Global::getFanNodeInfoByName("AerationHZ_Fan_HandRun_HzSet").Offset + deviceIndex * 4;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,UInt,address,0,1,0,0,0};
        uint uintData = frequency;
        QVariant uint_var_data = QVariant(uintData);
        setFrequencyTcpClient->abort();
        setFrequencyTcpClient->sendRequestWithResult(bpack,uint_var_data,4);

        ushort offset = Global::getFanNodeInfoByName("FAN_HandStart").Offset / 8;
        ushort index = Global::getFanNodeInfoByName("FAN_HandStart").Offset % 8;
        ushort runctrlByteSize = Global::fanDeviceInfo.RunCtr_Block_Size / 8;
        address = Global::fanDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = true;
        QVariant bool_var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,bool_var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SharedFanControlDialog::on_fanOpenPushButton_released()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        int deviceIndex = ui->fanIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanDeviceGroupInfo(deviceIndex);

        ushort offset = Global::getFanNodeInfoByName("FAN_HandStart").Offset / 8;
        ushort index = Global::getFanNodeInfoByName("FAN_HandStart").Offset % 8;
        ushort runctrlByteSize = Global::fanDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = false;
        QVariant bool_var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,bool_var_data,1);
    }
}

void SharedFanControlDialog::on_switchFanControlModePushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanNodeInfoByName("FAN_Auto_BOOL").Offset / 8;
        ushort index = Global::getFanNodeInfoByName("FAN_Auto_BOOL").Offset % 8;

        int deviceIndex = ui->fanIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = !fanMode;
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

void SharedFanControlDialog::on_outputFreqSlider_valueChanged(int value)
{
    frequency = value;
}

void SharedFanControlDialog::on_fanIndexComboBox_currentIndexChanged(const QString &arg1)
{
    ui->fanIndexLabel->setText(QString::number(ui->fanIndexComboBox->currentIndex()+1));
}

void SharedFanControlDialog::on_fanStopPushButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanNodeInfoByName("FAN_HandStop").Offset / 8;
        ushort index = Global::getFanNodeInfoByName("FAN_HandStop").Offset % 8;

        int deviceIndex = ui->fanIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
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

void SharedFanControlDialog::on_fanStopPushButton_released()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanNodeInfoByName("FAN_HandStop").Offset / 8;
        ushort index = Global::getFanNodeInfoByName("FAN_HandStop").Offset % 8;

        int deviceIndex = ui->fanIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = false;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
}

void SharedFanControlDialog::on_backupFanOpenPushButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanNodeInfoByName("FAN_SPARE_USE").Offset / 8;
        ushort index = Global::getFanNodeInfoByName("FAN_SPARE_USE").Offset % 8;

        int deviceIndex = ui->fanIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
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

void SharedFanControlDialog::on_backupFanOpenPushButton_released()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanNodeInfoByName("FAN_SPARE_USE").Offset / 8;
        ushort index = Global::getFanNodeInfoByName("FAN_SPARE_USE").Offset % 8;

        int deviceIndex = ui->fanIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = false;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
}

void SharedFanControlDialog::on_backupFanStopPushButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanNodeInfoByName("FAN_SPARE_NOUSE").Offset / 8;
        ushort index = Global::getFanNodeInfoByName("FAN_SPARE_NOUSE").Offset % 8;

        int deviceIndex = ui->fanIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
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

void SharedFanControlDialog::on_backupFanStopPushButton_released()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFanNodeInfoByName("FAN_SPARE_NOUSE").Offset / 8;
        ushort index = Global::getFanNodeInfoByName("FAN_SPARE_NOUSE").Offset % 8;

        int deviceIndex = ui->fanIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getFanDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::fanDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::fanDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = false;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
}

void SharedFanControlDialog::on_fanIndexComboBox_currentIndexChanged(int index)
{
    ui->fanIndexLabel->setText(QString::number(index+1));
}
