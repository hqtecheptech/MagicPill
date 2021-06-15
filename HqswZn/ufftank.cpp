#include "ufftank.h"
#include "ui_ufftank.h"
#include "dataformat.h"
#include "global.h"

#include <QPalette>
#include <QResizeEvent>
#include <QPixmap>

UFFTank::UFFTank(QWidget *parent) :
    BaseTankWidget(parent),
    ui(new Ui::UFFTank)
{
    ui->setupUi(this);
    ui->valveButton->setFlat(true);

    ferModel = new QStandardItemModel(this);
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
    runctrMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    tankRunBgImg.load("://image/old/nerFer/FerARun.bmp");
    tankRunQBgImg.load("://image/old/nerFer/FerARunBQ.bmp");
    tankStopBgImg.load("://image/old/nerFer/FerAStop.bmp");
    tankRunFanOpenBgImg.load("://image/old/nerFer/FerARunBQ.bmp");
    valveOpenBgImg.load("://image/new/green.bmp");
    valveCloseBgImg.load("://image/new/yellow.bmp");
    ferAutoBgImg.load("://image/old/FerLEDG.bmp");
    ferCustomBgImg.load("://image/old/FerLEDY.bmp");

    sftValveDialog = new SingelFerTankValveControlDialog(this);
}

UFFTank::~UFFTank()
{
    delete ui;
}

void UFFTank::showEvent(QShowEvent *event)
{
    myTimerThread = new MyTimerThread(2, this);
    connect(myTimerThread, SIGNAL(timeout()),this,SLOT(updateDeviceState()));
    myTimerThread->start();
}

int UFFTank::getLocation() const
{
    return location;
}

void UFFTank::setLocation(int value)
{
    location = value;
}

int UFFTank::getTankIndex() const
{
    return tankIndex;
}

void UFFTank::setTankIndex(int value)
{
    tankIndex = value;
    sftValveDialog->setValveIndex(tankIndex);
}

void UFFTank::updateDeviceState()
{
    ui->tankIndexLabel->setText(QString::number(location+1));

    ui->tankFrame->setObjectName("tankframe");
    ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerAStop.bmp)}");

    if(isFerStart)
    {
        ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARun.bmp)}");

        if(isAeration)
        {
            if(bgChange == 0)
            {
                ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARunBQ.bmp)}");
                bgChange = 1;
            }
            else
            {
                ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARun.bmp)}");
                bgChange = 0;
            }
        }
    }

    ui->valveButton->setObjectName("valveButton");
    if(isValveOpen)
    {
        ui->valveButton->setStyleSheet("QPushButton#valveButton{border-image:url(:/image/new/green_valve_button.png)}");
    }
    else
    {
        ui->valveButton->setStyleSheet("QPushButton#valveButton{border-image:url(:/image/new/red_valve_button.png)}");
    }
    if(isValveFault)
    {
        if(switchFlag)
        {
            ui->valveButton->setStyleSheet("QPushButton#valveButton{border-image:url(:/image/new/yellow_valve_button.png)}");
        }
        else
        {
            ui->valveButton->setStyleSheet("QPushButton#valveButton{border-image:url(:/image/new/green_valve_button.png)}");
        }
    }

    switchFlag = !switchFlag;
}

void UFFTank::setupFermentationMapper(QVector<float> values, int tankIndex)
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
    ferMapper->addMapping(ui->coValueLabel,6,"text");
    ferMapper->toFirst();
}

QString UFFTank::formatLongDateString(uint value)
{
    int day = value / 86400;
    int hour = (value - day*86400) / 3600;
    int minute = (value - day*86400 - hour*3600) / 60;
    int second = (value - day*86400 - hour*3600 - minute * 60);
    return QStringLiteral("%1天%2时%3分%4秒").arg(QString::number(day)).arg(QString::number(hour)).arg(QString::number(minute)).arg(QString::number(second));
}

void UFFTank::setupStepRunningTimeMapper(QVector<uint> values, int tankIndex)
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
//                QStandardItem *item = new QStandardItem(dt.toString("yyyy-MM-dd hh:mm"));
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
//    QModelIndex &index = timeModel->index(0,7);
//    QVariant totalRunTimeData = timeModel->data(index,Qt::UserRole);
//    uint totalRunTime = totalRunTimeData.toUInt();
//    ui->totalTimeValueLabel->setText(QString::number(totalRunTime));
//    index = timeModel->index(0,8);
//    QVariant intervalTimeData = timeModel->data(index,Qt::UserRole);
//    uint intervalRunTime = intervalTimeData.toUInt();
//    ui->intervalTimeValueLabel->setText(QString::number(intervalRunTime));

//    timeMapper->toFirst();
}

void UFFTank::setupStepMapper(QVector<ushort> values, int tankIndex)
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
void UFFTank::setupRunCtrMapper(QVector<bool> values, int tankIndex)
{
    QList<QStandardItem *> rowItems;
    for(int tn=0; tn<Global::ferDeviceInfo.RunCtr_Block_Size; tn++)
    {
        QStandardItem *item = new QStandardItem(QString::number(values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn]));
        if(tn == Global::getFermenationNodeInfoByName("FAN_Remote_BOOL").Offset)
        {
            isFanRemote = values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn];
        }
        /*if(tn == Global::getFermenationNodeInfoByName("FAN_Auto_BOOL").Offset)
        {
            isFanAuto = values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn];
        }
        if(tn == Global::getFermenationNodeInfoByName("FAN_Run_BOOL").Offset)
        {
            isFanOpen = values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn];
        }*/

        if(tn == Global::getFermenationNodeInfoByName("VALVE_Fault_BOOL").Offset)
        {
            isValveFault = values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn];
        }
        if(tn == Global::getFermenationNodeInfoByName("VALVE_Opened_BOOL").Offset)
        {
            isValveOpen = values[tankIndex*Global::ferDeviceInfo.RunCtr_Block_Size + tn];
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
    runctrMapper->addMapping(ui->valveButton,0,"icon");
    runctrMapper->toFirst();
}

QStandardItemModel *UFFTank::getRunctrModel() const
{
    return runctrModel;
}

QStandardItemModel *UFFTank::getStepModel() const
{
    return stepModel;
}

QStandardItemModel *UFFTank::getFerModel() const
{
    return ferModel;
}

QStandardItemModel *UFFTank::getTimeModel1() const
{
    return timeModel1;
}

void UFFTank::on_valveButton_clicked()
{
    sftValveDialog->show();
}
