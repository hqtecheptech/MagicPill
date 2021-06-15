#include "singledeodorationfan.h"
#include "ui_singledeodorationfan.h"

SingleDeodorationFan::SingleDeodorationFan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleDeodorationFan)
{
    ui->setupUi(this);

    prefix = "Fan";

    ui->indexLabel->setStyleSheet("QLabel {font: 11px;}");
    ui->label->setStyleSheet("QLabel {font: 11px;}");
    ui->nameLabel->setStyleSheet("QLabel {font: 11px;}");

    ui->fanPushButton->setObjectName("fanpushbutton");
    ui->fanPushButton->setStyleSheet("QPushButton#fanpushbutton{border-image:url(:/image/new/thin_fan_opened.png)}");
}

SingleDeodorationFan::~SingleDeodorationFan()
{
    delete ui;
}

QString SingleDeodorationFan::getName() const
{
    return name;
}

void SingleDeodorationFan::setName(const QString &value)
{
    name = value;
}

QString SingleDeodorationFan::getPrefix() const
{
    return prefix;
}

void SingleDeodorationFan::setPrefix(const QString &value)
{
    prefix = value;
}

QString SingleDeodorationFan::getTextIndexValue() const
{
    return textIndexValue;
}

void SingleDeodorationFan::setTextIndexValue(const QString &value)
{
    textIndexValue = value;
    ui->indexLabel->setText(value);
}

bool SingleDeodorationFan::getFanRunState() const
{
    return isFanRun;
}

void SingleDeodorationFan::setFanRunState(bool value)
{
    isFanRun = value;
    if(isFanRun)
    {
        ui->fanPushButton->setStyleSheet("QPushButton#fanpushbutton{border-image:url(:/image/new/thin_fan_opened.png)}");
    }
    else
    {
        ui->fanPushButton->setStyleSheet("QPushButton#fanpushbutton{border-image:url(:/image/new/thin_fan_closed.png)}");
    }
}

bool SingleDeodorationFan::getFanFaultState() const
{
    return isFanFault;
}

void SingleDeodorationFan::setFanFaultState(bool value)
{
    isFanFault = value;
    if(isFanFault)
    {
        ui->fanPushButton->setStyleSheet("QPushButton#fanpushbutton{border-image:url(:/image/new/thin_fan_fault.png)}");
    }
}

int SingleDeodorationFan::getDeviceIndex() const
{
    return deviceIndex;
}

void SingleDeodorationFan::setDeviceIndex(int value)
{
    deviceIndex =  value;
}

bool SingleDeodorationFan::getFanReadyState() const
{
    return isFanReady;
}

void SingleDeodorationFan::setFanReadyState(bool value)
{
    isFanReady = value;
}

QString SingleDeodorationFan::getRunSignalName()
{
    QString temp = prefix + "_" + textIndexValue + "_Run_Signal";
    return prefix + "_" + textIndexValue + "_Run_Signal";
}

QString SingleDeodorationFan::getFaultSignalName()
{
    return prefix + "_" + textIndexValue + "_False_Signal";
}

QString SingleDeodorationFan::getReadySignalName()
{
    return prefix + "_" + textIndexValue + "_Ready_Signal";
}

QString SingleDeodorationFan::getStartCtrName()
{
    return prefix + "_" + textIndexValue + "_Start_Signal";
}

QString SingleDeodorationFan::getStopCtrName()
{
    return prefix + "_" + textIndexValue + "_Stop_Signal";
}
