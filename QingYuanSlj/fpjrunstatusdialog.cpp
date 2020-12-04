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
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("SLJ_Left_Servo_Pulse");
    float address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->SLJ_Left_Servo_Pulse_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("SLJ_Right_Servo_Pulse");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->SLJ_Right_Servo_Pulse_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("SLJ_Total_Voltage");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->SLJ_Total_Voltage_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("SLJ_Total_Ampere");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->SLJ_Total_Ampere_label->setText(Global::currentYhcDataMap.value(address));

    /*deviceNode = Global::getYhcNodeInfoByName("SLJ_LeftFront");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->SLJ_LeftFront_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("SLJ_RightFront");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->SLJ_RightFront_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("SLJ_LeftBehind");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->SLJ_LeftBehind_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("SLJ_RightBehind");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->SLJ_RightBehind_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("SLJ_Lifting_Oc_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->SLJ_Lifting_Oc_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("SLJ_Sweeping_Motor_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->SLJ_Sweeping_Motor_Pressure_label->setText(Global::currentYhcDataMap.value(address));*/

    bool run;

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_CONTROL_MODE", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_CONTROL_MODE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_CONTROL_MODE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_REMOTE_CONTROL_MODE_1", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_REMOTE_CONTROL_MODE_1_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_REMOTE_CONTROL_MODE_1_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_REMOTE_CONTROL_MODE_2", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_REMOTE_CONTROL_MODE_2_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_REMOTE_CONTROL_MODE_2_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_WALKING_F_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_WALKING_F_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_WALKING_F_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_WALKING_B_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_WALKING_B_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_WALKING_B_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_SPOOL_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_SPOOL_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_SPOOL_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_SP_R_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_SP_R_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_SP_R_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_SP_D_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_SP_D_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_SP_D_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_SWEEPING", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_SWEEPING_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_SWEEPING_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_GATE_OPEN_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_GATE_OPEN_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_GATE_OPEN_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_GATE_CLOSE_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_GATE_CLOSE_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_GATE_CLOSE_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    cooler_1_state = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_COOLER_1_RUN", Global::currentYhcDataMap);
    cooler_2_state = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_COOLER_2_RUN", Global::currentYhcDataMap);
    if(cooler_1_state || cooler_2_state)
    {
        ui->SLJ_COOLER_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_COOLER_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_F_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_F_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_F_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_B_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_B_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_B_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_SP_R_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_SP_R_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_SP_R_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_SP_D_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_SP_D_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_SP_D_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_GATE_OPEN_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_GATE_OPEN_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_GATE_OPEN_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_GATE_CLOSE_IN_PLACE", Global::currentYhcDataMap);
    if(run)
    {
        ui->SLJ_GATE_CLOSE_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->SLJ_GATE_CLOSE_IN_PLACE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    servo_1_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_SERVO_1_FAULT", Global::currentYhcDataMap);
    if(servo_1_fault)
    {
        ui->SLJ_SERVO_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_SERVO_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    servo_2_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_SERVO_2_FAULT", Global::currentYhcDataMap);
    if(servo_2_fault)
    {
        ui->SLJ_SERVO_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_SERVO_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    lifting_em_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_LIFTING_EM_FAULT", Global::currentYhcDataMap);
    if(lifting_em_fault)
    {
        ui->SLJ_LIFTING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_LIFTING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    gate_em_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_GATE_EM_FAULT", Global::currentYhcDataMap);
    if(gate_em_fault)
    {
        ui->SLJ_GATE_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_GATE_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    spool_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_SPOOL_FAULT", Global::currentYhcDataMap);
    if(spool_fault)
    {
        ui->SLJ_SPOOL_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_SPOOL_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    sweeping_em_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_SWEEPING_EM_FAULT", Global::currentYhcDataMap);
    if(sweeping_em_fault)
    {
        ui->SLJ_SWEEPING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_SWEEPING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    cooler_1_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_COOLER_1_FAULT", Global::currentYhcDataMap);
    if(cooler_1_fault)
    {
        ui->SLJ_COOLER_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_COOLER_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    cooler_2_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_COOLER_2_FAULT", Global::currentYhcDataMap);
    if(cooler_1_fault)
    {
        ui->SLJ_COOLER_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_COOLER_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    walking_io_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_WIO_FILTER_BLOKING", Global::currentYhcDataMap);
    if(walking_io_fault)
    {
        ui->SLJ_WIO_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_WIO_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    walking_co_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_WCO_FILTER_BLOKING", Global::currentYhcDataMap);
    if(walking_co_fault)
    {
        ui->SLJ_WCO_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_WCO_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    walking_sp_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_SP_FILTER_BLOKING", Global::currentYhcDataMap);
    if(walking_sp_fault)
    {
        ui->SLJ_SP_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_SP_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    walking_gate_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_GATE_FILTER_BLOKING", Global::currentYhcDataMap);
    if(walking_gate_fault)
    {
        ui->SLJ_GATE_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_GATE_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    hsbo_1_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_HSBO_1_FILTER_BLOKING", Global::currentYhcDataMap);
    if(hsbo_1_fault)
    {
        ui->SLJ_HSBO_1_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_HSBO_1_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    hsbo_2_fault = Global::getYhcRunctrValueByName(_deviceIndex, "SLJ_HSBO_2_FILTER_BLOKING", Global::currentYhcDataMap);
    if(hsbo_2_fault)
    {
        ui->SLJ_HSBO_2_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->SLJ_HSBO_2_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    hs_1_level = Global::getYhcRunctrValueByName(_deviceIndex, "HS_1_LEVEL", Global::currentYhcDataMap);
    if(hs_1_level)
    {
        ui->HS_1_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->HS_1_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    hs_2_level = Global::getYhcRunctrValueByName(_deviceIndex, "HS_2_LEVEL", Global::currentYhcDataMap);
    if(hs_2_level)
    {
        ui->HS_2_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->HS_2_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
}

void FpjRunStatusDialog::swithState()
{
    if(servo_1_fault)
    {
        if(stateFlag)
        {
            ui->SLJ_SERVO_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_SERVO_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(servo_2_fault)
    {
        if(stateFlag)
        {
            ui->SLJ_SERVO_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_SERVO_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(lifting_em_fault)
    {
        if(stateFlag)
        {
            ui->SLJ_LIFTING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_LIFTING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(gate_em_fault)
    {
        if(stateFlag)
        {
            ui->SLJ_GATE_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_GATE_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(spool_fault)
    {
        if(stateFlag)
        {
            ui->SLJ_SPOOL_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_SPOOL_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(sweeping_em_fault)
    {
        if(stateFlag)
        {
            ui->SLJ_SWEEPING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_SWEEPING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(cooler_1_fault)
    {
        if(stateFlag)
        {
            ui->SLJ_COOLER_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_COOLER_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(cooler_2_fault)
    {
        if(stateFlag)
        {
            ui->SLJ_COOLER_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_COOLER_2_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(walking_io_fault)
    {
        if(stateFlag)
        {
            ui->SLJ_WIO_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_WIO_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(walking_co_fault)
    {
        if(stateFlag)
        {
            ui->SLJ_WCO_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_WCO_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(walking_sp_fault )
    {
        if(stateFlag)
        {
            ui->SLJ_SP_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_SP_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(walking_gate_fault)
    {
        if(stateFlag)
        {
            ui->SLJ_GATE_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_GATE_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(hsbo_1_fault)
    {
        if(stateFlag)
        {
            ui->SLJ_HSBO_1_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_HSBO_1_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(hsbo_2_fault )
    {
        if(stateFlag)
        {
            ui->SLJ_HSBO_2_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->SLJ_HSBO_2_FILTER_BLOKING_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(hs_1_level)
    {
        if(stateFlag)
        {
            ui->HS_1_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->HS_1_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(hs_2_level)
    {
        if(stateFlag)
        {
            ui->HS_2_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->HS_2_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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
