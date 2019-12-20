#include "fercontroldialog.h"
#include "ui_fercontroldialog.h"
#include "dataformat.h"
#include "centerwidget.h"
#include "global.h"
#include "tcpclientsocket.h"
#include "identity.h"
#include "renhuaifercontroltabpagewidget.h"
#include "xinyifercontroltabwidget.h"

FerControlDialog::FerControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FerControlDialog)
{
    ui->setupUi(this);
    //setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowTitle(QStringLiteral("发酵控制"));

    icoGreen.load("://image/old/FerLEDG.bmp");
    icoYellow.load("://image/old/FerLEDG.bmp");
    icoRed.load("://image/old/FerLEDY.bmp");
    ui->ferTotalRunTimeProgressBar->setRange(0,86400*30);

    for(int i=0;i<Global::ferDeviceInfo.Device_Number;i++)
    {
        ui->tankIndexComboBox->addItem(QStringLiteral("%1#发酵槽").arg(QString::number(i+1)));
    }

    /*for(int j=0;j<4;j++)
    {
        ui->runStepComboBox->addItem(QString::number(j+1));
    }*/
    connect(ui->tankIndexComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(tankIndexChanged(int)));
    connect(ui->runStepComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(stepIndexChanged(int)));

    customFerSettingDialog = new CustomFerSettingDialog(this);

    tcpClient = new TcpClientSocket(this);
    tcpClient1 = new TcpClientSocket(this);
}

FerControlDialog::~FerControlDialog()
{
    delete ui;
}

void FerControlDialog::showEvent(QShowEvent *)
{
    setFerRunSteps();
}

void FerControlDialog::closeEvent(QCloseEvent *event)
{

}

void FerControlDialog::tankIndexChanged(int index)
{
    tankIndex = index;
    ui->tankIndexLabel->setText(QString("%1#").arg(QString::number(index+1)));
}

void FerControlDialog::stepIndexChanged(int index)
{
    newStep = index+1;
}

void FerControlDialog::timerEvent( QTimerEvent *event )
{

}

int FerControlDialog::getTankIndex() const
{
    return tankIndex;
}

void FerControlDialog::setTankIndex(int value)
{
    tankIndex = value;
}

void FerControlDialog::showSetFerAuto(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {
        StreamPack bpack;
        ushort offset = Global::getFermenationNodeInfoByName("FER_Startctr_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FER_Startctr_BOOL").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankIndex - info.startIndex) * runctrlByteSize + offset;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Bool,address,index,1,0,0,0};
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient1->abort();
        disconnect(tcpClient1,0,0,0);
        connect(tcpClient1, SIGNAL(updateClients(QByteArray)),this,SLOT(showFerStart(QByteArray)));
        tcpClient1->sendRequestWithResult(bpack,var_data,1);
    }
}

void FerControlDialog::showFerStart(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode == 1)
    {

    }
}

void FerControlDialog::showFerStop(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QString msg;
    if(bDevice.bErrorCode==1)
    {

    }
}

void FerControlDialog::showStepChange(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QString msg;
    if(bDevice.bErrorCode==1)
    {
        currentStep = newStep;
        ui->runStateStepLabel->setText(QString::number(newStep));
    }
    else
    {
        ui->runStepComboBox->setCurrentIndex(currentStep-1);
    }
}

void FerControlDialog::on_customFerButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        customFerSettingDialog->setTankLocation(ui->tankIndexComboBox->currentIndex());
        customFerSettingDialog->setRunTime(handRunTime);
        customFerSettingDialog->setSpaceTime(handSpaceTime);
        customFerSettingDialog->setFrequency(handFrequency);
        customFerSettingDialog->show();
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void FerControlDialog::on_startFerButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        StreamPack bpack;
        ushort offset = Global::getFermenationNodeInfoByName("FER_Startctr_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FER_Startctr_BOOL").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankIndex - info.startIndex) * runctrlByteSize + offset;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Bool,address,index,1,0,0,0};
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient1->abort();
        disconnect(tcpClient1,0,0,0);
        connect(tcpClient1, SIGNAL(updateClients(QByteArray)),this,SLOT(showFerStart(QByteArray)));
        tcpClient1->sendRequestWithResult(bpack,var_data,1);

        /*QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (tankIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        connect(tcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showSetFerAuto(QByteArray)));
        tcpClient->sendRequestWithResult(bpack,var_data,1);*/
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void FerControlDialog::on_startFerButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset % 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + tankIndex * 4 + offset;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        connect(tcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showSetFerAuto(QByteArray)));
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }*/
}

