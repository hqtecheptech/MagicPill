#include "fftank.h"
#include "ui_fftank.h"
#include "dataformat.h"
#include "global.h"
#include "identity.h"
#include "fancontroldialog.h"

#include <QPalette>
#include <QResizeEvent>
#include <QPixmap>

FFTank::FFTank(QWidget *parent) :
    BaseTankWidget(parent),
    ui(new Ui::FFTank)
{
    ui->setupUi(this);
    ui->fanButton->setFlat(true);
    fanControlDialog = new FanControlDialog(this);
    connect(this, SIGNAL(dataChanged(QSet<int>, QMap<float,QString>)), fanControlDialog, SLOT(updateFermentationData(QSet<int>, QMap<float,QString>)));

    switchStateTimer = new QTimer(this);
    connect(switchStateTimer, SIGNAL(timeout()), this, SLOT(switchState()));
    /*ferModel = new QStandardItemModel(this);
    runctrModel = new QStandardItemModel(this);
    stepModel = new QStandardItemModel(this);
    timeModel = new QStandardItemModel(this);
    timeModel1 = new QStandardItemModel(this);

    ferMapper = new QDataWidgetMapper(this);
    ferMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    timeMapper = new QDataWidgetMapper(this);
    timeMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    stepMapper = new QDataWidgetMapper(this);
    stepMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    runctrMapper = new QDataWidgetMapper(this);
    runctrMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);*/

    ui->ocValueLabel->setVisible(false);
    ui->oxygenContent->setVisible(false);
    ui->wtValueLabel_3->setVisible(false);

    tankRunBgImg.load("://image/old/nerFer/FerARun.bmp");
    tankRunQBgImg.load("://image/old/nerFer/FerARunBQ.bmp");
    tankStopBgImg.load("://image/old/nerFer/FerAStop.bmp");
    tankRunFanOpenBgImg.load("://image/old/nerFer/FerARunBQ.bmp");
    fanOpenBgImg.load("://image/new/tank_valve_opened3.png");
    fanCloseBgImg.load("://image/new/tank_valve_closed.png");
    fanFaultBgImg.load("://image/new/tank_valve_fault.png");
    ferAutoBgImg.load("://image/old/FerLEDG.bmp");
    ferCustomBgImg.load("://image/old/FerLEDY.bmp");

    updateDeviceState();
}

FFTank::~FFTank()
{
    delete ui;
}

void FFTank::showEvent(QShowEvent *event)
{
    if(!switchStateTimer->isActive())
    {
        switchStateTimer->start(2000);
    }
}

void FFTank::closeEvent(QCloseEvent *event)
{
    if(switchStateTimer->isActive())
    {
        switchStateTimer->stop();
    }
}

int FFTank::getLocation() const
{
    return location;
}

void FFTank::setLocation(int value)
{
    location = value;
}

int FFTank::getTankIndex() const
{
    return tankIndex;
}

void FFTank::setTankIndex(int value)
{
    tankIndex = value;
    ui->tankIndexLabel->setText(QString::number(tankIndex+1));
}

void FFTank::updateDeviceState()
{
    ui->tankFrame->setObjectName("tankframe");
    ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerAStop.bmp)}");

    if(isFerStart)
    {
        ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARun.bmp)}");

        if(isAeration)
        {
            ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARunBQ.bmp)}");
        }
    }

    if(!isFanOpen)
    {
        ui->fanButton->setIcon(QIcon(fanCloseBgImg));
    }
    else
    {
        ui->fanButton->setIcon(QIcon(fanOpenBgImg));
    }
}

