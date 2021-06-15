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

    switchStateTimer = new QTimer(this);
    connect(switchStateTimer, SIGNAL(timeout()), this, SLOT(swithState()));
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
    ui->Yhc_Encoder_Cycle_Num_label->setText(Global::currentYhcDataMap.value(address));

    /*deviceNode = Global::getYhcNodeInfoByName("Yhc_Encoder_Speed");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Encoder_Speed_label->setText(Global::currentYhcDataMap.value(address));*/

    bool run, coro, inve, uploading;

    run = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_REMOTE_CONTROL", Global::currentYhcDataMap);
    if(run)
    {
        ui->YHC_REMOTE_CONTROL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_REMOTE_CONTROL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_KP_SLJ_CONTROL", Global::currentYhcDataMap);
    if(run)
    {
        ui->YHC_KP_SLJ_CONTROL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_KP_SLJ_CONTROL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_CONTROL_MODE", Global::currentYhcDataMap);
    if(run)
    {
        ui->YHC_CONTROL_MODE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_CONTROL_MODE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "WALKING_CV_STATE", Global::currentYhcDataMap);
    if(coro)
    {
        ui->WALKING_CV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->WALKING_CV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    inve = Global::getYhcRunctrValueByName(_deviceIndex, "WALKING_IV_STATE", Global::currentYhcDataMap);
    if(inve)
    {
        ui->WALKING_IV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->WALKING_IV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "CF_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->CF_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->CF_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "HS_F_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->HS_F_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->HS_F_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "HS_B_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->HS_B_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->HS_B_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    em_fault = Global::getYhcRunctrValueByName(_deviceIndex, "EM_FAULT", Global::currentYhcDataMap);
    if(!em_fault)
    {
        ui->EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }

    heater_fault = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_CF_FAULT", Global::currentYhcDataMap);
    if(!heater_fault)
    {
        ui->YHC_CF_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_CF_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }

    oc_fault = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_OC_FAULT", Global::currentYhcDataMap);
    if(!oc_fault)
    {
        ui->YHC_OC_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_OC_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }


    hs_level = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_HS_LEVEL", Global::currentYhcDataMap);
    if(!hs_level)
    {
        ui->YHC_HS_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_HS_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
}

void YhcRunStatusDialog::swithState()
{
    if(em_fault)
    {
        if(stateFlag)
        {
            ui->EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(heater_fault)
    {
        if(stateFlag)
        {
            ui->YHC_CF_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->YHC_CF_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(oc_fault)
    {
        if(stateFlag)
        {
            ui->YHC_OC_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->YHC_OC_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(hs_level)
    {
        if(stateFlag)
        {
            ui->YHC_HS_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->YHC_HS_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    stateFlag = !stateFlag;
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
        timer->start(1000);
    }

    if(!switchStateTimer->isActive())
    {
        switchStateTimer->start(300);
    }
}

void YhcRunStatusDialog::closeEvent(QCloseEvent *)
{
    if(timer->isActive())
    {
        timer->stop();
    }

    if(switchStateTimer->isActive())
    {
        switchStateTimer->stop();
    }
}

void YhcRunStatusDialog::on_closePushButton1_clicked()
{
    close();
}

void YhcRunStatusDialog::on_closePushButton2_clicked()
{
    close();
}
