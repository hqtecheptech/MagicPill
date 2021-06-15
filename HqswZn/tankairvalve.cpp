#include "tankairvalve.h"
#include "ui_tankairvalve.h"
#include "global.h"
#include "identity.h"

TankAirValve::TankAirValve(QWidget *parent) :
    BasePartWidget(parent),
    ui(new Ui::TankAirValve)
{
    ui->setupUi(this);
    ui->valvePushButton->setFlat(true);
    ui->valvePushButton->setObjectName("valvepushbutton");
    ui->valvePushButton->setStyleSheet("QPushButton#valvepushbutton{border-image:url(:/image/new/thin_horizontal_pipe_opened.png)}");

    prefix = "ValveFJ";
    myTimerThread = new MyTimerThread(2,this);
    actionTcpclient = new TcpClientSocket(this);
}

TankAirValve::~TankAirValve()
{
    delete ui;
}

void TankAirValve::setDeviceOpenedState(bool value)
{
    isValveOpen = value;
}

void TankAirValve::setDeviceClosedState(bool value)
{
    isValveClose = value;
}

void TankAirValve::showEvent(QShowEvent *event)
{
    connect(myTimerThread, SIGNAL(timeout()),this,SLOT(updateValveState()));
    myTimerThread->start();
}

void TankAirValve::closeEvent(QCloseEvent *event)
{

}

QString TankAirValve::getOpendSignalName()
{
    return prefix + "_" + textIndexValue + "_Opened_Signal";
}

QString TankAirValve::getClosedSignalName()
{
    return prefix + "_" + textIndexValue + "_Closed_Signal";
}

QString TankAirValve::getFaultSignalName()
{
    return prefix + "_" + textIndexValue + "_fault_Signal";
}

bool TankAirValve::getEnable() const
{
    return enable;
}

void TankAirValve::setEnable(bool value)
{
    enable = value;
    ui->valvePushButton->setEnabled(value);
}

QString TankAirValve::getName()
{
    return valveName;
}

void TankAirValve::setName(const QString &name)
{
    valveName = name;
    ui->nameLabel->setText(name);
}

bool TankAirValve::getDeviceClosedState() const
{
    return isValveClose;
}

bool TankAirValve::getDeviceFaultState() const
{
    return isValveFault;
}

void TankAirValve::setDeviceFaultState(bool value)
{
    isValveFault = value;
}

bool TankAirValve::getDeviceClosingState() const
{
    return isValveClosing;
}

void TankAirValve::setDeviceClosingState(bool value)
{
    isValveClosing = value;
}

bool TankAirValve::getDeviceOpeningState() const
{
    return isValveOpening;
}

void TankAirValve::setDeviceOpeningState(bool value)
{
    isValveOpening = value;
}

int TankAirValve::getDeviceIndex() const
{
    return deviceIndex;
}

void TankAirValve::setDeviceIndex(int value)
{
    deviceIndex = value;
}

QString TankAirValve::getPrefix() const
{
    return prefix;
}

void TankAirValve::setPrefix(const QString &value)
{
    prefix = value;
}

bool TankAirValve::getDeviceOpenedState() const
{
    return isValveOpen;
}

QString TankAirValve::getTextIndexValue() const
{
    return textIndexValue;
}

void TankAirValve::setTextIndexValue(const QString &value)
{
    textIndexValue = value;
    ui->valveIndexLabel->setText(textIndexValue);
}

void TankAirValve::updateValveState()
{
    ui->valvePushButton->setStyleSheet("QPushButton#valvepushbutton{border-image:url(:/image/new/thin_horizontal_pipe_opened.png)}");
    /*if(isValveOpen)
    {
        ui->valvePushButton->setStyleSheet("QPushButton#valvepushbutton{border-image:url(:/image/new/thin_horizontal_pipe_opened.png)}");
    }
    else if(isValveClose)
    {
        ui->valvePushButton->setStyleSheet("QPushButton#valvepushbutton{border-image:url(:/image/new/thin_horizontal_pipe_closed.png)}");
    }
    else
    {
        ui->valvePushButton->setStyleSheet("QPushButton#valvepushbutton{border-image:url(:/image/new/thin_horizontal_pipe_switching.png)}");
    }

    if(isValveFault)
    {
        ui->valvePushButton->setStyleSheet("QPushButton#valvepushbutton{border-image:url(:/image/new/thin_horizontal_pipe_fault.png)}");
    }*/

    switchFlag = !switchFlag;
}

void TankAirValve::on_valvePushButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        if(!isValveOpen && !isValveClose)
        {
            openValveControlPointName = prefix + "_" + textIndexValue + "_Start_Signal";
        }

        if(isValveOpening || isValveClosing)
        {
            return;
        }

        if(isValveClose)
        {
            openValveControlPointName = prefix + "_" + textIndexValue + "_Start_Signal";
        }
        if(isValveOpen)
        {
            openValveControlPointName = prefix + "_" + textIndexValue + "_Shut_Signal";
        }
        ushort offset = Global::getDeodorationNodeInfoByName(openValveControlPointName).Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName(openValveControlPointName).Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
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

void TankAirValve::on_valvePushButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName(openValveControlPointName).Offset / 8; 
        ushort index = Global::getDeodorationNodeInfoByName(openValveControlPointName).Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + deviceIndex * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = false;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }*/
}
