#include "fancontroldialog.h"
#include "ui_fancontroldialog.h"
#include "centerwidget.h"
#include "user.h"
#include "identity.h"
#include "xinyifercontroltabwidget.h"

#include <QMessageBox>
#include <QThreadPool>

FanControlDialog::FanControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FanControlDialog)
{
    ui->setupUi(this);
    //setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowTitle(QStringLiteral("风机控制"));

    msgBox = new QMessageBox(this);

    icoGreen.load("://image/old/FerLEDG.bmp");
    icoYellow.load("://image/old/FerLEDG.bmp");
    icoRed.load("://image/old/FerLEDY.bmp");
    fanAutoControlBg.load("://image/old/Auto.bmp");
    fanManualControlBg.load("://image/old/Mause.bmp");

    for(int i=0;i<Global::ferDeviceInfo.Device_Number;i++)
    {
        ui->fanIndexComboBox->addItem(QStringLiteral("%1#风机").arg(QString::number(i+1)));
    }
    connect(ui->fanIndexComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(fanIndexChanged(int)));

    tcpClient = new TcpClientSocket(this);
    actionTcpClient = new TcpClientSocket(this);
}

FanControlDialog::~FanControlDialog()
{
    delete ui;
}

void FanControlDialog::showEvent(QShowEvent *)
{

}

void FanControlDialog::closeEvent(QCloseEvent *event)
{

}

void FanControlDialog::fanIndexChanged(int index)
{
    tankIndex = index;
    ui->fanIndexLabel->setText(QString("%1#").arg(QString::number(index+1)));
}

int FanControlDialog::getTankIndex() const
{
    return tankIndex;
}

void FanControlDialog::setTankIndex(int value)
{
    tankIndex = value;
    ui->fanIndexLabel->setText(QString::number(tankIndex+1));
    ui->fanIndexComboBox->setCurrentIndex(value);
}

void FanControlDialog::setCombBoxVisible(bool value)
{
    ui->fanIndexComboBox->setVisible(value);
}

void FanControlDialog::on_fanOpenPushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        StreamPack bpack;

        if(frequency == 0)
        {
            msgBox->setText(QStringLiteral("当前风机频率为0，请先设置频率！"));
            msgBox->show();
            return;
        }

        ushort offset = Global::getFermenationNodeInfoByName("FAN_HandStart_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FAN_HandStart_BOOL").Offset % 8;

        info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankIndex - info.startIndex) * runctrlByteSize + offset;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        connect(tcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showFanOpen(QByteArray)));
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void FanControlDialog::localServerConnected(bool isConnected)
{
    if(isConnected)
    {
        QMessageBox messageBox;
        messageBox.setText(QStringLiteral("连接成功"));
        messageBox.exec();
    }
}

void FanControlDialog::on_fanStopPushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getFermenationNodeInfoByName("FAN_HandStop_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FAN_HandStop_BOOL").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void FanControlDialog::on_switchFanModePushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !fanMode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void FanControlDialog::showSwtichFanMode(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QString msg;
    if(bDevice.bErrorCode==1)
    {

    }
}

void FanControlDialog::showFanOpen(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {

    }
}

void FanControlDialog::showFanClose(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {

    }
}

void FanControlDialog::showChangeOutputFrequency(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {

    }
}

void FanControlDialog::getTestResult(int tv)
{
    if(tv > 0)
    {
        QMessageBox messageBox;
        messageBox.setText("get test result");
        messageBox.exec();
    }
}

void FanControlDialog::updateFermentationData(QSet<int> chengedDeviceSet, QMap<float,QString> dataMap)
{
    if(chengedDeviceSet.contains(tankIndex))
    {
        parseFermentationData(dataMap);
        parseFerRunTimeData(dataMap);
        parseFerStepData(dataMap);
        parseFerRunCtrData(dataMap);
    }
}

