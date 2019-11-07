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

void FanControlDialog::on_outputFreqSlider_valueChanged(int value)
{
    frequency = value;
}

void FanControlDialog::on_fanOpenPushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getFermenationNodeInfoByName("FAN_HandStart_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FAN_HandStart_BOOL").Offset % 8;

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
        connect(tcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showFanOpen(QByteArray)));
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
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

        ushort offset = Global::getFermenationNodeInfoByName("FAN_HandStart_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FAN_HandStart_BOOL").Offset % 8;

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
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
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
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
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
    /*int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_RF_R");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->runFreqLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_OV_R");
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

    ui->runStateLabel->setObjectName("runstate");
    if(isFanRun)
    {
        ui->runStateLabel->setStyleSheet("QLabel#runstate{background-color: rgb(0, 255, 0);}");
        ui->fanOpenPushButton->setEnabled(false);
        ui->fanStopPushButton->setEnabled(true);
    }
    else
    {
        ui->runStateLabel->setStyleSheet("QLabel#runstate{background-color: rgb(255, 0, 0);}");;
        ui->fanOpenPushButton->setEnabled(true);
        ui->fanStopPushButton->setEnabled(false);
    }

    //if(!isFanRemote)
    //{
        //ui->fanOpenPushButton->setEnabled(false);
        //ui->fanStopPushButton->setEnabled(false);
    //}
    //else
    //{
        if(isFanRun)
        {
            ui->fanOpenPushButton->setEnabled(false);
            ui->fanStopPushButton->setEnabled(true);
        }
        else
        {
            ui->fanOpenPushButton->setEnabled(true);
            ui->fanStopPushButton->setEnabled(false);
        }
    //}

    if(fanMode)
    {
        ui->switchFanModePushButton->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchFanModePushButton->setIcon(QIcon(fanManualControlBg));
    }

    bool faultState = Global::getFerRunctrValueByName(tankIndex,"FAN_FAULT_BOOL", dataMap);
    ui->faultStateLabel->setObjectName("faultstate");
    if(faultState)
    {
        ui->faultStateLabel->setStyleSheet("QLabel#faultstate{background-color: rgb(0, 255, 0);}");
    }
    else
    {
        ui->faultStateLabel->setStyleSheet("QLabel#faultstate{background-color: rgb(255, 0, 0);}");
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
