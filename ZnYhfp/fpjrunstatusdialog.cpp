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
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Fpj_Left_Servo_Pulse");
    float address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Left_Servo_Pulse_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Right_Servo_Pulse");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Right_Servo_Pulse_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Total_Voltage");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Total_Voltage_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Total_Ampere");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Total_Ampere_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Hs_1_Tempture");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Hs_1_Tempture_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Hs_1_Oil_Level");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Hs_1_Oil_Level_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Left_Roller_Rotation_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Left_Roller_Rotation_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Right_Roller_Rotation_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Right_Roller_Rotation_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Left_Walking_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Left_Walking_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Right_Walking_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Right_Walking_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Sweeping_Motor_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Sweeping_Motor_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Roller_Lifting_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Roller_Lifting_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Roller_Tilt_Angle");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Roller_Tilt_Angle_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_RightBottom");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_RightBottom_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_RightMiddle");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_RightMiddle_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_RightTop");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_RightTop_label->setText(Global::currentYhcDataMap.value(address));

    bool run;

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_REMOTE_CONTROL_MODE", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_REMOTE_CONTROL_MODE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_REMOTE_CONTROL_MODE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_PANEL_CONTROL_MODE", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_PANEL_CONTROL_MODE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_PANEL_CONTROL_MODE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_YHC_CONTROL_MODE", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_YHC_CONTROL_MODE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_YHC_CONTROL_MODE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_ROLLER_F_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_ROLLER_F_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_ROLLER_F_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_ROLLER_B_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_ROLLER_B_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_ROLLER_B_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_ROLLER_CORO_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_ROLLER_CORO_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_ROLLER_CORO_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_ROLLER_INVE_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_ROLLER_INVE_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_ROLLER_INVE_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_SPOOL_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_SPOOL_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_SPOOL_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_ROLLER_R_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_ROLLER_R_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_ROLLER_R_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_ROLLER_D_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_ROLLER_D_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_ROLLER_D_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_SWEEPING_R_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_SWEEPING_R_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_SWEEPING_R_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_SWEEPING_D_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_SWEEPING_D_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_SWEEPING_D_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_SWEEPING_W_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_SWEEPING_W_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_SWEEPING_W_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }  

    cooler_1_state = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_COOLER_1_RUN", Global::currentYhcDataMap);
    if(cooler_1_state)
    {
        ui->FPJ_COOLER_1_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_COOLER_1_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    cooler_2_state = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_COOLER_2_RUN", Global::currentYhcDataMap);
    if(cooler_2_state)
    {
        ui->FPJ_COOLER_2_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_COOLER_2_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_PL_PUMP_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_PL_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_PL_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_NUB_PUMP_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_NUB_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_NUB_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }


    // In-place Signal

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_F_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_F_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_F_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_B_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_B_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_B_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_ROLLER_R_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_ROLLER_R_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_ROLLER_R_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_ROLLER_D_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_ROLLER_D_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_ROLLER_D_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    // Fault Signal

    walking_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_WALKING_FAULT", Global::currentYhcDataMap);
    if(walking_fault)
    {
        ui->FPJ_WALKING_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_WALKING_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    roller_rt_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_ROLLER_RT_FAULT", Global::currentYhcDataMap);
    if(roller_rt_fault)
    {
        ui->FPJ_ROLLER_RT_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_ROLLER_RT_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    roller_lifting_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_ROLLER_LIFTING_EM_FAULT", Global::currentYhcDataMap);
    if(roller_lifting_fault)
    {
        ui->FPJ_ROLLER_LIFTING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_ROLLER_LIFTING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    sweeping_lifting_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_SWEEPING_LIFTING_FAULT", Global::currentYhcDataMap);
    if(sweeping_lifting_fault)
    {
        ui->FPJ_SWEEPING_LIFTING_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_SWEEPING_LIFTING_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    sweeping_rt_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_SWEEPING_RT_FAULT", Global::currentYhcDataMap);
    if(sweeping_rt_fault)
    {
        ui->FPJ_SWEEPING_RT_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_SWEEPING_RT_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    spool_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_SPOOL_FAULT", Global::currentYhcDataMap);
    if(spool_fault)
    {
        ui->FPJ_SPOOL_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_SPOOL_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    cooler_1_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_COOLER_1_FAULT", Global::currentYhcDataMap);
    if(cooler_1_fault)
    {
        ui->FPJ_COOLER_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_COOLER_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    cooler_2_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_COOLER_2_FAULT", Global::currentYhcDataMap);
    if(cooler_2_fault)
    {
        ui->FPJ_COOLER_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_COOLER_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    fpj_pl_pump_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_PL_PUMP_FAULT", Global::currentYhcDataMap);
    if(fpj_pl_pump_fault)
    {
        ui->FPJ_PL_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_PL_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    fpj_nub_pump_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_NUB_PUMP_FAULT", Global::currentYhcDataMap);
    if(fpj_nub_pump_fault)
    {
        ui->FPJ_NUB_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_NUB_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    fpj_ot_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_OT_LEVEL", Global::currentYhcDataMap);
    if(fpj_ot_fault)
    {
        ui->FPJ_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    fpj_wt_level = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_WT_LEVEL", Global::currentYhcDataMap);
    if(fpj_wt_level)
    {
        ui->FPJ_WT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_WT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
}

void FpjRunStatusDialog::swithState()
{
    if(walking_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_WALKING_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_WALKING_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(roller_rt_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_ROLLER_RT_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_ROLLER_RT_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(roller_lifting_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_ROLLER_LIFTING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_ROLLER_LIFTING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(sweeping_lifting_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_SWEEPING_LIFTING_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_SWEEPING_LIFTING_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(sweeping_rt_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_SWEEPING_RT_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_SWEEPING_RT_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(spool_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_SPOOL_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_SPOOL_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(cooler_1_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_COOLER_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_COOLER_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(cooler_2_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_COOLER_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_COOLER_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(fpj_pl_pump_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_PL_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_PL_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(fpj_nub_pump_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_NUB_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_NUB_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(fpj_ot_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(fpj_wt_level)
    {
        if(stateFlag)
        {
            ui->FPJ_WT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_WT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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