void FanControlDialog::parseFermentationData(QMap<float,QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_RF_UDI");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    ui->runFreqLabel->setText(dataMap[address]);


    deviceNode = Global::getFermenationNodeInfoByName("FER_HAND_FS");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    frequency = dataMap[address].toInt();

    deviceNode = Global::getFermenationNodeInfoByName("AIR_INPUT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    float value = dataMap.value(address).toFloat();
    ui->airInputLabel->setText(QString::number(value, 'f', 2));

    deviceNode = Global::getFermenationNodeInfoByName("VALVE_OPEN_TIME");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    if(oldValveOpen != dataMap[address])
    {
        oldValveOpen = dataMap[address];
        ui->valveOpenTimeLabel->setText(dataMap[address]);
    }

    //Here only have one value for all devices, so set deviceIndex to const 0.
    deviceNode = Global::getFermenationNodeInfoByName("VALVE_CLOSE_TIME");
    address = deviceNode.Offset + (info.offset + 0 - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    if(oldValveClose != dataMap[address])
    {
        oldValveClose = dataMap[address];
        ui->valveCloseTimeLabel->setText(dataMap[address]);
    }

    /*deviceNode = Global::getFermenationNodeInfoByName("FER_OV_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->ovLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_OI_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->ocLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_OP_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->opLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_OF_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->outputFreqLabel->setText(dataMap[address]);*/
}

void FanControlDialog::parseFerRunTimeData(QMap<float,QString> dataMap)
{

}

void FanControlDialog::parseFerStepData(QMap<float,QString> dataMap)
{

}

void FanControlDialog::parseFerRunCtrData(QMap<float,QString> dataMap)
{
    isFanRun = Global::getFerRunctrValueByName(tankIndex,"FAN_Run_BOOL", dataMap);
    //isFanRemote = Global::getFerRunctrValueByName(tankIndex,"FAN_Remote_BOOL", dataMap);
    fanMode = Global::getFerRunctrValueByName(tankIndex,"FER_Auto_BOOL", dataMap);
    isValveOpened = Global::getFerRunctrValueByName(tankIndex,"AIR_VALVE_STATUS", dataMap);

    if(isValveOpened)
    {
        ui->airValveStateLabel->setStyleSheet("QLabel#airValveStateLabel{background-color: rgb(0, 255, 0);}");
        ui->ValveOpenPushButton->setEnabled(false);
        ui->ValveClosePushButton->setEnabled(true);
    }
    else
    {
        ui->airValveStateLabel->setStyleSheet("QLabel#airValveStateLabel{background-color: rgb(255, 0, 0);}");;
        ui->ValveOpenPushButton->setEnabled(true);
        ui->ValveClosePushButton->setEnabled(false);
    }

    if(isFanRun)
    {
        ui->runStateLabel->setStyleSheet("QLabel#runStateLabel{background-color: rgb(0, 255, 0);}");
        ui->fanOpenPushButton->setEnabled(false);
        ui->fanStopPushButton->setEnabled(true);
    }
    else
    {
        ui->runStateLabel->setStyleSheet("QLabel#runStateLabel{background-color: rgb(255, 0, 0);}");;
        ui->fanOpenPushButton->setEnabled(true);
        ui->fanStopPushButton->setEnabled(false);
    }

    if(fanMode)
    {
        ui->switchFanModePushButton->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchFanModePushButton->setIcon(QIcon(fanManualControlBg));
    }

    isFault = Global::getFerRunctrValueByName(tankIndex,"FAN_FAULT_BOOL", dataMap);
    ui->faultStateLabel->setObjectName("faultstate");
    if(isFault)
    {
        ui->faultStateLabel->setStyleSheet("QLabel#faultstate{background-color: rgb(255, 255, 0);}");
    }
    else
    {
        ui->faultStateLabel->setStyleSheet("QLabel#faultstate{background-color: rgb(255, 255, 255);}");
    }

    isSwitchFault = Global::getFerRunctrValueByName(tankIndex,"FAN_Open_Timeout_BOOL", dataMap);
    if(!isSwitchFault)
    {
        isSwitchFault = Global::getFerRunctrValueByName(tankIndex,"FAN_Close_Timeout_BOOL", dataMap);
    }
    if(isSwitchFault)
    {
        ui->faultStateLabel->setStyleSheet("QLabel#faultstate{background-color: rgb(255, 255, 0);}");
    }

    bool alertState = false;
    ui->alertStateLabel->setObjectName("alertstate");
    if(alertState)
    {
        ui->alertStateLabel->setStyleSheet("QLabel#alertstate{background-color: rgb(0, 255, 0);}");
    }
    else
    {
        ui->alertStateLabel->setStyleSheet("QLabel#alertstate{background-color: rgb(255, 0, 0);}");
    }

    bool ofState = false;
    ui->ofStateLabel->setObjectName("ofState");
    if(ofState)
    {
        ui->ofStateLabel->setStyleSheet("QLabel#ofState{background-color: rgb(0, 255, 0);}");
    }
    else
    {
        ui->ofStateLabel->setStyleSheet("QLabel#ofState{background-color: rgb(255, 0, 0);}");
    }

    bool ovState = false;
    ui->ovStateLabel->setObjectName("ovState");
    if(ovState)
    {
        ui->ovStateLabel->setStyleSheet("QLabel#ovState{background-color: rgb(0, 255, 0);}");
    }
    else
    {
        ui->ovStateLabel->setStyleSheet("QLabel#ovState{background-color: rgb(255, 0, 0);}");
    }

    bool ohState = false;
    ui->ohStateLabel->setObjectName("ohState");
    if(ohState)
    {
        ui->ohStateLabel->setStyleSheet("QLabel#ohState{background-color: rgb(0, 255, 0);}");
    }
    else
    {
        ui->ohStateLabel->setStyleSheet("QLabel#ohState{background-color: rgb(255, 0, 0);}");
    }

    bool otState = false;
    ui->otStateLabel->setObjectName("otState");
    if(otState)
    {
        ui->otStateLabel->setStyleSheet("QLabel#otState{background-color: rgb(0, 255, 0);}");
    }
    else
    {
        ui->otStateLabel->setStyleSheet("QLabel#otState{background-color: rgb(255, 0, 0);}");
    }
}


void FanControlDialog::on_fanIndexComboBox_currentIndexChanged(int index)
{
    tankIndex = index;
    parseFermentationData(Global::currentFermenationDataMap);
    parseFerRunCtrData(Global::currentFermenationDataMap);
}

void FanControlDialog::on_ValveClosePushButton_2_clicked()
{
    close();
}

void FanControlDialog::on_ValveOpenPushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getFermenationNodeInfoByName("AIR_VALVE_OPEN").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("AIR_VALVE_OPEN").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void FanControlDialog::on_ValveClosePushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getFermenationNodeInfoByName("AIR_VALVE_OPEN").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("AIR_VALVE_OPEN").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void FanControlDialog::on_modifyOpenTimeButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        StreamPack bpack;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Int,0,0,1,0,0,0};
        //Length of ushort address and value, plus length of scrc.
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bpack.bDataLength = 1;
        bpack.bStreamLength += (4+2)*bpack.bDataLength + 4;

        QList<ushort> addrs;
        QList<int> values;
        DeviceNode deviceNode = Global::getFermenationNodeInfoByName("VALVE_OPEN_TIME");
        ushort addr = deviceNode.Offset + (info.offset + tankIndex - info.startIndex)
                * Global::getLengthByDataType(deviceNode.DataType);
        addrs.append(addr);
        values.append(ui->valveOpenTimeLabel->text().toInt());

        QByteArray allPackData, SData, crcData;
        QDataStream out(&SData,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out.setByteOrder(QDataStream::LittleEndian);

        allPackData.append((char*)&bpack, sizeof(bpack));

        foreach(ushort item, addrs)
        {
            out << item;
        }

        foreach(int item, values)
        {
            out << item;
        }

        SData.insert(0, allPackData);

        uint scrc = actionTcpClient->StreamLen_CRC32(SData);

        QDataStream out1(&crcData,QIODevice::WriteOnly);
        out1.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out1.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out1.setByteOrder(QDataStream::LittleEndian);
        out1 << scrc;

        SData.append(crcData);

        // Send frequency firstly.
        actionTcpClient->sendRequestWithResults(SData);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void FanControlDialog::on_modifyCloseTimeButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        StreamPack bpack;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Int,0,0,1,0,0,0};
        //Length of ushort address and value, plus length of scrc.
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bpack.bDataLength = 1;
        bpack.bStreamLength += (4+2)*bpack.bDataLength + 4;

        QList<ushort> addrs;
        QList<int> values;
        DeviceNode deviceNode = Global::getFermenationNodeInfoByName("VALVE_CLOSE_TIME");
        ushort addr = deviceNode.Offset;
        addrs.append(addr);
        values.append(ui->valveCloseTimeLabel->text().toInt());

        QByteArray allPackData, SData, crcData;
        QDataStream out(&SData,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out.setByteOrder(QDataStream::LittleEndian);

        allPackData.append((char*)&bpack, sizeof(bpack));

        foreach(ushort item, addrs)
        {
            out << item;
        }

        foreach(int item, values)
        {
            out << item;
        }

        SData.insert(0, allPackData);

        uint scrc = actionTcpClient->StreamLen_CRC32(SData);

        QDataStream out1(&crcData,QIODevice::WriteOnly);
        out1.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out1.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out1.setByteOrder(QDataStream::LittleEndian);
        out1 << scrc;

        SData.append(crcData);

        // Send frequency firstly.
        actionTcpClient->sendRequestWithResults(SData);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void FanControlDialog::on_modifyFrePushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        StreamPack bpack;

        frequencySetting = ui->outputFreqSlider->value();
        if(frequencySetting == 0)
        {
            msgBox->setText(QStringLiteral("风机频率设置为0，请先设置频率！"));
            msgBox->show();
            return;
        }

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Int,0,0,1,0,0,0};
        //Length of ushort address and value, plus length of scrc.
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bpack.bDataLength = 1;
        bpack.bStreamLength += (4+2)*bpack.bDataLength + 4;

        QList<ushort> addrs;
        QList<int> values;
        DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_HAND_FS");
        ushort addr = deviceNode.Offset + (info.offset + tankIndex - info.startIndex)
                * Global::getLengthByDataType(deviceNode.DataType);
        addrs.append(addr);
        values.append(frequencySetting);

        QByteArray allPackData, SData, crcData;
        QDataStream out(&SData,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out.setByteOrder(QDataStream::LittleEndian);

        allPackData.append((char*)&bpack, sizeof(bpack));

        foreach(ushort item, addrs)
        {
            out << item;
        }

        foreach(int item, values)
        {
            out << item;
        }

        SData.insert(0, allPackData);

        uint scrc = actionTcpClient->StreamLen_CRC32(SData);

        QDataStream out1(&crcData,QIODevice::WriteOnly);
        out1.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out1.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out1.setByteOrder(QDataStream::LittleEndian);
        out1 << scrc;

        SData.append(crcData);

        actionTcpClient->sendRequestWithResults(SData);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}
