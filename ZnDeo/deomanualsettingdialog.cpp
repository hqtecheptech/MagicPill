#include "deomanualsettingdialog.h"
#include "ui_deomanualsettingdialog.h"

#include "global.h"
#include "identity.h"

DeoManualSettingDialog::DeoManualSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeoManualSettingDialog)
{
    ui->setupUi(this);

    ui->label_22->setStyleSheet("QLabel {font: bold 11px;}");
    ui->label_24->setStyleSheet("QLabel {font: bold 11px;}");
    ui->label_27->setStyleSheet("QLabel {font: bold 11px;}");
    ui->label_30->setStyleSheet("QLabel {font: bold 11px;}");
    ui->label_42->setStyleSheet("QLabel {font: bold 11px;}");
    ui->label_43->setStyleSheet("QLabel {font: bold 11px;}");
    ui->label_50->setStyleSheet("QLabel {font: bold 11px;}");
    ui->label_51->setStyleSheet("QLabel {font: bold 11px;}");
    ui->label_55->setStyleSheet("QLabel {font: bold 11px;}");
    ui->label_57->setStyleSheet("QLabel {font: bold 11px;}");
    ui->label_60->setStyleSheet("QLabel {font: bold 11px;}");
    ui->label_61->setStyleSheet("QLabel {font: bold 11px;}");

    ui->label_34->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_16->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_17->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_20->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_21->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_36->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_25->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_26->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_28->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_29->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_31->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_33->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_32->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_35->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_37->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_39->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_44->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_46->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_45->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_49->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_54->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_53->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_52->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_56->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_38->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_47->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_59->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_58->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_48->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_63->setStyleSheet("QLabel {font: bold 13px;}");
    ui->label_62->setStyleSheet("QLabel {font: bold 13px;}");

    ui->startFirstFanButton->setStyleSheet("QPushButton {font: bold 15px;}");
    ui->stopFirstFanButton->setStyleSheet("QPushButton {font: bold 15px;}");
    ui->startFirstPumpButton->setStyleSheet("QPushButton {font: bold 15px;}");
    ui->stopFirstPumpButton->setStyleSheet("QPushButton {font: bold 15px;}");
    ui->startSecondPumpButton->setStyleSheet("QPushButton {font: bold 15px;}");
    ui->stopSecondPumpButton->setStyleSheet("QPushButton {font: bold 15px;}");
    ui->startFirstPlPumpButton->setStyleSheet("QPushButton {font: bold 15px;}");
    ui->stopFirstPumpPlButton->setStyleSheet("QPushButton {font: bold 15px;}");
    ui->startSecondPlPumpButton->setStyleSheet("QPushButton {font: bold 15px;}");
    ui->stopSecondPumpPLButton->setStyleSheet("QPushButton {font: bold 15px;}");
    ui->startFirstBwValveButton->setStyleSheet("QPushButton {font: bold 15px;}");
    ui->stopFirstBwValveButton->setStyleSheet("QPushButton {font: bold 15px;}");
    ui->startSecondBwValveButton->setStyleSheet("QPushButton {font: bold 15px;}");
    ui->stopSecondBwValveButton->setStyleSheet("QPushButton {font: bold 15px;}");

    ui->startBwValveButton_2->setStyleSheet("QPushButton {font: bold 15px;}");

    fanAutoControlBg.load("://image/old/Auto.bmp");
    fanManualControlBg.load("://image/old/Mause.bmp");

    msgBox = new QMessageBox(this);
    msgBox->setStyleSheet(
        "QPushButton {"
        " background-color: #89AFDE;"
        " border-style: outset;"
        " border-width: 2px;"
        " border-radius: 10px;"
        " border-color: beige;"
        " font: bold 24px;"
        " min-width: 5em;"
        " min-height:5em;"
        " padding: 20px;"
        "}"
        "QLabel {"
        " min-height:5em;"
        " font:24px;"
        " background-color: #89AFDE;"
        " border-style: outset;"
        " border-width: 2px;"
        " border-radius: 10px;"
        " border-color: beige;"
        " padding: 20px;"
        "}"
    );

    tcpClient = new TcpClientSocket(this);
}

