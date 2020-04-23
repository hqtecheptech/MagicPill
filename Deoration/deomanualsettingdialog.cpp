#include "deomanualsettingdialog.h"
#include "ui_deomanualsettingdialog.h"

#include "global.h"
#include "identity.h"

DeoManualSettingDialog::DeoManualSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeoManualSettingDialog)
{
    ui->setupUi(this);

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

void DeoManualSettingDialog::on_startFirstFanButton_clicked()
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
        bool data = !fan_1_started;
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

void DeoManualSettingDialog::parseDeoData(QMap<float, QString> dataMap)
{

}

void DeoManualSettingDialog::parseDeoRunCtrData(QMap<float, QString> dataMap)
{
    fanMode = Global::getDeoRunctrValueByName(deviceIndex,"Fan_Remote_Auto_Switch",dataMap);
    if(!fanMode)
    {
        ui->switchFanModePushButton->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchFanModePushButton->setIcon(QIcon(fanManualControlBg));
    }

    wtMode = Global::getDeoRunctrValueByName(deviceIndex,"Water_Tank_Remote_Auto_Switch",dataMap);
    if(!wtMode)
    {
        ui->switchWaterTankModePushButton->setIcon(QIcon(fanAutoControlBg));
    }
    else
    {
        ui->switchWaterTankModePushButton->setIcon(QIcon(fanManualControlBg));
    }

    fan_1_started = Global::getDeoRunctrValueByName(deviceIndex,"Fan_1_Start_Write",dataMap);
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

    pump_1_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_1_Start_Signal",dataMap);
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

    pump_pl_1_started = Global::getDeoRunctrValueByName(deviceIndex,"Pump_PL_1_Start_Signal",dataMap);
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

    heater_1_started = Global::getDeoRunctrValueByName(deviceIndex,"Heater_1_Start",dataMap);
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
    }

    valve_bw_started = Global::getDeoRunctrValueByName(deviceIndex,"ValveBW_1_Start",dataMap);
    if(valve_bw_started)
    {
        ui->startBwValveButton->setText("关闭");
    }
    else
    {
        ui->startBwValveButton->setText("启动");
    }
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

void DeoManualSettingDialog::on_startFirstPlPumpButton_clicked()
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
        bool data = !pump_pl_1_started;
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

void DeoManualSettingDialog::on_startSecondPlPumpButton_clicked()
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
        bool data = !pump_pl_2_started;
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
        bool data = !valve_bw_started;
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

        ushort offset = Global::getDeodorationNodeInfoByName("Fan_Remote_Auto_Switch").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Fan_Remote_Auto_Switch").Offset % 8;

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
