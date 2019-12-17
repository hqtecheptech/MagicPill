#include "pipevalve.h"
#include "ui_pipevalve.h"
#include "global.h"
#include "identity.h"

#include <QDebug>

PiPeValve::PiPeValve(QWidget *parent) :
    BasePartWidget(parent),
    ui(new Ui::PiPeValve)
{
    ui->setupUi(this);
    ui->valvePushButton->setFlat(true);
    ui->valvePushButton->setObjectName("valvepushbutton");
    ui->valvePushButton->setStyleSheet("QPushButton#valvepushbutton{border-image:url(:/image/new/thin_vertical_pipe_opened.png)}");

    prefix = "ValveGD";
    myTimerThread = new MyTimerThread(2,this);
    actionTcpclient = new TcpClientSocket(this);
}

PiPeValve::~PiPeValve()
{
    delete ui;
}

bool PiPeValve::getDeviceOpenedState()
{
    return isValveOpen;
}

void PiPeValve::showEvent(QShowEvent *event)
{
    connect(myTimerThread, SIGNAL(timeout()),this,SLOT(updateValveState()));
    myTimerThread->start();
}

void PiPeValve::closeEvent(QCloseEvent *event)
{

}

QString PiPeValve::getPrefix() const
{
    return prefix;
}

void PiPeValve::setPrefix(const QString &value)
{
    prefix = value;
}

bool PiPeValve::getDeviceFaultState() const
{
    return isValveFault;
}

void PiPeValve::setDeviceFaultState(bool value)
{
    isValveFault = value;
}

QString PiPeValve::getName()
{
    return valveName;
}

void PiPeValve::setName(const QString &name)
{
    valveName = name;
    ui->nameLabel->setText(name);
}

QString PiPeValve::getOpendSignalName()
{
    return prefix + "_" + textIndexValue + "_Opened_Signal";
}

QString PiPeValve::getClosedSignalName()
{
    return prefix + "_" + textIndexValue + "_Closed_Signal";
}

QString PiPeValve::getFaultSignalName()
{
    return prefix + "_" + textIndexValue + "_fault_Signal";
}

QString PiPeValve::getTextIndexValue() const
{
    return textIndexValue;
}

void PiPeValve::setTextIndexValue(const QString &value)
{
    textIndexValue = value;
    ui->valveIndexLabel->setText(textIndexValue);
}

void PiPeValve::setDeviceClosedState(bool value)
{
    isValveClose = value;
}

void PiPeValve::setDeviceOpenedState(bool value)
{
    isValveOpen = value;
}

int PiPeValve::getDeviceIndex() const
{
    return valveIndex;
}

void PiPeValve::setDeviceIndex(int value)
{
    valveIndex = value;
}

bool PiPeValve::getDeviceClosedState()
{
    return isValveClose;
}

bool PiPeValve::getDeviceClosingState() const
{
    return isValveClosing;
}

void PiPeValve::setDeviceClosingState(bool value)
{
    isValveClosing = value;
}

bool PiPeValve::getDeviceOpeningState() const
{
    return isValveOpening;
}

void PiPeValve::setDeviceOpeningState(bool value)
{
    isValveOpening = value;
}

bool PiPeValve::getEnable() const
{
    return enable;
}

void PiPeValve::setEnable(bool value)
{
    enable = value;
    ui->valvePushButton->setEnabled(value);
}

void PiPeValve::updateValveState()
{
    if(isValveOpen & !isValveClose)
    {
        ui->valvePushButton->setStyleSheet("QPushButton#valvepushbutton{border-image:url(:/image/new/thin_vertical_pipe_opened.png)}");
    }
    else if(isValveClose & !isValveOpen)
    {
        ui->valvePushButton->setStyleSheet("QPushButton#valvepushbutton{border-image:url(:/image/new/thin_vertical_pipe_closed.png)}");
    }
    else
    {
        ui->valvePushButton->setStyleSheet("QPushButton#valvepushbutton{border-image:url(:/image/new/thin_vertical_pipe_switching.png)}");
    }

    if(isValveFault)
    {
        ui->valvePushButton->setStyleSheet("QPushButton#valvepushbutton{border-image:url(:/image/new/thin_vertical_pipe_fault.png)}");
    }

    switchFlag = !switchFlag;
}

void PiPeValve::on_valvePushButton_pressed()
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

        qDebug() << openValveControlPointName;


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

void PiPeValve::on_valvePushButton_released()
{

}
