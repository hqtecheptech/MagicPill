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
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Fpj_Upload_Pressure");
    float address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Upload_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Cp_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Cp_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Hs_1_Tempture");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Hs_1_Tempture_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Hs_1_Oil_Level");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Fpj_Hs_1_Oil_Level_label->setText(Global::currentYhcDataMap.value(address));

    bool run;

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_WSD_EM_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_WSD_EM_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_WSD_EM_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_UPLOAD_HS_PUMP_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_UPLOAD_HS_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_UPLOAD_HS_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_CP_HS_PUMP_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_CP_HS_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_CP_HS_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_WALKING_HS_PUMP_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_WALKING_HS_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_WALKING_HS_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_HS_PUMP_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_HS_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_HS_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_HS_CF_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_HS_CF_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_HS_CF_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_HEATER_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_HEATER_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_HEATER_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_UPLOAD_R_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_UPLOAD_R_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_UPLOAD_R_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_UPLOAD_D_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_UPLOAD_D_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_UPLOAD_D_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }  


    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_CP_R_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_CP_R_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_CP_R_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_CP_D_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_CP_D_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_CP_D_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_WOT_PUSH_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_WOT_PUSH_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_WOT_PUSH_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_WOT_PULL_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_WOT_PULL_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_WOT_PULL_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_WS_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_WS_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_WS_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_WL_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_WL_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_WL_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_UPLOADER_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_UPLOADER_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_UPLOADER_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_CP_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->FPJ_CP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->FPJ_CP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    // Fault Signal

    FPJ_WALKING_SD_EM_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_WALKING_SD_EM_FAULT", Global::currentYhcDataMap);
    if(FPJ_WALKING_SD_EM_FAULT)
    {
        ui->FPJ_WALKING_SD_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_WALKING_SD_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    FPJ_UPLOAD_HS_PUMP_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_UPLOAD_HS_PUMP_FAULT", Global::currentYhcDataMap);
    if(FPJ_UPLOAD_HS_PUMP_FAULT)
    {
        ui->FPJ_UPLOAD_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_UPLOAD_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    FPJ_CP_HS_PUMP_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_CP_HS_PUMP_FAULT", Global::currentYhcDataMap);
    if(FPJ_CP_HS_PUMP_FAULT)
    {
        ui->FPJ_CP_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_CP_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    FPJ_WALKING_HS_PUMP_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_WALKING_HS_PUMP_FAULT", Global::currentYhcDataMap);
    if(FPJ_WALKING_HS_PUMP_FAULT)
    {
        ui->FPJ_WALKING_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_WALKING_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    FPJ_SPOOL_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_SPOOL_FAULT", Global::currentYhcDataMap);
    if(FPJ_SPOOL_FAULT)
    {
        ui->FPJ_SPOOL_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_SPOOL_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    FPJ_HS_CF_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_HS_CF_FAULT", Global::currentYhcDataMap);
    if(FPJ_HS_CF_FAULT)
    {
        ui->FPJ_HS_CF_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_HS_CF_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    FPJ_HEATER_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_HEATER_FAULT", Global::currentYhcDataMap);
    if(FPJ_HEATER_FAULT)
    {
        ui->FPJ_HEATER_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_HEATER_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    FPJ_UFB_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_UFB_FAULT", Global::currentYhcDataMap);
    if(FPJ_UFB_FAULT)
    {
        ui->FPJ_UFB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_UFB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    FPJ_CPFB_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_CPFB_FAULT", Global::currentYhcDataMap);
    if(FPJ_CPFB_FAULT)
    {
        ui->FPJ_CPFB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_CPFB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    FPJ_FB_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_FB_FAULT", Global::currentYhcDataMap);
    if(FPJ_FB_FAULT)
    {
        ui->FPJ_FB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_FB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }   

    FPJ_WHFB_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_WHFB_FAULT", Global::currentYhcDataMap);
    if(FPJ_WHFB_FAULT)
    {
        ui->FPJ_WHFB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_WHFB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }

    FPJ_OT_LEVEL = Global::getYhcRunctrValueByName(_deviceIndex, "FPJ_OT_LEVEL", Global::currentYhcDataMap);
    if(FPJ_OT_LEVEL)
    {
        ui->FPJ_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
    else
    {
        ui->FPJ_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
}

void FpjRunStatusDialog::swithState()
{
    if(FPJ_WALKING_SD_EM_FAULT)
    {
        if(stateFlag)
        {
            ui->FPJ_WALKING_SD_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_WALKING_SD_EM_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(FPJ_UPLOAD_HS_PUMP_FAULT)
    {
        if(stateFlag)
        {
            ui->FPJ_UPLOAD_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_UPLOAD_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(FPJ_CP_HS_PUMP_FAULT)
    {
        if(stateFlag)
        {
            ui->FPJ_CP_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_CP_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(FPJ_WALKING_HS_PUMP_FAULT)
    {
        if(stateFlag)
        {
            ui->FPJ_WALKING_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_WALKING_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(FPJ_SPOOL_FAULT)
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

    if(FPJ_HS_CF_FAULT)
    {
        if(stateFlag)
        {
            ui->FPJ_HS_CF_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_HS_CF_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(FPJ_HEATER_FAULT)
    {
        if(stateFlag)
        {
            ui->FPJ_HEATER_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_HEATER_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(FPJ_UFB_FAULT)
    {
        if(stateFlag)
        {
            ui->FPJ_UFB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_UFB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(FPJ_CPFB_FAULT)
    {
        if(stateFlag)
        {
            ui->FPJ_CPFB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_CPFB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(FPJ_FB_FAULT)
    {
        if(stateFlag)
        {
            ui->FPJ_FB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_FB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(FPJ_OT_LEVEL)
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

    if(FPJ_WHFB_FAULT)
    {
        if(stateFlag)
        {
            ui->FPJ_WHFB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->FPJ_WHFB_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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