DeoManualSettingDialog::~DeoManualSettingDialog()
{
    delete ui;
}

void DeoManualSettingDialog::updateFermentationData(QSet<int> changedDataSet, QMap<float, QString> dataMap)
{
    if(changedDataSet.contains(deviceIndex))
    {
        parseDeoData(dataMap);
        parseDeoRunCtrData(dataMap);
    }
}

void DeoManualSettingDialog::parseDeoData(QMap<float, QString> dataMap)
{

}

void DeoManualSettingDialog::parseDeoRunCtrData(QMap<float, QString> dataMap)
{
    fanMode = Global::getDeoRunctrValueByName(deviceIndex,"Fan_1_Auto",dataMap);
    if(!fanMode)
    {
        ui->switchFanModePushButton->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchFanModePushButton->setIcon(QIcon(fanManualControlBg));
    }

    fanSbMode = Global::getDeoRunctrValueByName(deviceIndex,"Fan_1_StandBy",dataMap);
    if(!fanSbMode)
    {
        ui->switchFanSbPushButton->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchFanSbPushButton->setIcon(QIcon(fanManualControlBg));
    }

    pump_1_mode = Global::getDeoRunctrValueByName(deviceIndex,"Pump_1_Auto",dataMap);
    if(!pump_1_mode)
    {
        ui->switchPumpModePushButton_1->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchPumpModePushButton_1->setIcon(QIcon(fanManualControlBg));
    }

    pump_1_sb_mode = Global::getDeoRunctrValueByName(deviceIndex,"Pump_1_StandBy",dataMap);
    if(!pump_1_sb_mode)
    {
        ui->switchPumpSbPushButton_1->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchPumpSbPushButton_1->setIcon(QIcon(fanManualControlBg));
    }

    pump_2_mode = Global::getDeoRunctrValueByName(deviceIndex,"Pump_2_Auto",dataMap);
    if(!pump_2_mode)
    {
        ui->switchPumpModePushButton_2->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchPumpModePushButton_2->setIcon(QIcon(fanManualControlBg));
    }

    pump_2_sb_mode = Global::getDeoRunctrValueByName(deviceIndex,"Pump_2_StandBy",dataMap);
    if(!pump_2_sb_mode)
    {
        ui->switchPumpSbPushButton_2->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchPumpSbPushButton_2->setIcon(QIcon(fanManualControlBg));
    }

    pump_pl_1_mode = Global::getDeoRunctrValueByName(deviceIndex,"Pump_PL_1_Auto",dataMap);
    if(!pump_pl_1_mode)
    {
        ui->switchPumpPlModePushButton_1->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchPumpPlModePushButton_1->setIcon(QIcon(fanManualControlBg));
    }

    pump_pl_1_sb_mode = Global::getDeoRunctrValueByName(deviceIndex,"Pump_PL_1_StandBy",dataMap);
    if(!pump_pl_1_sb_mode)
    {
        ui->switchPumpPlSbPushButton_1->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchPumpPlSbPushButton_1->setIcon(QIcon(fanManualControlBg));
    }

    pump_pl_2_mode = Global::getDeoRunctrValueByName(deviceIndex,"Pump_PL_2_Auto",dataMap);
    if(!pump_pl_2_mode)
    {
        ui->switchPumpPlModePushButton_2->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchPumpPlModePushButton_2->setIcon(QIcon(fanManualControlBg));
    }

    pump_pl_2_sb_mode = Global::getDeoRunctrValueByName(deviceIndex,"Pump_PL_2_StandBy",dataMap);
    if(!pump_pl_2_sb_mode)
    {
        ui->switchPumpPlSbPushButton_2->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchPumpPlSbPushButton_2->setIcon(QIcon(fanManualControlBg));
    }

    bw_valve_1_mode = Global::getDeoRunctrValueByName(deviceIndex,"ValveBW_1_Auto",dataMap);
    if(!bw_valve_1_mode)
    {
        ui->switchBwValveModePushButton_1->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchBwValveModePushButton_1->setIcon(QIcon(fanManualControlBg));
    }

    bw_valve_2_mode = Global::getDeoRunctrValueByName(deviceIndex,"ValveBW_2_Auto",dataMap);
    if(!bw_valve_2_mode)
    {
        ui->switchBwValveModePushButton_2->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchBwValveModePushButton_2->setIcon(QIcon(fanManualControlBg));
    }

    /*wtMode = Global::getDeoRunctrValueByName(deviceIndex,"Water_Tank_Remote_Auto_Switch",dataMap);
    if(!wtMode)
    {
        ui->switchWaterTankModePushButton->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchWaterTankModePushButton->setIcon(QIcon(fanManualControlBg));
    }*/

    bool isRun = Global::getDeoRunctrValueByName(deviceIndex,"Fan_1_Run_Signal",dataMap);
    if(isRun)
    {
        ui->startFirstFanButton->setEnabled(false);
        ui->stopFirstFanButton->setEnabled(true);
    }
    else
    {
        ui->startFirstFanButton->setEnabled(true);
        ui->stopFirstFanButton->setEnabled(false);
    }

    /*fan_1_started = Global::getDeoRunctrValueByName(deviceIndex,"Fan_1_Start_Write",dataMap);
    if(fan_1_started)
    {
        ui->startFirstFanButton->setText("关闭");
    }
    else
    {
        ui->startFirstFanButton->setText("启动");
    }

    fan_2_started = Global::getDeoRunctrValueByName(deviceIndex,"Fan_2_Start_Write",dataMap);
    if(fan_2_started)
    {
        ui->startSecondFanButton->setText("关闭");
    }
    else
    {
        ui->startSecondFanButton->setText("启动");
    }

    fan_3_started = Global::getDeoRunctrValueByName(deviceIndex,"Fan_3_Start_Write",dataMap);
    if(fan_3_started)
    {
        ui->startThirdFanButton->setText("关闭");
    }
    else
    {
        ui->startThirdFanButton->setText("启动");
    }*/

    isRun = Global::getDeoRunctrValueByName(deviceIndex,"Pump_1_Run_Signal",dataMap);
    if(isRun)
    {
        ui->startFirstPumpButton->setEnabled(false);
        ui->stopFirstPumpButton->setEnabled(true);
    }
    else
    {
        ui->startFirstPumpButton->setEnabled(true);
        ui->stopFirstPumpButton->setEnabled(false);
    }

    isRun = Global::getDeoRunctrValueByName(deviceIndex,"Pump_2_Run_Signal",dataMap);
    if(isRun)
    {
        ui->startSecondPumpButton->setEnabled(false);
        ui->stopSecondPumpButton->setEnabled(true);
    }
    else
    {
        ui->startSecondPumpButton->setEnabled(true);
        ui->stopSecondPumpButton->setEnabled(false);
    }

    isRun = Global::getDeoRunctrValueByName(deviceIndex,"Pump_PL_1_Run_Signal",dataMap);
    if(isRun)
    {
        ui->startFirstPlPumpButton->setEnabled(false);
        ui->stopFirstPumpPlButton->setEnabled(true);
    }
    else
    {
        ui->startFirstPlPumpButton->setEnabled(true);
        ui->stopFirstPumpPlButton->setEnabled(false);
    }

    isRun = Global::getDeoRunctrValueByName(deviceIndex,"Pump_PL_2_Run_Signal",dataMap);
    if(isRun)
    {
        ui->startSecondPlPumpButton->setEnabled(false);
        ui->stopSecondPumpPLButton->setEnabled(true);
    }
    else
    {
        ui->startSecondPlPumpButton->setEnabled(true);
        ui->stopSecondPumpPLButton->setEnabled(false);
    }

    /*pump_1_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_1_Start_Signal",dataMap);
    if(pump_1_started)
    {
        ui->startFirstPumpButton->setText("关闭");
    }
    else
    {
        ui->startFirstPumpButton->setText("启动");
    }

    pump_2_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_2_Start_Signal",dataMap);
    if(pump_2_started)
    {
        ui->startSecondPumpButton->setText("关闭");
    }
    else
    {
        ui->startSecondPumpButton->setText("启动");
    }

    pump_3_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_3_Start_Signal",dataMap);
    if(pump_3_started)
    {
        ui->startThirdPumpButton->setText("关闭");
    }
    else
    {
        ui->startThirdPumpButton->setText("启动");
    }

    pump_4_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_4_Start_Signal",dataMap);
    if(pump_4_started)
    {
        ui->startForthPumpButton->setText("关闭");
    }
    else
    {
        ui->startForthPumpButton->setText("启动");
    }*/

    /*pump_pl_1_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_PL_1_Start_Signal",dataMap);
    if(pump_pl_1_started)
    {
        ui->startFirstPlPumpButton->setText("关闭");
    }
    else
    {
        ui->startFirstPlPumpButton->setText("启动");
    }

    pump_pl_2_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_PL_2_Start_Signal",dataMap);
    if(pump_pl_2_started)
    {
        ui->startSecondPlPumpButton->setText("关闭");
    }
    else
    {
        ui->startSecondPlPumpButton->setText("启动");
    }

    pump_pl_3_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_PL_3_Start_Signal",dataMap);
    if(pump_pl_3_started)
    {
        ui->startThirdPlPumpButton->setText("关闭");
    }
    else
    {
        ui->startThirdPlPumpButton->setText("启动");
    }

    pump_pl_4_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_PL_4_Start_Signal",dataMap);
    if(pump_pl_4_started)
    {
        ui->startForthPlPumpButton->setText("关闭");
    }
    else
    {
        ui->startForthPlPumpButton->setText("启动");
    }

    pump_pl_5_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_PL_5_Start_Signal",dataMap);
    if(pump_pl_5_started)
    {
        ui->startFifthPlPumpButton->setText("关闭");
    }
    else
    {
        ui->startFifthPlPumpButton->setText("启动");
    }

    pump_pl_6_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_PL_6_Start_Signal",dataMap);
    if(pump_pl_6_started)
    {
        ui->startSixthPlPumpButton->setText("关闭");
    }
    else
    {
        ui->startSixthPlPumpButton->setText("启动");
    }*/

    /*heater_1_started = Global::getDeoRunctrValueByName(deviceIndex,"Heater_1_Start",dataMap);
    if(heater_1_started)
    {
        ui->startFirstHeaterButton->setText("关闭");
    }
    else
    {
        ui->startFirstHeaterButton->setText("启动");
    }

    heater_2_started = Global::getDeoRunctrValueByName(deviceIndex,"Heater_2_Start",dataMap);
    if(heater_2_started)
    {
        ui->startSecondHeaterButton->setText("关闭");
    }
    else
    {
        ui->startSecondHeaterButton->setText("启动");
    }*/

    isRun = Global::getDeoRunctrValueByName(deviceIndex,"ValveBW_1_Run_Signal",dataMap);
    if(isRun)
    {
        ui->startFirstBwValveButton->setEnabled(false);
        ui->stopFirstBwValveButton->setEnabled(true);
    }
    else
    {
        ui->startFirstBwValveButton->setEnabled(true);
        ui->stopFirstBwValveButton->setEnabled(false);
    }

    isRun = Global::getDeoRunctrValueByName(deviceIndex,"ValveBW_2_Run_Signal",dataMap);
    if(isRun)
    {
        ui->startSecondBwValveButton->setEnabled(false);
        ui->stopSecondBwValveButton->setEnabled(true);
    }
    else
    {
        ui->startSecondBwValveButton->setEnabled(true);
        ui->stopSecondBwValveButton->setEnabled(false);
    }

    /*valve_bw_1_started = Global::getDeoRunctrValueByName(deviceIndex,"ValveBW_1_Start",dataMap);
    if(valve_bw_1_started)
    {
        ui->startFirstBwValveButton->setText("关闭");
    }
    else
    {
        ui->startFirstBwValveButton->setText("启动");
    }

    valve_bw_2_started = Global::getDeoRunctrValueByName(deviceIndex,"ValveBW_2_Start",dataMap);
    if(valve_bw_2_started)
    {
        ui->startSecondBwValveButton->setText("关闭");
    }
    else
    {
        ui->startSecondBwValveButton->setText("启动");
    }

    valve_bw_3_started = Global::getDeoRunctrValueByName(deviceIndex,"ValveBW_3_Start",dataMap);
    if(valve_bw_3_started)
    {
        ui->startThirdBwValveButton->setText("关闭");
    }
    else
    {
        ui->startThirdBwValveButton->setText("启动");
    }

    valve_bw_4_started = Global::getDeoRunctrValueByName(deviceIndex,"ValveBW_4_Start",dataMap);
    if(valve_bw_4_started)
    {
        ui->startForthBwValveButton->setText("关闭");
    }
    else
    {
        ui->startForthBwValveButton->setText("启动");
    }

    pump_am_1_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_Am_1_Start_Signal",dataMap);
    if(pump_am_1_started)
    {
        ui->startFirstAmPumpButton->setText("关闭");
    }
    else
    {
        ui->startFirstAmPumpButton->setText("启动");
    }

    pump_am_2_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_Am_2_Start_Signal",dataMap);
    if(pump_am_2_started)
    {
        ui->startSecondAmPumpButton->setText("关闭");
    }
    else
    {
        ui->startSecondAmPumpButton->setText("启动");
    }*/
}