void FFTank::switchState()
{
    ui->tankFrame->setObjectName("tankframe");

    extraRuntimeAddition = extraRuntimeAddition + 2;

    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    if(isFerStart || deviceIndex == 0 || deviceIndex == 2 || deviceIndex == 7 || deviceIndex == 11)
    {
        DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_TOT_UDI");
        float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
        uint runtime = Global::currentFermenationDataMap[address].toUInt() + extraRuntimeAddition;
        ui->totalTimeValueLabel->setText(formatLongDateString(runtime));

        if(isAeration || deviceIndex == 0 || deviceIndex == 2 || deviceIndex == 7 || deviceIndex == 11)
        {
            deviceNode = Global::getFermenationNodeInfoByName("FER_AE_UDI");
            float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
            uint runtime = Global::currentFermenationDataMap[address].toUInt() + extraRuntimeAddition;
            ui->runTimeValueLabel->setText(QString::number(runtime));

            if(switchFlag)
            {
                ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARunBQ.bmp)}");
            }
            else
            {
                ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARun.bmp)}");
            }
        }
        else
        {
            deviceNode = Global::getFermenationNodeInfoByName("FER_STA_UDI");
            float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
            uint runtime = Global::currentFermenationDataMap[address].toUInt() + extraRuntimeAddition;
            ui->spacTimeValueLabel->setText(QString::number(runtime));
        }
    }

    if(isFanFault)
    {
        if(switchFlag)
        {
            ui->fanButton->setIcon(QIcon(fanFaultBgImg));
        }
        else
        {
            ui->fanButton->setIcon(QIcon(fanCloseBgImg));
        }
    }

    switchFlag = !switchFlag;
}

void FFTank::setupFermentationMapper(QVector<float> values, int tankIndex)
{
    QList<QStandardItem *> rowItems;
    for(int tn = 0; tn<values.length(); ++tn)
    {
        if(tn % Global::ferDeviceInfo.Device_Number == tankIndex)
        {
            QStandardItem *item = new QStandardItem(QString::number(values[tn]));
            item->setData(QVariant(values[tn]),Qt::UserRole);
            rowItems.append(item);
        }
    }

    ferModel->clear();
    ferModel->appendRow(rowItems);
    ferMapper->clearMapping();
    ferMapper->setModel(ferModel);
    ferMapper->addMapping(ui->wtValueLabel,3,"text");
    ferMapper->toFirst();
}

QString FFTank::formatLongDateString(uint value)
{
    int day = value / 86400;
    int hour = (value - day*86400) / 3600;
    int minute = (value - day*86400 - hour*3600) / 60;
    int second = (value - day*86400 - hour*3600 - minute * 60);
    return QStringLiteral("%1天%2时%3分%4秒").arg(QString::number(day)).arg(QString::number(hour)).arg(QString::number(minute)).arg(QString::number(second));
}

void FFTank::setupStepRunningTimeMapper(QVector<uint> values, int tankIndex)
{
//    QList<QStandardItem *> rowItems;
//    QList<QStandardItem *> rowItems1;
//    for(int tn = 0; tn<values.length(); ++tn)
//    {
//        if(tn % Global::ferDeviceInfo.Device_Number == tankIndex)
//        {
//            int section = tn / Global::ferDeviceInfo.Device_Number;
//            QDateTime dt = QDateTime::fromTime_t(values[tn]);
//            if(section != 0 && section != 1)
//            {
//                int day = values[tn] / 86400;
//                int hour = (values[tn] - day*86400) / 3600;
//                int minute = (values[tn] - day*86400 - hour*3600) / 60;
//                int second = (values[tn] - day*86400 - hour*3600 - minute * 60);
//                QString strTotalTime = QStringLiteral("%1天%2时%3分%4秒").arg(QString::number(day)).arg(QString::number(hour)).arg(QString::number(minute)).arg(QString::number(second));

//                QStandardItem *item = new QStandardItem(strTotalTime);
//                item->setData(QVariant(values[tn]),Qt::UserRole);
//                rowItems.append(item);
//                QStandardItem *item1 = new QStandardItem(strTotalTime);
//                item1->setData(QVariant(values[tn]),Qt::UserRole);
//                rowItems1.append(item1);
//            }
//            else
//            {
//                QStandardItem *item = new QStandardItem(dt.toString("yyyy-MM-dd\nhh:mm:ss"));
//                item->setData(QVariant(values[tn]),Qt::UserRole);
//                rowItems.append(item);
//                QStandardItem *item1 = new QStandardItem(dt.toString("yyyy-MM-dd hh:mm:ss"));
//                item1->setData(QVariant(values[tn]),Qt::UserRole);
//                rowItems1.append(item1);
//            }
//        }
//    }
//    timeModel->clear();
//    timeModel1->clear();
//    timeModel->appendRow(rowItems);
//    timeModel1->appendRow(rowItems1);

//    timeMapper->clearMapping();
//    timeMapper->setModel(timeModel);
//    timeMapper->addMapping(ui->startTimeValueLabel,0,"text");
//    timeMapper->addMapping(ui->endTimeValueLabel,1,"text");
//    //timeMapper->addMapping(ui->totalTimeValueLabel,2,"text");
//    QModelIndex &index = timeModel->index(0,2);
//    QVariant totalRunTimeData = timeModel->data(index,Qt::UserRole);
//    uint totalRunTime = totalRunTimeData.toUInt();
//    ui->totalTimeValueLabel->setText(formatLongDateString(totalRunTime));

//    timeMapper->toFirst();
}