void FerControlDialog::on_endFerButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        StreamPack bpack;

        //Stop auto fermentation.
        ushort offset = Global::getFermenationNodeInfoByName("FER_Stopctr_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FER_Stopctr_BOOL").Offset % 8;
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankIndex - info.startIndex) * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Bool,address,index,1,0,stime,etime};
        bool data = true;
        QVariant var_data = QVariant(data);
        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        connect(tcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showFerStop(QByteArray)));
        tcpClient->sendRequestWithResult(bpack,var_data,1);

        //Stop manual aeration.
        offset = Global::getFermenationNodeInfoByName("Aeration_Stop").Offset / 8;
        index = Global::getFermenationNodeInfoByName("Aeration_Stop").Offset % 8;
        runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        address = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankIndex - info.startIndex) * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Bool,address,index,1,0,stime,etime};
        data = true;
        var_data = QVariant(data);
        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        connect(tcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showFerStop(QByteArray)));
        tcpClient->sendRequestWithResult(bpack,var_data,1);

        /*QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFermenationNodeInfoByName("FER_STOPCTR_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FER_STOPCTR_BOOL").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (tankIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        connect(tcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showFerStop(QByteArray)));
        tcpClient->sendRequestWithResult(bpack,var_data,1);*/
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void FerControlDialog::on_endFerButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFermenationNodeInfoByName("FER_STOPCTR_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FER_STOPCTR_BOOL").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (tankIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        connect(tcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showFerStop(QByteArray)));
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }*/
}

void FerControlDialog::updateFermentationData(QSet<int> changedDeviceSet, QMap<float,QString> dataMap)
{
    if(changedDeviceSet.contains(tankIndex))
    {
        parseFermentationData(dataMap);
        parseFerRunTimeData(dataMap);
        parseFerStepData(dataMap);
        parseFerRunCtrData(dataMap);
        parseFerStartEndTime(dataMap);
    }
}

void FerControlDialog::parseFermentationData(QMap<float,QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_WT_R");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    ui->weightTemptureLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_HT_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            *  Global::getLengthByDataType(deviceNode.DataType);
    ui->highTemptureLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_MT_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->midTemptureLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_LT_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->lowTemptureLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_ET_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    ui->envTemptureLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_HM_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            *  Global::getLengthByDataType(deviceNode.DataType);
    ui->envhumidityLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_OX_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->oxLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_H2S_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->co2Label->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_NH3_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->nh3Label->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_VOC_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->vocLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_CO2_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->vocLabel->setText(dataMap[address]);

    /*deviceNode = Global::getFermenationNodeInfoByName("FER_RO_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->coRateLabel->setText(dataMap[address]);*/
}

