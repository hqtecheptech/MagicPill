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
    ui->Yhc_Encoder_Cycle_Num_label->setText(Global::currentYhcDataMap[address]);

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Encoder_Speed");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Encoder_Speed_label->setText(Global::currentYhcDataMap[address]);

    bool run, coro, inve, uploading;

    deviceNode = Global::getYhcNodeInfoByName("CURRENT_DEVICE");
    run = Global::getYhcRunctrValueByName(_deviceIndex, "CURRENT_DEVICE", Global::currentYhcDataMap);
    if(run)
    {
        ui->CURRENT_DEVICE_label->setText(deviceNode.Alert1);
    }
    else
    {
        ui->CURRENT_DEVICE_label->setText(deviceNode.Alert0);
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "EM_RUN", Global::currentYhcDataMap);
    Global::getYhcRunctrValueByName(_deviceIndex, "EM_RUN", Global::currentYhcDataMap);
    em_fault = Global::getYhcRunctrValueByName(_deviceIndex, "EM_RUN", Global::currentYhcDataMap);
    if(!em_fault)
    {
        if(run)
        {
            ui->em_status_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
        }
        else
        {
            ui->em_status_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
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

    uploading = Global::getYhcRunctrValueByName(_deviceIndex, "WALKING_UL_VALVE_STATE", Global::currentYhcDataMap);
    if(uploading)
    {
        ui->WALKING_UL_VALVE_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->WALKING_UL_VALVE_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "CT_RV_STATE", Global::currentYhcDataMap);
    if(coro)
    {
        ui->CT_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->CT_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    inve = Global::getYhcRunctrValueByName(_deviceIndex, "CT_DV_STATE", Global::currentYhcDataMap);
    if(inve)
    {
        ui->CT_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->CT_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "HS_HEATING_RUN", Global::currentYhcDataMap);
    heater_fault = Global::getYhcRunctrValueByName(_deviceIndex, "HS_HEATING_FAULT", Global::currentYhcDataMap);
    if(!heater_fault)
    {
        if(run)
        {
            ui->HS_HEATING_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
        }
        else
        {
            ui->HS_HEATING_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
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

    run = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_LEFT_AC_SWITCH", Global::currentYhcDataMap);
    if(run)
    {
        ui->YHC_LEFT_AC_SWITCH_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_LEFT_AC_SWITCH_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_RIGHT_AC_SWITCH", Global::currentYhcDataMap);
    if(run)
    {
        ui->YHC_RIGHT_AC_SWITCH_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_RIGHT_AC_SWITCH_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "CT_R_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->CT_R_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->CT_R_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "CT_D_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->CT_D_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->CT_D_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    hs_level = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_HS_LEVEL", Global::currentYhcDataMap);
    if(!hs_level)
    {
        ui->YHC_HS_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    filter_blocking = Global::getYhcRunctrValueByName(_deviceIndex, "FILTER_BLOCKING", Global::currentYhcDataMap);
    if(!filter_blocking)
    {
        ui->FILTER_BLOCKING_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
}

void YhcRunStatusDialog::swithState()
{
    if(em_fault)
    {
        if(stateFlag)
        {
            ui->em_status_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->em_status_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(heater_fault)
    {
        if(stateFlag)
        {
            ui->HS_HEATING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->HS_HEATING_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(filter_blocking)
    {
        if(stateFlag)
        {
            ui->FILTER_BLOCKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FILTER_BLOCKING_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(!hs_level)
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
