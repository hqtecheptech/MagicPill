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
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Yhc_Sp_Oil_Pressure");
    float address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Sp_Oil_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Oil_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Oil_Pressure_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Hs_Oil_Level");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Hs_Oil_Level_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Hs_Tempture");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->Yhc_Hs_Tempture_label->setText(Global::currentYhcDataMap.value(address));


    bool run, coro, inve, uploading;

    run = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_SP_HS_PUMP_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->YHC_SP_HS_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_SP_HS_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_HS_PUMP_RUN", Global::currentYhcDataMap);
    if(run)
    {
        ui->YHC_HS_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_HS_PUMP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_HS_CF_RUN", Global::currentYhcDataMap);
    if(coro)
    {
        ui->YHC_HS_CF_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_HS_CF_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    inve = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_BACKUP_EM_RUN", Global::currentYhcDataMap);
    if(inve)
    {
        ui->YHC_BACKUP_EM_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_BACKUP_EM_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    run = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_CY_OUT", Global::currentYhcDataMap);
    if(run)
    {
        ui->YHC_CY_OUT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_CY_OUT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_CY_IN", Global::currentYhcDataMap);
    if(coro)
    {
        ui->YHC_CY_IN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_CY_IN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_CT_R_STATE", Global::currentYhcDataMap);
    if(coro)
    {
        ui->YHC_CT_R_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_CT_R_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_CT_D_STATE", Global::currentYhcDataMap);
    if(coro)
    {
        ui->YHC_CT_D_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_CT_D_STATE_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_GP_OUT", Global::currentYhcDataMap);
    if(coro)
    {
        ui->YHC_GP_OUT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_GP_OUT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_GP_IN", Global::currentYhcDataMap);
    if(coro)
    {
        ui->YHC_GP_IN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_GP_IN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }

    coro = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_SP_RUN", Global::currentYhcDataMap);
    if(coro)
    {
        ui->YHC_SP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_SP_RUN_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
    }  

    YHC_SP_HS_PUMP_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_SP_HS_PUMP_FAULT", Global::currentYhcDataMap);
    if(!YHC_SP_HS_PUMP_FAULT)
    {
        ui->YHC_SP_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_SP_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }

    YHC_HS_PUMP_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_HS_PUMP_FAULT", Global::currentYhcDataMap);
    if(!YHC_HS_PUMP_FAULT)
    {
        ui->YHC_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }

    YHC_CF_FAULT = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_CF_FAULT", Global::currentYhcDataMap);
    if(!YHC_CF_FAULT)
    {
        ui->YHC_CF_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_CF_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }

    YHC_SP_FILTER_BLOCKING = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_SP_FILTER_BLOCKING", Global::currentYhcDataMap);
    if(!YHC_SP_FILTER_BLOCKING)
    {
        ui->YHC_SP_FILTER_BLOCKING_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_SP_FILTER_BLOCKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }

    YHC_FILTER_BLOCKING = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_FILTER_BLOCKING", Global::currentYhcDataMap);
    if(!YHC_FILTER_BLOCKING)
    {
        ui->YHC_FILTER_BLOCKING_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_FILTER_BLOCKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }

    YHC_OT_LEVEL = Global::getYhcRunctrValueByName(_deviceIndex, "YHC_OT_LEVEL", Global::currentYhcDataMap);
    if(!YHC_OT_LEVEL)
    {
        ui->YHC_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_run.png)");
    }
    else
    {
        ui->YHC_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
    }
}

void YhcRunStatusDialog::swithState()
{
    if(YHC_SP_HS_PUMP_FAULT)
    {
        if(stateFlag)
        {
            ui->YHC_SP_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->YHC_SP_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(YHC_HS_PUMP_FAULT)
    {
        if(stateFlag)
        {
            ui->YHC_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->YHC_HS_PUMP_FAULT_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(YHC_CF_FAULT)
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

    if(YHC_SP_FILTER_BLOCKING)
    {
        if(stateFlag)
        {
            ui->YHC_SP_FILTER_BLOCKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->YHC_SP_FILTER_BLOCKING_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(YHC_FILTER_BLOCKING)
    {
        if(stateFlag)
        {
            ui->YHC_FILTER_BLOCKING_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->YHC_FILTER_BLOCKING_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
        }
    }

    if(YHC_OT_LEVEL)
    {
        if(stateFlag)
        {
            ui->YHC_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_fault.png)");
        }
        else
        {
            ui->YHC_OT_LEVEL_label->setStyleSheet("background-image: url(:/pic/wheel_stop.png)");
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