void FFTank::setupStepMapper(QVector<ushort> values, int tankIndex)
{
    QList<QStandardItem *> rowItems;
    for(int tn = 0; tn<values.length(); ++tn)
    {
        if(tn % Global::ferDeviceInfo.Device_Number == tankIndex)
        {
            QStandardItem *item = new QStandardItem(QString::number(values[tn]));
            item->setData(QVariant(values[tn]),Qt::UserRole);
            rowItems.append(item);
        }
    }

    stepModel->clear();
    stepModel->appendRow(rowItems);
    stepMapper->clearMapping();
    stepMapper->setModel(stepModel);
    //stepMapper->addMapping(ui->ferStepValueLabel,0,"text");
    stepMapper->toFirst();
}

// Todo
void FFTank::setupRunCtrMapper(QVector<bool> values, int tankIndex)
{
    QList<QStandardItem *> rowItems;
    for(int tn=0; tn<Global::ferDeviceInfo.RunCtr_Block_Size; tn++)
    {
        QStandardItem *item = new QStandardItem(QString::number(values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn]));
        if(tn == Global::getFermenationNodeInfoByName("FAN_Remote_BOOL").Offset)
        {
            isFanRemote = values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn];
        }
        if(tn == Global::getFermenationNodeInfoByName("FAN_Auto_BOOL").Offset)
        {
            isFanAuto = values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn];
        }
        if(tn == Global::getFermenationNodeInfoByName("FAN_Run_BOOL").Offset)
        {
            isFanOpen = values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn];
        }
        if(tn == Global::getFermenationNodeInfoByName("FER_State_BOOL").Offset)
        {
            isFerStart = values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn];
        }
        if(tn == Global::getFermenationNodeInfoByName("FER_AERATION_BOOL").Offset)
        {
            isAeration = values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn];
        }
        if(tn == Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset)
        {
            isFerAuto = values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn];
        }
        item->setData(QVariant(values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn]),Qt::UserRole);
        rowItems.append(item);
    }
    runctrModel->clear();
    runctrModel->appendRow(rowItems);
    runctrMapper->clearMapping();
    runctrMapper->setModel(runctrModel);
    runctrMapper->addMapping(ui->fanButton,0,"icon");
    runctrMapper->toFirst();
}

QStandardItemModel *FFTank::getRunctrModel() const
{
    return runctrModel;
}

QStandardItemModel *FFTank::getStepModel() const
{
    return stepModel;
}

QStandardItemModel *FFTank::getFerModel() const
{
    return ferModel;
}

QStandardItemModel *FFTank::getTimeModel1() const
{
    return timeModel1;
}

void FFTank::updateFermentationData(QSet<int> changedDeviceSet, QMap<float,QString> dataMap)
{
    if(changedDeviceSet.contains(tankIndex))
    {
        parseFermentationData(dataMap);
        parseFerRunTimeData(dataMap);
        //parseFerStepData(dataMap);
        parseFerRunCtrData(dataMap);
        parseFerStartEndTime(dataMap);
        emit dataChanged(changedDeviceSet, dataMap);
    }
}

void FFTank::parseFermentationData(QMap<float,QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_WT_R");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->wtValueLabel->setText(dataMap[address]);

    /*deviceNode = Global::getFermenationNodeInfoByName("FER_CO_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->ocValueLabel->setText(dataMap[address]);*/
}

