#include "fpjrunstatusdialog.h"
#include "ui_fpjrunstatusdialog.h"

FpjRunStatusDialog::FpjRunStatusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FpjRunStatusDialog)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showStatus()));

    switchStateTimer = new QTimer(this);
    connect(switchStateTimer, SIGNAL(timeout()), this, SLOT(swithState()));
}

FpjRunStatusDialog::~FpjRunStatusDialog()
{
    delete ui;
}

void FpjRunStatusDialog::showEvent(QShowEvent *)
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

void FpjRunStatusDialog::closeEvent(QCloseEvent *)
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

void FpjRunStatusDialog::showStatus()
{
    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(_deviceIndex);

    DeviceNode deviceNode = Global::getYhcNodeInfoByName("QLJ_Total_Voltage");
    float address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_Total_Voltage_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_Total_Ampere");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_Total_Ampere_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_Hs_Tempture");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_Hs_Tempture_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_Hs_Oil_Level");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_Hs_Oil_Level_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_CP_L_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_CP_L_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_CP_W_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_CP_W_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_MS_MOVE_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_MS_MOVE_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_MS_W_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_MS_W_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_SS_MOVE_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_SS_MOVE_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_SS_W_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_SS_W_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_PS_1_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_PS_1_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_PS_2_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_PS_2_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_SG_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_SG_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_MIXER_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_MIXER_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_Walking_Speed");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_Walking_Speed_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_Left_Track_Encoder_Cycle_Num");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_Left_Track_Encoder_Cycle_Num_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("QLJ_Right_Track_Encoder_Cycle_Num");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->QLJ_Right_Track_Encoder_Cycle_Num_label->setText(Global::currentYhcDataMap.value(address));


    bool run;

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_KP_CONTROL", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_KP_CONTROL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_KP_CONTROL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_REMOTE_CONTROL", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_REMOTE_CONTROL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_REMOTE_CONTROL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_F_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_F_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_F_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_B_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_B_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_B_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_CP_R_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_CP_R_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_CP_R_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_CP_D_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_CP_D_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_CP_D_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_CP_W_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_CP_W_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_CP_W_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_MS_RV_STATE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_MS_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_MS_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_MS_DV_STATE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_MS_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_MS_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_MS_W_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_MS_W_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_MS_W_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_SS_RV_STATE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_SS_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_SS_RV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_SS_DV_STATE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_SS_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_SS_DV_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_SS_W_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_SS_W_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_SS_W_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_PS_1_UP_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_PS_1_UP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_PS_1_UP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_PS_1_DOWN_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_PS_1_DOWN_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_PS_1_DOWN_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_PS_2_UP_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_PS_2_UP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_PS_2_UP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_PS_2_DOWN_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_PS_2_DOWN_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_PS_2_DOWN_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_SG_OPEN_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_SG_OPEN_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_SG_OPEN_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_SG_CLOSE_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_SG_CLOSE_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_SG_CLOSE_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_MIXER_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_MIXER_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_MIXER_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_AM_PUMP_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_AM_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_AM_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }  

    cooler_1_state = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_COOLER_1_RUN", Global::currentYhcDataMap);
    if(cooler_1_state )
    {
        ui->QLJ_COOLER_1_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_COOLER_1_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    cooler_2_state = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_COOLER_2_RUN", Global::currentYhcDataMap);
    if(cooler_2_state )
    {
        ui->QLJ_COOLER_2_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_COOLER_2_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    // IN Place

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_F_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_F_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_F_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_B_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_B_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_B_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_CP_R_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_CP_R_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_CP_R_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_CP_D_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_CP_D_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_CP_D_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_CP_W_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_CP_W_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_CP_W_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_MS_RV_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_MS_RV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_MS_RV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_MS_DV_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_MS_DV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_MS_DV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_MS_W_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_MS_W_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_MS_W_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_SS_RV_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_SS_RV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_SS_RV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_SS_DV_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_SS_DV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_SS_DV_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_SS_W_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_SS_W_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_SS_W_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_PS_1_UP_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_PS_1_UP_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_PS_1_UP_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_PS_1_DOWN_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_PS_1_DOWN_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_PS_1_DOWN_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_PS_2_UP_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_PS_2_UP_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_PS_2_UP_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_PS_2_DOWN_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_PS_2_DOWN_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_PS_2_DOWN_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_SG_OPEN_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_SG_OPEN_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_SG_OPEN_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_SG_CLOSE_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->QLJ_SG_CLOSE_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->QLJ_SG_CLOSE_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    // FAULT

    QLJ_WALKING_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_WALKING_FAULT", Global::currentYhcDataMap);
    if(QLJ_WALKING_FAULT)
    {
        ui->QLJ_WALKING_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_WALKING_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_CP_L_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_CP_L_FAULT", Global::currentYhcDataMap);
    if(QLJ_CP_L_FAULT)
    {
        ui->QLJ_CP_L_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_CP_L_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_CP_W_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_CP_W_FAULT", Global::currentYhcDataMap);
    if(QLJ_CP_W_FAULT)
    {
        ui->QLJ_CP_W_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_CP_W_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_MS_MOVE_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_MS_MOVE_FAULT", Global::currentYhcDataMap);
    if(QLJ_MS_MOVE_FAULT)
    {
        ui->QLJ_MS_MOVE_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_MS_MOVE_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_MS_W_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_MS_W_FAULT", Global::currentYhcDataMap);
    if(QLJ_MS_W_FAULT)
    {
        ui->QLJ_MS_W_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_MS_W_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_SS_MOVE_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_SS_MOVE_FAULT", Global::currentYhcDataMap);
    if(QLJ_SS_MOVE_FAULT)
    {
        ui->QLJ_SS_MOVE_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_SS_MOVE_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_SS_W_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_SS_W_FAULT", Global::currentYhcDataMap);
    if(QLJ_SS_W_FAULT)
    {
        ui->QLJ_SS_W_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_SS_W_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_PS_1_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_PS_1_FAULT", Global::currentYhcDataMap);
    if(QLJ_PS_1_FAULT)
    {
        ui->QLJ_PS_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_PS_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_PS_2_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_PS_2_FAULT", Global::currentYhcDataMap);
    if(QLJ_PS_2_FAULT)
    {
        ui->QLJ_PS_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_PS_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_SG_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_SG_FAULT", Global::currentYhcDataMap);
    if(QLJ_SG_FAULT)
    {
        ui->QLJ_SG_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_SG_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_MIXER_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_MIXER_FAULT", Global::currentYhcDataMap);
    if(QLJ_MIXER_FAULT)
    {
        ui->QLJ_MIXER_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_MIXER_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_AM_PUMP_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_AM_PUMP_FAULT", Global::currentYhcDataMap);
    if(QLJ_AM_PUMP_FAULT)
    {
        ui->QLJ_AM_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_AM_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_COOLER_1_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_COOLER_1_FAULT", Global::currentYhcDataMap);
    if(QLJ_COOLER_1_FAULT)
    {
        ui->QLJ_COOLER_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_COOLER_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_COOLER_2_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_COOLER_2_FAULT", Global::currentYhcDataMap);
    if(QLJ_COOLER_2_FAULT)
    {
        ui->QLJ_COOLER_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_COOLER_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_OT_LEVEL = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_OT_LEVEL", Global::currentYhcDataMap);
    if(QLJ_OT_LEVEL)
    {
        ui->QLJ_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    QLJ_WT_LEVEL = Global::getYhcRunctrValueByName(_deviceIndex, "QLJ_WT_LEVEL", Global::currentYhcDataMap);
    if(QLJ_WT_LEVEL)
    {
        ui->QLJ_WT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->QLJ_WT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
}

void FpjRunStatusDialog::swithState()
{
    if(QLJ_WALKING_FAULT)
    {
        if(stateFlag)
        {
            ui->QLJ_WALKING_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_WALKING_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_CP_L_FAULT)
    {
        if(stateFlag)
        {
            ui->QLJ_CP_L_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_CP_L_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_CP_W_FAULT)
    {
        if(stateFlag)
        {
            ui->QLJ_CP_W_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_CP_W_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_MS_MOVE_FAULT)
    {
        if(stateFlag)
        {
            ui->QLJ_MS_MOVE_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_MS_MOVE_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_MS_W_FAULT)
    {
        if(stateFlag)
        {
            ui->QLJ_MS_W_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_MS_W_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_SS_MOVE_FAULT)
    {
        if(stateFlag)
        {
            ui->QLJ_SS_MOVE_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_SS_MOVE_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_SS_W_FAULT)
    {
        if(stateFlag)
        {
            ui->QLJ_SS_W_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_SS_W_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_PS_1_FAULT)
    {
        if(stateFlag)
        {
            ui->QLJ_PS_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_PS_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_PS_2_FAULT)
    {
        if(stateFlag)
        {
            ui->QLJ_PS_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_PS_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_SG_FAULT)
    {
        if(stateFlag)
        {
            ui->QLJ_SG_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_SG_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_MIXER_FAULT )
    {
        if(stateFlag)
        {
            ui->QLJ_MIXER_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_MIXER_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_AM_PUMP_FAULT)
    {
        if(stateFlag)
        {
            ui->QLJ_AM_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_AM_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_COOLER_1_FAULT)
    {
        if(stateFlag)
        {
            ui->QLJ_COOLER_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_COOLER_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_COOLER_2_FAULT )
    {
        if(stateFlag)
        {
            ui->QLJ_COOLER_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_COOLER_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_OT_LEVEL)
    {
        if(stateFlag)
        {
            ui->QLJ_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(QLJ_WT_LEVEL)
    {
        if(stateFlag)
        {
            ui->QLJ_WT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->QLJ_WT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    stateFlag = !stateFlag;

}

void FpjRunStatusDialog::on_closePushButton1_clicked()
{
    close();
}

void FpjRunStatusDialog::on_closePushButton2_clicked()
{
    close();
}
