#include "fftank.h"
#include "ui_fftank.h"
#include "dataformat.h"
#include "global.h"
#include "identity.h"
#include "fancontroldialog.h"

#include <QPalette>
#include <QResizeEvent>
#include <QPixmap>

FFTank::FFTank(QWidget *parent) :
    BaseTankWidget(parent),
    ui(new Ui::FFTank)
{
    ui->setupUi(this);
    ui->fanButton->setFlat(true);
    fanControlDialog = new FanControlDialog(this);
    connect(this, SIGNAL(dataChanged(QSet<int>, QMap<float,QString>)), fanControlDialog, SLOT(updateFermentationData(QSet<int>, QMap<float,QString>)));

    switchStateTimer = new QTimer(this);
    connect(switchStateTimer, SIGNAL(timeout()), this, SLOT(switchState()));

    tankRunBgImg.load("://image/old/nerFer/FerARun.bmp");
    tankRunQBgImg.load("://image/old/nerFer/FerARunBQ.bmp");
    tankStopBgImg.load("://image/old/nerFer/FerAStop.bmp");
    tankRunFanOpenBgImg.load("://image/old/nerFer/FerARunBQ.bmp");
    fanOpenBgImg.load("://image/new/tank_valve_opened3.png");
    fanCloseBgImg.load("://image/new/tank_valve_closed.png");
    fanFaultBgImg.load("://image/new/tank_valve_fault.png");
    ferAutoBgImg.load("://image/old/FerLEDG.bmp");
    ferCustomBgImg.load("://image/old/FerLEDY.bmp");

    updateDeviceState();
}

FFTank::~FFTank()
{
    delete ui;
}

void FFTank::showEvent(QShowEvent *event)
{
    if(!switchStateTimer->isActive())
    {
        switchStateTimer->start(1000);
    }
}

void FFTank::closeEvent(QCloseEvent *event)
{
    if(switchStateTimer->isActive())
    {
        switchStateTimer->stop();
    }
}

int FFTank::getLocation() const
{
    return location;
}

void FFTank::setLocation(int value)
{
    location = value;
}

int FFTank::getTankIndex() const
{
    return tankIndex;
}

void FFTank::setTankIndex(int value)
{
    tankIndex = value;
    ui->tankIndexLabel->setText(QString::number(tankIndex+1));
}

void FFTank::updateDeviceState()
{
    ui->tankFrame->setObjectName("tankframe");
    ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerAStop.bmp)}");

    if(isFerStart)
    {
        ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARun.bmp)}");

        if(isFanOpen)
        {
            ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARunBQ.bmp)}");
        }
    }

    if(!isFanOpen)
    {
        ui->fanButton->setIcon(QIcon(fanCloseBgImg));
    }
    else
    {
        ui->fanButton->setIcon(QIcon(fanOpenBgImg));
    }
}

void FFTank::switchState()
{
    ui->tankFrame->setObjectName("tankframe");
    extraRuntimeAddition++;
    if(isFerStart)
    {
        ui->totalTimeValueLabel->setText(formatLongDateString(latestFerRunTime + extraRuntimeAddition));

        if(isFanOpen)
        {
            ui->runTimeValueLabel->setText(QString::number(latestFerAeTime + extraRuntimeAddition));

            if(switchFlag)
            {
                ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARunBQ.bmp)}");
            }
            else
            {
                ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARun.bmp)}");
            }
        }
        else
        {
            ui->spacTimeValueLabel->setText(QString::number(latestFerStaTime + extraRuntimeAddition));
        }
    }
    else
    {
        ui->runTimeValueLabel->setText(QString::number(0));
        ui->spacTimeValueLabel->setText(QString::number(0));
    }

    if(isFanFault)
    {
        if(switchFlag)
        {
            ui->fanButton->setIcon(QIcon(fanFaultBgImg));
        }
        else
        {
            ui->fanButton->setIcon(QIcon(fanCloseBgImg));
        }
    }

    if(isSwitchFault)
    {
        if(switchFlag)
        {
            ui->fanButton->setIcon(QIcon(fanOpenBgImg));
        }
        else
        {
            ui->fanButton->setIcon(QIcon(fanCloseBgImg));
        }
    }

    switchFlag = !switchFlag;
}

QString FFTank::formatLongDateString(uint value)
{
    int day = value / 86400;
    int hour = (value - day*86400) / 3600;
    int minute = (value - day*86400 - hour*3600) / 60;
    int second = (value - day*86400 - hour*3600 - minute * 60);
    return QStringLiteral("%1天%2时%3分%4秒").arg(QString::number(day)).arg(QString::number(hour)).arg(QString::number(minute)).arg(QString::number(second));
}