void DeoManualSettingDialog::on_startSecondFanButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Fan_2_Start_Write").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Fan_2_Start_Write").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !fan_2_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startFirstPumpButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_1_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_1_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_1_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startSecondPumpButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_2_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_2_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_2_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startFirstHeaterButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Heater_1_Start").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Heater_1_Start").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !heater_1_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startSecondHeaterButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Heater_2_Start").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Heater_2_Start").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !heater_2_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startBwValveButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_1_Start").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_1_Start").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !valve_bw_1_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startBwValveButton_2_clicked()
{
    close();
}

void DeoManualSettingDialog::on_switchFanModePushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Fan_1_Auto").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Fan_1_Auto").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !fanMode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void DeoManualSettingDialog::on_switchWaterTankModePushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Water_Tank_Remote_Auto_Switch").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Water_Tank_Remote_Auto_Switch").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !wtMode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void DeoManualSettingDialog::on_startThirdFanButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Fan_3_Start_Write").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Fan_3_Start_Write").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !fan_3_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startThirdPumpButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_3_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_3_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_3_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startForthPumpButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_4_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_4_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_4_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startSecondBwValveButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_2_Start").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_2_Start").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !valve_bw_2_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startFirstBwValveButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_1_Start").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_1_Start").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !valve_bw_1_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startThirdBwValveButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_3_Start").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_3_Start").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !valve_bw_3_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startForthBwValveButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_4_Start").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_4_Start").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !valve_bw_4_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startThirdPlPumpButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_3_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_3_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_pl_3_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startForthPlPumpButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_4_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_4_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_pl_4_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startFifthPlPumpButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_5_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_5_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_pl_5_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startSixthPlPumpButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_6_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_6_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_pl_6_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startFirstAmPumpButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_Am_1_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_Am_1_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_am_1_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startSecondAmPumpButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_Am_2_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_Am_2_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_am_2_started;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startFirstFanButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Fan_1_Start_Write").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Fan_1_Start_Write").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;

        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startFirstFanButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Fan_1_Start_Write").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Fan_1_Start_Write").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }*/
}