void FerControlDialog::parseFerRunTimeData(QMap<float,QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode;
    float address;
    uint runtime;

    deviceNode = Global::getFermenationNodeInfoByName("FER_TOT_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    runtime = dataMap[address].toUInt();
    ui->ferTotalTimeLabel->setText(formatLongDateString(runtime));
    ui->ferTotalRunTimeProgressBar->setValue(runtime);

    deviceNode = Global::getFermenationNodeInfoByName("FER_CURRENT_STEP_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    runtime = dataMap[address].toUInt();
    ui->currentStepTimeLabel->setText(formatLongDateString(runtime));

    /*deviceNode = Global::getFermenationNodeInfoByName("FER_STEP1_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    runtime = dataMap[address].toUInt();
    ui->firstStepTimeLabel->setText(formatLongDateString(runtime));

    deviceNode = Global::getFermenationNodeInfoByName("FER_STEP2_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    runtime = dataMap[address].toUInt();
    ui->SecondStepTimeLabel->setText(formatLongDateString(runtime));

    deviceNode = Global::getFermenationNodeInfoByName("FER_STEP3_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    runtime = dataMap[address].toUInt();
    ui->thirdStepTimeLabel->setText(formatLongDateString(runtime));

    deviceNode = Global::getFermenationNodeInfoByName("FER_STEP4_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    runtime = dataMap[address].toUInt();
    ui->fouthStepTimeLabel->setText(formatLongDateString(runtime));*/

    deviceNode = Global::getFermenationNodeInfoByName("FER_AE_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    runtime = dataMap[address].toUInt();
    ui->aerationTotalTimeLabel->setText(formatLongDateString(runtime));

    deviceNode = Global::getFermenationNodeInfoByName("FER_STA_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    runtime = dataMap[address].toUInt();
    ui->stillTotalTimeLabel->setText(formatLongDateString(runtime));

    deviceNode = Global::getFermenationNodeInfoByName("FER_Hand_RunTime");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    handRunTime = dataMap[address].toInt() / 60;
    ui->handRunTimeValueLabel->setText(QString::number(handRunTime));

    deviceNode = Global::getFermenationNodeInfoByName("FER_Hand_SpaceTime");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    handSpaceTime = dataMap[address].toInt() / 60;
    ui->handSpaceTimeValeLabel->setText(QString::number(handSpaceTime));

    deviceNode = Global::getFermenationNodeInfoByName("FER_HAND_FSP");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    handFrequency = dataMap[address].toInt();
    ui->handFrequencyValeLabel->setText(QString::number(handFrequency));


    /*deviceNode = Global::getFermenationNodeInfoByName("FER_START_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    runtime = dataMap[address].toUInt();
    QDateTime dt = QDateTime::fromTime_t(runtime);
    ui->ferStartTimeLabel->setText(dt.toString("yyyy-MM-dd hh:mm:ss"));

    deviceNode = Global::getFermenationNodeInfoByName("FER_END_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    runtime = dataMap[address].toUInt();
    qDebug() << "ff tank endtime = " << runtime;
    dt = QDateTime::fromTime_t(runtime);
    ui->ferEndTimeLabel->setText(dt.toString("yyyy-MM-dd hh:mm:ss"));*/
}

void FerControlDialog::parseFerStepData(QMap<float,QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_STEPCTR_UI");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    int step = dataMap[address].toUInt();
    ui->runStateStepLabel->setText(QString::number(step));
}

void FerControlDialog::parseFerStartEndTime(QMap<float,QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_START_UDI");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) *
            Global::getLengthByDataType(deviceNode.DataType);
    uint runtime = dataMap[address].toUInt();
    QDateTime dt = QDateTime::fromTime_t(runtime);
    ui->ferStartTimeLabel->setText(dt.toString("yyyy-MM-dd hh:mm"));

    deviceNode = Global::getFermenationNodeInfoByName("FER_END_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) *
            Global::getLengthByDataType(deviceNode.DataType);
    runtime = dataMap[address].toUInt();
    dt = QDateTime::fromTime_t(runtime);
    ui->ferEndTimeLabel->setText(dt.toString("yyyy-MM-dd hh:mm"));
}

void FerControlDialog::parseFerRunCtrData(QMap<float,QString> dataMap)
{
    //isFanRemote = Global::getFerRunctrValueByName(tankIndex,"FAN_Remote_BOOL", dataMap);
    isFanAuto = Global::getFerRunctrValueByName(tankIndex,"FER_Auto_BOOL", dataMap);
    isFanRun = Global::getFerRunctrValueByName(tankIndex,"FAN_Run_BOOL", dataMap);
    //isFerPaused = Global::getFerRunctrValueByName(tankIndex,"FER_Pause", dataMap);
    isFerStarted = Global::getFerRunctrValueByName(tankIndex,"FER_State_BOOL", dataMap);
    if(isFerStarted)
    {
        ui->ferStateLabel->setText(QStringLiteral("发酵中..."));
        if(isFanRun)
        {
            ui->aerationStateLabel->setText(QStringLiteral("曝气中..."));
        }
        else
        {
            ui->aerationStateLabel->setText(QStringLiteral("曝气终止"));
        }
        /*ui->pauseFerButton->setEnabled(true);
        if(isFerPaused)
        {
            ui->pauseFerButton->setText(QStringLiteral("暂停发酵"));
        }
        else
        {
            ui->pauseFerButton->setText(QStringLiteral("继续发酵"));
        }*/
    }
    else
    {
        //ui->pauseFerButton->setEnabled(false);
        ui->ferStateLabel->setText(QStringLiteral("发酵终止"));
        ui->aerationStateLabel->setText(QStringLiteral(""));
    }
    ui->endFerButton->setEnabled(isFerStarted);

    /*bool ferSensor = Global::getFerRunctrValueByName(tankIndex,"FER_SENSOR_BOOL", dataMap);
    p = ui->sensorStateLabel->palette();
    if(ferSensor)
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::green)));
    }
    else
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::red)));
    }
    ui->sensorStateLabel->setPalette(p);*/
}

QString FerControlDialog::formatLongDateString(uint value)
{
    int day = value / 86400;
    int hour = (value - day*86400) / 3600;
    int minute = (value - day*86400 - hour*3600) / 60;
    int second = (value - day*86400 - hour*3600 - minute * 60);
    return QStringLiteral("%1天%2时%3分%4秒").arg(QString::number(day)).arg(QString::number(hour)).arg(QString::number(minute)).arg(QString::number(second));
}

void FerControlDialog::setFerRunSteps()
{
    int deviceIndex = ui->tankIndexComboBox->currentIndex();
    if(deviceIndex >= 0)
    {
        if(_ferConfigs.contains(deviceIndex))
        {
            ui->runStepComboBox->clear();
            for(int i=0;i<_ferConfigs[deviceIndex].length();i++)
            {
                ui->runStepComboBox->addItem(QString::number(i+1));
            }
        }
        else
        {
            QList<FerStep *> ferSteps;
            FerConfig::loadConfig(&ferSteps, deviceIndex);
            if(ferSteps.length() > 0)
            {
                _ferConfigs.insert(deviceIndex, ferSteps);
                ui->runStepComboBox->clear();
                for(int i=0;i<ferSteps.length();i++)
                {
                    ui->runStepComboBox->addItem(QString::number(i+1));
                }
            }
            else
            {
                ui->runStepComboBox->clear();
            }
        }
    }
}

void FerControlDialog::on_tankIndexComboBox_currentIndexChanged(int index)
{
    tankIndex = index;
    setFerRunSteps();
    parseFermentationData(Global::currentFermenationDataMap);
    parseFerRunCtrData(Global::currentFermenationDataMap);
    parseFerRunTimeData(Global::currentFermenationDataMap);
    parseFerStepData(Global::currentFermenationDataMap);
    parseFerStartEndTime(Global::currentFermenationDataMap);
}

void FerControlDialog::on_changeStepButton_clicked()
{
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);

    StreamPack bpack;
    bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Int,0,0,1,0,0,0};
    //Length of ushort address and value, plus length of scrc.
    bpack.bDataLength = 1;
    bpack.bStreamLength += (4+2)*bpack.bDataLength + 4;

    QList<ushort> addrs;
    QList<int> values;
    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_STEPCTR_UI_CTRL");
    ushort addr = deviceNode.Offset + (info.offset + tankIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    addrs.append(addr);
    int data = ui->runStepComboBox->currentIndex() + 1;
    values.append(data);

    QByteArray allPackData, SData, crcData;
    QDataStream out(&SData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out.setByteOrder(QDataStream::LittleEndian);

    allPackData.append((char*)&bpack, sizeof(bpack));

    foreach(ushort item, addrs)
    {
        out << item;
    }

    foreach(int item, values)
    {
        out << item;
    }

    SData.insert(0, allPackData);

    uint scrc = tcpClient->StreamLen_CRC32(SData);

    QDataStream out1(&crcData,QIODevice::WriteOnly);
    out1.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out1.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out1.setByteOrder(QDataStream::LittleEndian);
    out1 << scrc;

    SData.append(crcData);

    tcpClient->sendRequestWithResults(SData);
}


void FerControlDialog::on_pauseFerButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        StreamPack bpack;
        ushort offset = Global::getFermenationNodeInfoByName("FER_Pause").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FER_Pause").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankIndex - info.startIndex) * runctrlByteSize + offset;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Bool,address,index,1,0,0,0};
        bool data = !isFerPaused;
        QVariant var_data = QVariant(data);

        tcpClient1->abort();
        tcpClient1->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}
