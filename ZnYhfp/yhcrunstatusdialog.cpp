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
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Yhc_Encoder_1_Cycle_Num");
    float address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Encoder_1_Cycle_Num_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Encoder_2_Cycle_Num");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Encoder_2_Cycle_Num_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Walking_Speed");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Walking_Speed_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Total_Voltage");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Total_Voltage_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Total_Ampere");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Total_Ampere_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Hs_Oil_Level");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Hs_Oil_Level_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Hs_Tempture");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Hs_Tempture_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Hs_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Hs_Pressure_label->setText(Global::currentYhcDataMap.value(address));

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

    run = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_KP_CONTROL", Global::currentYhcDataMap);
    if(run)
    {
        ui->YHC_KP_CONTROL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_KP_CONTROL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "CT_RV_STATE", Global::currentYhcDataMap);
    if(coro)
    {
        ui->CT_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->CT_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "CT_DV_STATE", Global::currentYhcDataMap);
    if(coro)
    {
        ui->CT_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->CT_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "SS_RV_STATE", Global::currentYhcDataMap);
    if(coro)
    {
        ui->SS_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SS_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "SS_DV_STATE", Global::currentYhcDataMap);
    if(coro)
    {
        ui->SS_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SS_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "GP_RV_STATE", Global::currentYhcDataMap);
    if(coro)
    {
        ui->GP_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->GP_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "GP_DV_STATE", Global::currentYhcDataMap);
    if(coro)
    {
        ui->GP_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->GP_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "RP_RV_STATE", Global::currentYhcDataMap);
    if(coro)
    {
        ui->RP_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->RP_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "RP_DV_STATE", Global::currentYhcDataMap);
    if(coro)
    {
        ui->RP_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->RP_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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

    run = Global::getYhcRunctrValueByName(_deviceIndex, "CT_RV_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->CT_RV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->CT_RV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "CT_DV_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->CT_DV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->CT_DV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SS_RV_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->SS_RV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SS_RV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SS_DV_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->SS_DV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SS_DV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "GP_RV_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->GP_RV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->GP_RV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "GP_DV_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->GP_DV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->GP_DV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "RP_RV_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->RP_RV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->RP_RV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "RP_DV_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->RP_DV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->RP_DV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_NO_POWER", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_NO_POWER_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_NO_POWER_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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

    cf_fault = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_CF_FAULT", Global::currentYhcDataMap);
    if(!cf_fault)
    {
        ui->YHC_CF_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_CF_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }

    ct_fault = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_CT_FAULT", Global::currentYhcDataMap);
    if(!ct_fault)
    {
        ui->YHC_CT_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_CT_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }

    ss_fault = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_SS_FAULT", Global::currentYhcDataMap);
    if(!ss_fault)
    {
        ui->YHC_SS_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_SS_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }

    gp_fault = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_GP_FAULT", Global::currentYhcDataMap);
    if(!gp_fault)
    {
        ui->YHC_GP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_GP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }

    rp_fault = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_RP_FAULT", Global::currentYhcDataMap);
    if(!rp_fault)
    {
        ui->YHC_RP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_RP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
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

    if(cf_fault)
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

    if(ct_fault)
    {
        if(stateFlag)
        {
            ui->YHC_CT_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->YHC_CT_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(ss_fault)
    {
        if(stateFlag)
        {
            ui->YHC_SS_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->YHC_SS_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(gp_fault)
    {
        if(stateFlag)
        {
            ui->YHC_GP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->YHC_GP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(rp_fault)
    {
        if(stateFlag)
        {
            ui->YHC_RP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->YHC_RP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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