void DeoManualSettingDialog::on_stopFirstFanButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Fan_1_Stop_Write").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Fan_1_Stop_Write").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_stopFirstFanButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Fan_1_Stop_Write").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Fan_1_Stop_Write").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }*/
}

void DeoManualSettingDialog::on_startFirstPumpButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_1_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_1_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startFirstPumpButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_1_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_1_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }*/
}

void DeoManualSettingDialog::on_stopFirstPumpButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_1_Stop_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_1_Stop_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_stopFirstPumpButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_1_Stop_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_1_Stop_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }*/
}

void DeoManualSettingDialog::on_startSecondPumpButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_2_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_2_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startSecondPumpButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_2_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_2_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }*/
}

void DeoManualSettingDialog::on_startFirstPlPumpButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_1_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_1_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startFirstPlPumpButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_1_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_1_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }*/
}

void DeoManualSettingDialog::on_stopFirstPumpPlButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_1_Stop_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_1_Stop_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_stopFirstPumpPlButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_1_Stop_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_1_Stop_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }*/
}

void DeoManualSettingDialog::on_startSecondPlPumpButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_2_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_2_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startSecondPlPumpButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_2_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_2_Start_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }*/
}

void DeoManualSettingDialog::on_stopSecondPumpPLButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_2_Stop_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_2_Stop_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_stopSecondPumpPLButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_2_Stop_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_2_Stop_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }*/
}

