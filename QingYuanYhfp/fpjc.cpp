#include "fpjc.h"
#include "ui_fpjc.h"

#include <QMap>
#include <QVector>

Fpjc::Fpjc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fpjc)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    deviceIndex = 0;

    st.start();

    qRegisterMetaType<Plc_Db>("Plc_Db");
    qRegisterMetaType<HistData>("HistData");

    ui->speedDownButton->setVisible(false);
    ui->speedUpButton->setVisible(false);
    ui->yhUpButton->setVisible(false);
    ui->yhDownButton->setVisible(false);

    ui->Fpj_LeftBehind_label->setVisible(false);
    ui->Fpj_LeftFront_label->setVisible(false);
    ui->Fpj_Left_Roller_Speed_label->setVisible(false);

    QString eixtStyleStr="QPushButton#exitButton{background: transparent; background-image: url(:/pic/退出.png);}"
                         "QPushButton#exitButton:hover{background: transparent; background-image: url(:/pic/退出.png);}"
                         "QPushButton#exitButton:pressed{background: transparent; background-image: url(:/pic/退出.png);}";
    ui->exitButton->setStyleSheet(eixtStyleStr);

    ui->speedDownButton->setStyleSheet("QPushButton#speedDownButton{background: transparent; background-image: url(:/pic/_02.png);}");
    ui->speedUpButton->setStyleSheet("QPushButton#speedUpButton{background: transparent; background-image: url(:/pic/+02.png);}");

    ui->yhDownButton->setStyleSheet("QPushButton#yhDownButton{background: transparent; background-image: url(:/pic/_02.png);}");
    ui->yhUpButton->setStyleSheet("QPushButton#yhUpButton{background: transparent; background-image: url(:/pic/+02.png);}");

    QString speedLabelStyleStr = "background: transparent;"
                                 "background-position:center;"
                                 "background-repeat:no-repeat;"
                                 "background-image: url(:/pic/输入框.png);"
                                 "color:white;";
    ui->speedLabel->setStyleSheet(speedLabelStyleStr);
    ui->yhChoiseLabel->setStyleSheet(speedLabelStyleStr);

    ui->fpjWatchsWidget->setLeftTopPlateRange(0, 300, 0, 180);
    ui->fpjWatchsWidget->setRightTopPlateRange(0, 300, 0, 180);
    ui->fpjWatchsWidget->setLeftBottomPlateRange(0, 300, 0, 25000);
    ui->fpjWatchsWidget->setRightBottomPlateRange(0, 300, 0, 25000);
    ui->fpjWatchsWidget->setLeftCenterPlateRange(0, 150, 0, 150);
    //ui->fpjWatchsWidget->setRightCenterPlateRange(0, 150, 0, 150);

    ui->leftFpButton->setEnabled(false);
    ui->rightFpButton->setEnabled(false);

    for(int i=0; i < CP; i++)
    {
        lrpArr.append(0);
        lrsArr.append(0);
        lwpArr.append(0);
        lspArr.append(0);
        rrpArr.append(0);
        rrsArr.append(0);
        rwpArr.append(0);
        rspArr.append(0);
    }
    ui->leftCurveWidget->setRange(300, 180);
    ui->rightCurveWidget->setRange(300, 180);

    checkNetStateTimer = new QTimer(this);
    connect(checkNetStateTimer, SIGNAL(timeout()), this, SLOT(checkWifiConnection()));

    updateWatchTimer = new QTimer(this);
    connect(updateWatchTimer, SIGNAL(timeout()), this, SLOT(updateWatch()));

    updateChartTimer = new QTimer(this);
    connect(updateChartTimer, SIGNAL(timeout()), this, SLOT(updateChart()));
    updateChartTimer->start(60000);

    alertHisDlg = new AlertHistoryDialog(this);
    hisDlg = new HistoryDlg(this);   
    runStatusDlg = new FpjRunStatusDialog(this);

    QStringList hisItems = {"左翻抛压力", "左翻抛转速", "左履带压力", "左履带转速"
                           ,"右翻抛压力", "右翻抛转速", "右履带压力", "右履带转速"};
    hisDlg->setQueryItems(hisItems);

    dbWorker = new DatabaseWorker;
    dbWorker->moveToThread(&dbThread);
    connect(&dbThread, &QThread::finished, dbWorker, &QObject::deleteLater);
    connect(this, SIGNAL(histDataReady(HistData)), dbWorker, SLOT(saveHistData(HistData)));
    dbThread.start();
}

Fpjc::~Fpjc()
{
    delete ui;
}

void Fpjc::showEvent(QShowEvent *)
{
    if(!updateWatchTimer->isActive())
    {
        updateWatchTimer->start(5000);
    }

    if(!checkNetStateTimer->isActive())
    {
        checkNetStateTimer->start(10000);
    }
}

