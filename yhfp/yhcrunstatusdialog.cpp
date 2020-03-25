#include "yhcrunstatusdialog.h"
#include "ui_yhcrunstatusdialog.h"

YhcRunStatusDialog::YhcRunStatusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::YhcRunStatusDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showStatus()));
}

YhcRunStatusDialog::~YhcRunStatusDialog()
{
    delete ui;
}

void YhcRunStatusDialog::showStatus()
{
    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(_deviceIndex);
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Yhc_Encoder_Cycle_Num");
    float address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Encoder_Cycle_Num_label->setText(Global::currentYhcDataMap[address]);

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Encoder_Speed");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Encoder_Speed_label->setText(Global::currentYhcDataMap[address]);
}

int YhcRunStatusDialog::deviceIndex() const
{
    return _deviceIndex;
}

void YhcRunStatusDialog::setDeviceIndex(int deviceIndex)
{
    _deviceIndex = deviceIndex;
}

void YhcRunStatusDialog::showEvent(QShowEvent *)
{
    if(!timer->isActive())
    {
        timer->start(2000);
    }
}

void YhcRunStatusDialog::closeEvent(QCloseEvent *)
{
    if(timer->isActive())
    {
        timer->stop();
    }
}
