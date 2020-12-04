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

    /*deviceNode = Global::getYhcNodeInfoByName("Fpj_LeftFront");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_LeftFront_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_RightFront");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_RightFront_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_LeftBehind");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_LeftBehind_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_RightBehind");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_RightBehind_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Lifting_Oc_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Lifting_Oc_Pressure_label->setText(Global::currentYhcDataMap.value(address));*/

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Sweeping_Motor_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Sweeping_Motor_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    bool run;

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_CONTROL_MODE", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_CONTROL_MODE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_CONTROL_MODE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_REMOTE_CONTROL_MODE_1", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_REMOTE_CONTROL_MODE_1_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_REMOTE_CONTROL_MODE_1_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_REMOTE_CONTROL_MODE_2", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_REMOTE_CONTROL_MODE_2_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_REMOTE_CONTROL_MODE_2_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_SWEEPING", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_SWEEPING_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_SWEEPING_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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

    cooler_1_state = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_COOLER_1_RUN", Global::currentYhcDataMap);
    cooler_2_state = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_COOLER_2_RUN", Global::currentYhcDataMap);
    if(cooler_1_state || cooler_2_state)
    {
        ui->FPJ_COOLER_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_COOLER_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

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

    servo_1_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_SERVO_1_FAULT", Global::currentYhcDataMap);
    if(servo_1_fault)
    {
        ui->FPJ_SERVO_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_SERVO_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
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

    spool_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_SPOOL_FAULT", Global::currentYhcDataMap);
    if(spool_fault)
    {
        ui->FPJ_SPOOL_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_SPOOL_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    sweeping_em_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_SWEEPING_EM_FAULT", Global::currentYhcDataMap);
    if(sweeping_em_fault)
    {
        ui->FPJ_SWEEPING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_SWEEPING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    lifting_em_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_LIFTING_EM_FAULT", Global::currentYhcDataMap);
    if(lifting_em_fault)
    {
        ui->FPJ_LIFTING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_LIFTING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    roller_em_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_ROLLER_EM_FAULT", Global::currentYhcDataMap);
    if(roller_em_fault)
    {
        ui->FPJ_ROLLER_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_ROLLER_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    walking_oc_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_WALKING_OC_EM_FAULT", Global::currentYhcDataMap);
    if(roller_em_fault)
    {
        ui->FPJ_WALKING_OC_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_WALKING_OC_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    lifting_oc_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_LIFTING_OC_EM_FAULT", Global::currentYhcDataMap);
    if(roller_em_fault)
    {
        ui->FPJ_LIFTING_OC_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_LIFTING_OC_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    roller_oc_fault = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_ROLLER_OC_EM_FAULT", Global::currentYhcDataMap);
    if(roller_em_fault)
    {
        ui->FPJ_ROLLER_OC_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_ROLLER_OC_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    os_level = Global::getYhcRunctrValueByName(_deviceIndex, "HS_LEVEL", Global::currentYhcDataMap);
    if(os_level)
    {
        ui->HS_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->HS_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
}

void FpjRunStatusDialog::swithState()
{
    if(servo_1_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_SERVO_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_SERVO_1_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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

    if(sweeping_em_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_SWEEPING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_SWEEPING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(lifting_em_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_LIFTING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_LIFTING_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(roller_em_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_ROLLER_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_ROLLER_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(walking_oc_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_WALKING_OC_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_WALKING_OC_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(lifting_oc_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_LIFTING_OC_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_LIFTING_OC_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(roller_oc_fault)
    {
        if(stateFlag)
        {
            ui->FPJ_ROLLER_OC_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_ROLLER_OC_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(os_level)
    {
        if(stateFlag)
        {
            ui->HS_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->HS_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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
