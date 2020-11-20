#include "deodorationfan.h"
#include "ui_deodorationfan.h"

DeodorationFan::DeodorationFan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeodorationFan)
{
    ui->setupUi(this);
    valveFront = new TankAirValve(this);
    valveBack = new TankAirValve(this);
    valveFront->setEnable(false);
    valveBack->setEnable(false);
    ui->verticalLayout_1->addWidget(valveFront);
    ui->verticalLayout_2->addWidget(valveBack);
    fanRotationTimer = new QTimer(this);
    connect(fanRotationTimer, SIGNAL(timeout()), this, SLOT(rotateFan()));
    fanNegBgImg.load("://image/fan_neg.png");
    fanPosBgImg.load("://image/fan_pos.png");
    fanOpenedBgImg.load("://image/new/green_fan_opened.png");
    fanClosedBgImg.load("://image/new/red_fan_closed.png");
    fanFaultBgImg.load("://image/new/yellow_fan_fault.png");
    ui->fanRotationLabel->setPixmap(fanNegBgImg);
}

DeodorationFan::~DeodorationFan()
{
    delete ui;
}

void DeodorationFan::showEvent(QShowEvent *event)
{
    fanRotationTimer->start(1000);
}

void DeodorationFan::closeEvent(QCloseEvent *event)
{
    fanRotationTimer->stop();
}

void DeodorationFan::rotateFan()
{
    if(faultState)
    {
        ui->fanRotationLabel->setText(QStringLiteral("故障"));
        ui->fan_label->setPixmap(fanFaultBgImg);
        if(faultFlag)
        {
            ui->fanRotationLabel->setStyleSheet("background-color: rgb(255, 0, 0)");
        }
        else
        {
            ui->fanRotationLabel->setStyleSheet("background-color: rgb(255, 255, 255)");
        }
        faultFlag = !faultFlag;
    }
    else
    {
        if(isFanRunning)
        {
            ui->fan_label->setPixmap(fanOpenedBgImg);
            if(fanAngleFlag)
            {
                ui->fanRotationLabel->setPixmap(fanPosBgImg);
            }
            else
            {
                ui->fanRotationLabel->setPixmap(fanNegBgImg);
            }
            fanAngleFlag = !fanAngleFlag;
        }
        else
        {
            ui->fan_label->setPixmap(fanClosedBgImg);
            ui->fanRotationLabel->setPixmap(fanPosBgImg);
        }
    }
}

void DeodorationFan::SetFrontValveState(bool state)
{
    valveFront->setDeviceOpenedState(state);
}

void DeodorationFan::SetBackValveState(bool state)
{
    valveBack->setDeviceOpenedState(state);
}

void DeodorationFan::SetFanState(bool state)
{
    isFanRunning = state;
}

bool DeodorationFan::getFaultState() const
{
    return faultState;
}

void DeodorationFan::setFaultState(bool value)
{
    faultState = value;
}

int DeodorationFan::getFanIndex() const
{
    return fanIndex;
}

void DeodorationFan::setFanIndex(int value)
{
    fanIndex = value;
    ui->fanIndexLabel->setText(QString::number(fanIndex));
}