void FFTank::parseFerStartEndTime(QMap<float,QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_Start_Time");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 12;
    qDebug() << "Start time = " << dataMap[address];
    uint runtime = dataMap[address].toUInt();
    QDateTime dt = QDateTime::fromTime_t(runtime);
    ui->startTimeValueLabel->setText(dt.toString("yyyy-MM-dd hh:mm"));

    deviceNode = Global::getFermenationNodeInfoByName("FER_End_Time");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 12;
    qDebug() << "End time = " << dataMap[address];
    runtime = dataMap[address].toUInt();
    dt = QDateTime::fromTime_t(runtime);
    ui->endTimeValueLabel->setText(dt.toString("yyyy-MM-dd hh:mm"));
}

void FFTank::parseFerRunTimeData(QMap<float,QString> dataMap)
{
    extraRuntimeAddition = 0;

    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode;
    float address;
    uint runtime;
    /*deviceNode = Global::getFermenationNodeInfoByName("FER_TOT_UDI");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    uint runtime = dataMap[address].toUInt();
    ui->totalTimeValueLabel->setText(formatLongDateString(runtime));

    deviceNode = Global::getFermenationNodeInfoByName("FER_START_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    runtime = dataMap[address].toUInt();
    QDateTime dt = QDateTime::fromTime_t(runtime);
    ui->startTimeValueLabel->setText(dt.toString("yyyy-MM-dd hh:mm"));

    deviceNode = Global::getFermenationNodeInfoByName("FER_END_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    runtime = dataMap[address].toUInt();
    qDebug() << "fercontrol endtime = " << runtime;
    dt = QDateTime::fromTime_t(runtime);
    ui->endTimeValueLabel->setText(dt.toString("yyyy-MM-dd hh:mm"));*/

    deviceNode = Global::getFermenationNodeInfoByName("FER_AE_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    runtime = dataMap[address].toUInt();
    ui->runTimeValueLabel->setText(QString::number(runtime));

    deviceNode = Global::getFermenationNodeInfoByName("FER_STA_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    runtime = dataMap[address].toUInt();
    ui->spacTimeValueLabel->setText(QString::number(runtime));
}

void FFTank::parseFerStepData(QMap<float,QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_STEPCTR_UI");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    int step = dataMap[address].toInt();
}

void FFTank::parseFerRunCtrData(QMap<float,QString> dataMap)
{
    isFanRemote = Global::getFerRunctrValueByName(tankIndex,"FAN_Remote_BOOL", dataMap);
    isFanAuto = Global::getFerRunctrValueByName(tankIndex,"FAN_Auto_BOOL", dataMap);
    isFanOpen = Global::getFerRunctrValueByName(tankIndex,"FAN_Run_BOOL", dataMap);
    isFerStart = Global::getFerRunctrValueByName(tankIndex,"FER_State_BOOL", dataMap);
    isAeration = Global::getFerRunctrValueByName(tankIndex,"FER_AERATION_BOOL", dataMap);
    isFerAuto = Global::getFerRunctrValueByName(tankIndex,"FER_Auto_BOOL", dataMap);
    isFanFault = Global::getFerRunctrValueByName(tankIndex,"FAN_Fault_BOOL", dataMap);

    ui->tankFrame->setObjectName("tankframe");
    ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerAStop.bmp)}");

    if(isFerStart)
    {
        ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARun.bmp)}");

        if(isAeration)
        {
            ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARunBQ.bmp)}");
        }
    }

    if(!isFanOpen)
    {
        ui->fanButton->setIcon(QIcon(fanCloseBgImg));
    }
    else
    {
        ui->fanButton->setIcon(QIcon(fanOpenBgImg));
    }

    if(isFanFault)
    {
        ui->fanButton->setIcon(QIcon(fanFaultBgImg));
    }
}

void FFTank::on_fanButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        fanControlDialog->setTankIndex(tankIndex);
        fanControlDialog->setCombBoxVisible(false);
        fanControlDialog->show();
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}