void FFTank::updateFermentationData(QSet<int> changedDeviceSet, QMap<float,QString> dataMap)
{
    if(changedDeviceSet.contains(tankIndex))
    {
        parseFermentationData(dataMap);
        parseFerRunTimeData(dataMap);
        //parseFerStepData(dataMap);
        parseFerRunCtrData(dataMap);
        parseFerStartEndTime(dataMap);
        emit dataChanged(changedDeviceSet, dataMap);
    }
}

void FFTank::parseFermentationData(QMap<float,QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_WT_R");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
                            * Global::getLengthByDataType(deviceNode.DataType);
    ui->wtValueLabel->setText(dataMap.value(address));
}

void FFTank::parseFerStartEndTime(QMap<float,QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_START_UDI");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    //qDebug() << "Start time = " << dataMap.value(address);
    uint runtime = dataMap.value(address).toUInt();
    QDateTime dt = QDateTime::fromTime_t(runtime);
    ui->startTimeValueLabel->setText(dt.toString("yyyy-MM-dd hh:mm"));

    deviceNode = Global::getFermenationNodeInfoByName("FER_END_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    //qDebug() << "End time = " << dataMap.value(address);
    runtime = dataMap.value(address).toUInt();
    dt = QDateTime::fromTime_t(runtime);
    ui->endTimeValueLabel->setText(dt.toString("yyyy-MM-dd hh:mm"));
}

void FFTank::parseFerRunTimeData(QMap<float,QString> dataMap)
{
    extraRuntimeAddition = 0;

    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode;
    float address;
    deviceNode = Global::getFermenationNodeInfoByName("FER_TOT_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    latestFerRunTime = dataMap.value(address).toUInt();
    ui->totalTimeValueLabel->setText(formatLongDateString(latestFerRunTime));

    deviceNode = Global::getFermenationNodeInfoByName("FER_AE_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    latestFerAeTime = dataMap.value(address).toUInt();
    ui->runTimeValueLabel->setText(QString::number(latestFerAeTime));

    deviceNode = Global::getFermenationNodeInfoByName("FER_STA_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    latestFerStaTime = dataMap.value(address).toUInt();
    ui->spacTimeValueLabel->setText(QString::number(latestFerStaTime));
}

void FFTank::parseFerStepData(QMap<float,QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_STEPCTR_UI");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    int step = dataMap.value(address).toUInt();
}

void FFTank::parseFerRunCtrData(QMap<float,QString> dataMap)
{
    //isFanRemote = Global::getFerRunctrValueByName(tankIndex,"FAN_Remote_BOOL", dataMap);
    //isFanAuto = Global::getFerRunctrValueByName(tankIndex,"FAN_Auto_BOOL", dataMap);
    isFanOpen = Global::getFerRunctrValueByName(tankIndex,"FAN_Run_BOOL", dataMap);
    isFerStart = Global::getFerRunctrValueByName(tankIndex,"FER_State_BOOL", dataMap);
    //isAeration = Global::getFerRunctrValueByName(tankIndex,"FER_AERATION_BOOL", dataMap);
    isFerAuto = Global::getFerRunctrValueByName(tankIndex,"FER_Auto_BOOL", dataMap);
    isFanFault = Global::getFerRunctrValueByName(tankIndex,"FAN_FAULT_BOOL", dataMap);
    isSwitchFault = Global::getFerRunctrValueByName(tankIndex,"FAN_Open_Timeout_BOOL", dataMap);
    if(!isSwitchFault)
    {
        isSwitchFault = Global::getFerRunctrValueByName(tankIndex,"FAN_Close_Timeout_BOOL", dataMap);
    }

    ui->tankFrame->setObjectName("tankframe");
    ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerAStop.bmp)}");

    if(isFerStart)
    {
        ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARun.bmp)}");

        if(isFanOpen)
        {
            ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARunBQ.bmp)}");
        }
    }

    if(!isFanOpen)
    {
        ui->fanButton->setIcon(QIcon(fanCloseBgImg));
    }
    else
    {
        ui->fanButton->setIcon(QIcon(fanOpenBgImg));
    }

    if(isFanFault)
    {
        ui->fanButton->setIcon(QIcon(fanFaultBgImg));
    }
}

void FFTank::on_fanButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        fanControlDialog->setTankIndex(tankIndex);
        fanControlDialog->setCombBoxVisible(false);
        fanControlDialog->show();
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}