void Fpjc::closeEvent(QCloseEvent *)
{
    if(checkNetStateTimer->isActive())
    {
        checkNetStateTimer->stop();
    }

    if(updateWatchTimer->isActive())
    {
        updateWatchTimer->stop();
    }
}

void Fpjc::on_exitButton_clicked()
{
    close();
}

void Fpjc::updateWatch()
{
    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Fpj_Left_Roller_Rotation_Pressure");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Left_Roller_Rotation_Pressure value: " << Global::currentYhcDataMap.value(address);
    float lrp = Global::currentYhcDataMap.value(address).toFloat();

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Left_Roller_Speed");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Left_Roller_Speed value: " << Global::currentYhcDataMap.value(address);
    ushort lrs = Global::currentYhcDataMap.value(address).toUShort();

    //ui->Fpj_Left_Roller_Speed_label->setText(Global::currentYhcDataMap.value(address));

    ui->fpjWatchsWidget->updateLeftTopPlate(lrp, lrs);

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Right_Roller_Rotation_Pressure");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Right_Roller_Rotation_Pressure value: " << Global::currentYhcDataMap.value(address);
    float rrp = Global::currentYhcDataMap.value(address).toFloat();

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Right_Roller_Speed");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Right_Roller_Speed value: " << Global::currentYhcDataMap.value(address);
    ushort rrs = Global::currentYhcDataMap.value(address).toUShort();

    //ui->Fpj_Right_Roller_Speed_label->setText(Global::currentYhcDataMap.value(address));

    ui->fpjWatchsWidget->updateRightTopPlate(rrp, rrs);

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Left_Walking_Pressure");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Left_Walking_Pressure value: " << Global::currentYhcDataMap.value(address);
    float lwp = Global::currentYhcDataMap.value(address).toFloat();

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Left_Servo_Pulse");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Left_Servo_Pulse value: " << Global::currentYhcDataMap.value(address);
    uint lsp = Global::currentYhcDataMap.value(address).toUInt();

    ui->fpjWatchsWidget->updateLeftBottomPlate(lwp, lsp);

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Right_Walking_Pressure");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Right_Walking_Pressure value: " << Global::currentYhcDataMap.value(address);
    float rwp = Global::currentYhcDataMap.value(address).toFloat();

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Right_Servo_Pulse");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Right_Servo_Pulse value: " << Global::currentYhcDataMap.value(address);
    uint rsp = Global::currentYhcDataMap.value(address).toUInt();

    ui->fpjWatchsWidget->updateRightBottomPlate(rwp, rsp);

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Hs_1_Tempture");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Hs_1_Tempture value: " << Global::currentYhcDataMap.value(address);
    float ht1 = Global::currentYhcDataMap.value(address).toFloat();

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Hs_1_Oil_Level");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Hs_1_Oil_Level value: " << Global::currentYhcDataMap.value(address);
    float hl1 = Global::currentYhcDataMap.value(address).toFloat();

    ui->fpjWatchsWidget->updateLeftCenterPlate(ht1, hl1);

    /*deviceNode = Global::getYhcNodeInfoByName("Fpj_Hs_2_Tempture");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Hs_2_Tempture value: " << Global::currentYhcDataMap.value(address);
    float ht2 = Global::currentYhcDataMap.value(address).toFloat();

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Hs_2_Oil_Level");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Hs_2_Oil_Level value: " << Global::currentYhcDataMap.value(address);
    float hl2 = Global::currentYhcDataMap.value(address).toUShort();

    ui->fpjWatchsWidget->updateRightCenterPlate(ht2, hl2);

    deviceNode = Global::getYhcNodeInfoByName("Fpj_LeftFront");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_LeftFront value: " << Global::currentYhcDataMap.value(address);
    ui->Fpj_LeftFront_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_RightFront");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_RightFront value: " << Global::currentYhcDataMap.value(address);
    ui->Fpj_RightFront_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_LeftBehind");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_LeftBehind value: " << Global::currentYhcDataMap.value(address);
    ui->Fpj_LeftBehind_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_RightBehind");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_RightBehind value: " << Global::currentYhcDataMap.value(address);
    ui->Fpj_RightBehind_label->setText(Global::currentYhcDataMap.value(address));*/

    deviceNode = Global::getYhcNodeInfoByName("Fpj_RightTop");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_RightTop value: " << Global::currentYhcDataMap.value(address);
    ui->Fpj_RightFront_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_RightMiddle");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_RightMiddle value: " << Global::currentYhcDataMap.value(address);
    ui->Fpj_RightBehind_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_RightBottom");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_RightBottom value: " << Global::currentYhcDataMap.value(address);
    ui->Fpj_Right_Roller_Speed_label->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Left_Track_Encoder_Cycle_Num");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Left_Track_Encoder_Cycle_Num value: " << Global::currentYhcDataMap.value(address);
    ui->speedLabel->setText(Global::currentYhcDataMap.value(address));

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Right_Track_Encoder_Cycle_Num");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Right_Track_Encoder_Cycle_Num value: " << Global::currentYhcDataMap.value(address);
    ui->yhChoiseLabel->setText(Global::currentYhcDataMap.value(address));
}