void DeoManualSettingDialog::on_startFirstBwValveButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_1_Start").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_1_Start").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startFirstBwValveButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_1_Start").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_1_Start").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }*/
}

void DeoManualSettingDialog::on_stopFirstBwValveButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_1_Stop").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_1_Stop").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_stopFirstBwValveButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_1_Stop").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_1_Stop").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }*/
}

void DeoManualSettingDialog::on_startSecondBwValveButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_2_Start").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_2_Start").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_startSecondBwValveButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_2_Start").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_2_Start").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }*/
}

void DeoManualSettingDialog::on_stopSecondBwValveButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_2_Stop").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_2_Stop").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_stopSecondBwValveButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_2_Stop").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_2_Stop").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }*/
}

void DeoManualSettingDialog::on_switchPumpPlSbPushButton_1_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_1_StandBy").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_1_StandBy").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_pl_1_sb_mode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void DeoManualSettingDialog::on_switchPumpPlSbPushButton_2_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_2_StandBy").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_2_StandBy").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_pl_2_sb_mode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void DeoManualSettingDialog::on_switchPumpPlModePushButton_1_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_1_Auto").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_1_Auto").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_pl_1_mode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void DeoManualSettingDialog::on_switchPumpPlModePushButton_2_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_PL_2_Auto").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_PL_2_Auto").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_pl_2_mode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void DeoManualSettingDialog::on_switchFanSbPushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Fan_1_StandBy").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Fan_1_StandBy").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !fanSbMode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void DeoManualSettingDialog::on_switchPumpModePushButton_1_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_1_Auto").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_1_Auto").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_1_mode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void DeoManualSettingDialog::on_switchPumpModePushButton_2_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_2_Auto").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_2_Auto").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_2_mode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void DeoManualSettingDialog::on_switchPumpSbPushButton_1_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_1_StandBy").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_1_StandBy").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_1_sb_mode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void DeoManualSettingDialog::on_switchPumpSbPushButton_2_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_2_StandBy").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_2_StandBy").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !pump_2_sb_mode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void DeoManualSettingDialog::on_switchBwValveModePushButton_1_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_1_Auto").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_1_Auto").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !bw_valve_1_mode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void DeoManualSettingDialog::on_switchBwValveModePushButton_2_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("ValveBW_2_Auto").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("ValveBW_2_Auto").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = !bw_valve_2_mode;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
    }
}

void DeoManualSettingDialog::on_stopSecondPumpButton_released()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_2_Stop_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_2_Stop_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}

void DeoManualSettingDialog::on_stopSecondPumpButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getDeodorationNodeInfoByName("Pump_2_Stop_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Pump_2_Stop_Signal").Offset % 8;

        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (info.offset + deviceIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox->setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox->show();
        close();
    }
}