void Fpjc::updateChart()
{
    QDateTime currentdt = QDateTime::currentDateTime();
    uint stime =currentdt.toTime_t();
    HistData data;

    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(deviceIndex);
    /*DeviceNode deviceNode = Global::getYhcNodeInfoByName("Fpj_Left_Roller_Pressure");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Left_Roller_Pressure value: " << Global::currentYhcDataMap.value(address);
    int index = Global::convertAddressToIndex(address, deviceNode.DataType);
    float lrp = Global::currentYhcDataMap.value(address).toFloat();

    data.address = address;
    strcpy(data.dataType, deviceNode.DataType.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.deviceIndex = index;
    data.address = address;
    data.index = 0;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Fpj_Left_Roller_Pressure").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);*/

    float lrp = 0;

    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Fpj_Left_Roller_Speed");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Left_Roller_Speed value: " << Global::currentYhcDataMap.value(address);
    int index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ushort lrs = Global::currentYhcDataMap.value(address).toUShort();

    data.address = address;
    strcpy(data.dataType, deviceNode.DataType.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.deviceIndex = index;
    data.address = address;
    data.index = 0;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Fpj_Left_Roller_Speed").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);

    if(lrpArr.count() == CP)
    {
        lrpArr.append(lrp);
        lrpArr.removeFirst();
    }
    else
    {
        lrpArr.append(lrp);
    }
    if(lrsArr.count() == CP)
    {
        lrsArr.append(lrs);
        lrsArr.removeFirst();
    }
    else
    {
        lrsArr.append(lrs);
    }

    if(!leftChartType)
    {
        ui->leftCurveWidget->updateUI(lrp, lrs);
    }

    /*deviceNode = Global::getYhcNodeInfoByName("Fpj_Right_Roller_Pressure");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Right_Roller_Pressure value: " << Global::currentYhcDataMap.value(address);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    float rrp = Global::currentYhcDataMap.value(address).toFloat();

    data.address = address;
    strcpy(data.dataType, deviceNode.DataType.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.deviceIndex = index;
    data.address = address;
    data.index = 0;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Fpj_Right_Roller_Pressure").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);*/

    float rrp = 0;

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Right_Roller_Speed");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Right_Roller_Speed value: " << Global::currentYhcDataMap.value(address);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ushort rrs = Global::currentYhcDataMap.value(address).toUShort();

    data.address = address;
    strcpy(data.dataType, deviceNode.DataType.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.deviceIndex = index;
    data.address = address;
    data.index = 0;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Fpj_Right_Roller_Speed").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);

    if(rrpArr.count() == CP)
    {
        rrpArr.append(rrp);
        rrpArr.removeFirst();
    }
    else
    {
        rrpArr.append(rrp);
    }
    if(rrsArr.count() == CP)
    {
        rrsArr.append(rrs);
        rrsArr.removeFirst();
    }
    else
    {
        rrsArr.append(rrs);
    }

    if(!rightChartType)
    {
        ui->rightCurveWidget->updateUI(rrp, rrs);
    }

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Left_Walking_Pressure");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Left_Walking_Pressure value: " << Global::currentYhcDataMap.value(address);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    float lwp = Global::currentYhcDataMap.value(address).toFloat();

    data.address = address;
    strcpy(data.dataType, deviceNode.DataType.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.deviceIndex = index;
    data.address = address;
    data.index = 0;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Fpj_Left_Walking_Pressure").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Left_Servo_Pulse");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Left_Servo_Pulse value: " << Global::currentYhcDataMap.value(address);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    uint lsp = Global::currentYhcDataMap.value(address).toUInt();

    data.address = address;
    strcpy(data.dataType, deviceNode.DataType.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.deviceIndex = index;
    data.address = address;
    data.index = 0;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Fpj_Left_Servo_Pulse").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);

    if(lwpArr.count() == CP)
    {
        lwpArr.append(lwp);
        lwpArr.removeFirst();
    }
    else
    {
        lwpArr.append(lwp);
    }
    if(lspArr.count() == CP)
    {
        lspArr.append(lsp);
        lspArr.removeFirst();
    }
    else
    {
        lspArr.append(lsp);
    }
    if(leftChartType)
    {
        ui->leftCurveWidget->updateUI(lwp, lsp);
    }

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Right_Walking_Pressure");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Right_Walking_Pressure value: " << Global::currentYhcDataMap.value(address);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    float rwp = Global::currentYhcDataMap.value(address).toFloat();

    data.address = address;
    strcpy(data.dataType, deviceNode.DataType.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.deviceIndex = index;
    data.address = address;
    data.index = 0;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Fpj_Right_Walking_Pressure").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);

    deviceNode = Global::getYhcNodeInfoByName("Fpj_Right_Servo_Pulse");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Fpj_Right_Servo_Pulse value: " << Global::currentYhcDataMap.value(address);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    uint rsp = Global::currentYhcDataMap.value(address).toUInt();

    data.address = address;
    strcpy(data.dataType, deviceNode.DataType.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.deviceIndex = index;
    data.address = address;
    data.index = 0;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Fpj_Right_Servo_Pulse").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);

    if(rwpArr.count() == CP)
    {
        rwpArr.append(rwp);
        rwpArr.removeFirst();
    }
    else
    {
        rwpArr.append(rwp);
    }
    if(rspArr.count() == CP)
    {
        rspArr.append(lsp);
        rspArr.removeFirst();
    }
    else
    {
        rspArr.append(rsp);
    }

    if(rightChartType)
    {
        ui->rightCurveWidget->updateUI(rwp, rsp);
    }
}

void Fpjc::checkWifiConnection()
{
    /*bool wifiConnected = Global::getYhcRunctrValueByName(_deviceIndex, "SIGNAL_STATE", Global::currentYhcDataMap);
    if(!wifiConnected)
    {
        ui->wlanLabel->setStyleSheet("QLabel#wlanLabel{background: transparent;"
                                     "background-position:center;"
                                     "background-repeat:no-repeat;"
                                     "background-image: url(:/pic/no_wifi.png);}");
    }
    else
    {
        ui->wlanLabel->setStyleSheet("QLabel#wlanLabel{background: transparent;"
                                     "background-position:center;"
                                     "background-repeat:no-repeat;"
                                     "background-image: url(:/pic/信号4.png);}");
    }*/
}

void Fpjc::on_leftFpButton_clicked()
{
    ui->leftFpButton->setEnabled(false);
    ui->leftFpButton->setStyleSheet("border-image: url(:/pic/左翻抛.png) 0 0 0 0;");

    leftChartType = false;
    ui->leftCurveWidget->setRange(300, 180);
    for(int i=0; i<lrpArr.count(); i++)
    {
        ui->leftCurveWidget->updateUI(lrpArr.at(i), lrsArr.at(i));
    }

    ui->leftLdButton->setEnabled(true);
    ui->leftLdButton->setStyleSheet("border-image: url(:/pic/左履带.png) 0 0 0 100;");
}

void Fpjc::on_leftLdButton_clicked()
{
    ui->leftLdButton->setEnabled(false);
    ui->leftLdButton->setStyleSheet("border-image: url(:/pic/左履带-拷贝.png) 0 0 0 0;");

    leftChartType = true;
    ui->leftCurveWidget->setRange(300, 25000);
    for(int i=0; i<lwpArr.count(); i++)
    {
        ui->leftCurveWidget->updateUI(lwpArr.at(i), lspArr.at(i));
    }

    ui->leftFpButton->setEnabled(true);
    ui->leftFpButton->setStyleSheet("border-image: url(:/pic/左翻抛-拷贝.png) 0 100 0 0;");
}

void Fpjc::on_rightFpButton_clicked()
{
    ui->rightFpButton->setEnabled(false);
    ui->rightFpButton->setStyleSheet("border-image: url(:/pic/右翻抛.png) 0 0 0 0;");

    rightChartType = false;
    ui->rightCurveWidget->setRange(300, 180);
    for(int i=0; i<rrpArr.count(); i++)
    {
        ui->rightCurveWidget->updateUI(rrpArr.at(i), rrsArr.at(i));
    }

    ui->rightLdButton->setEnabled(true);
    ui->rightLdButton->setStyleSheet("border-image: url(:/pic/右履带.png) 0 0 0 100;");
}

void Fpjc::on_rightLdButton_clicked()
{
    ui->rightLdButton->setEnabled(false);
    ui->rightLdButton->setStyleSheet("border-image: url(:/pic/右履带-拷贝.png) 0 0 0 0;");

    rightChartType = true;
    ui->rightCurveWidget->setRange(300, 25000);
    for(int i=0; i<rwpArr.count(); i++)
    {
        ui->rightCurveWidget->updateUI(rwpArr.at(i), rspArr.at(i));
    }

    ui->rightFpButton->setEnabled(true);
    ui->rightFpButton->setStyleSheet("border-image: url(:/pic/右翻抛-拷贝.png) 0 100 0 0;");
}

void Fpjc::on_alertButton_clicked()
{
    alertHisDlg->show();
}

void Fpjc::on_historyButton_clicked()
{
    hisDlg->show();
}

void Fpjc::on_assistButton_clicked()
{
    runStatusDlg->show();
}
