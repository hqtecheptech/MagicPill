#include "yhcc.h"
#include "ui_yhcc.h"

#include <QMap>
#include <QVector>

Yhcc::Yhcc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Yhcc)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    _deviceIndex = 0;

    st.start();

    qRegisterMetaType<Plc_Db>("Plc_Db");
    qRegisterMetaType<HistData>("HistData");

    QString eixtStyleStr="QPushButton#yhcExitButton{background: transparent; background-image: url(:/pic/退出.png);}"
                         "QPushButton#yhcExitButton:hover{background: transparent; background-image: url(:/pic/退出.png);}"
                         "QPushButton#yhcExitButton:pressed{background: transparent; background-image: url(:/pic/退出.png);}";
    ui->yhcExitButton->setStyleSheet(eixtStyleStr);

    ui->speedDownButton->setStyleSheet("QPushButton#speedDownButton{background: transparent; background-image: url(:/pic/_02.png);}");
    ui->speedUpButton->setStyleSheet("QPushButton#speedUpButton{background: transparent; background-image: url(:/pic/+02.png);}");

    //ui->yhDownButton->setStyleSheet("QPushButton#yhDownButton{background: transparent; background-image: url(:/pic/_02.png);}");
    //ui->yhUpButton->setStyleSheet("QPushButton#yhUpButton{background: transparent; background-image: url(:/pic/+02.png);}");

    QString speedLabelStyleStr = "background: transparent;"
                                 "background-position:center;"
                                 "background-repeat:no-repeat;"
                                 "background-image: url(:/pic/输入框.png);"
                                 "color:white;";
    ui->speedLabel->setStyleSheet(speedLabelStyleStr);
    //ui->yhChoiseLabel->setStyleSheet(speedLabelStyleStr);

    //QString titleLabelStypeStr = "background: transparent;"
     //                            "color:rgb(200, 200, 200);";

    //ui->titleLabel->setStyleSheet(titleLabelStypeStr);

    checkNetStateTimer = new QTimer(this);
    connect(checkNetStateTimer, SIGNAL(timeout()), this, SLOT(getNetState()));

    testTimer = new QTimer(this);
    connect(testTimer, SIGNAL(timeout()), this, SLOT(wirteTestData()));

    /*controller = Syscontroller::getInstance(Global::systemConfig.deviceType, Global::systemConfig.deviceGroup);
    if(controller != Q_NULLPTR)
    {
        connect(controller, SIGNAL(resultReady()), this, SLOT(handleControllerResult()));
        connect(controller, &Syscontroller::plcDbUpdated, this, &Yhcc::handlePlcDataUpdate);
        connect(this, SIGNAL(requestControl()), controller, SLOT(applyControlRequest()));
    }*/

    getServerConnectStateTcpClient = new TcpClientSocket(this);
    connect(getServerConnectStateTcpClient, SIGNAL(updateConnectState(bool)), this, SLOT(localServerConnected(bool)));
    getAllDataTcpClient = new TcpClientSocket(this);
    connect(getAllDataTcpClient, SIGNAL(updateClients(QByteArray)), this, SLOT(showData(QByteArray)));
    actionTcpClient = new TcpClientSocket(this);

    connect(this, SIGNAL(dataUpdate(QSet<int>, QMap<float,QString>)), this,SLOT(updateData(QSet<int>, QMap<float,QString>)));

    dbWorker = new DatabaseWorker;
    dbWorker->moveToThread(&dbThread);
    connect(&dbThread, &QThread::finished, dbWorker, &QObject::deleteLater);
    connect(this, SIGNAL(histDataReady(HistData)), dbWorker, SLOT(saveHistData(HistData)));
    dbThread.start();

    psWorker = new ParseServerDataWorker;
    psWorker->moveToThread(&psThread);
    connect(&psThread, &QThread::finished, psWorker, &QObject::deleteLater);
    connect(this, SIGNAL(serverDataReceived(QByteArray)), psWorker, SLOT(parseYhcServerData(QByteArray)), Qt::QueuedConnection);
    connect(psWorker, &ParseServerDataWorker::resultReady, this, &Yhcc::dispatchData, Qt::QueuedConnection);
    psThread.start();

    updateChartsTimer = new QTimer(this);
    connect(updateChartsTimer, SIGNAL(timeout()), this, SLOT(updateCharts()));
    updateChartsTimer->start(60000);

    readDataTimer = new QTimer(this);
    connect(readDataTimer, SIGNAL(timeout()), this, SLOT(readData()));
    readDataTimer->start(1000);

    hisDlg = new HistoryDlg(this);
    alertHisDlg = new AlertHistoryDialog(this);
    runStatusDlg = new YhcRunStatusDialog(this);

    QStringList hisItems = {"压力", "转速", "电流", "电压"};
    hisDlg->setQueryItems(hisItems);

    ui->ylzsChartWidget->setRange(300, 100);
    ui->dydlChartWidget->setRange(500, 450);
}

Yhcc::~Yhcc()
{
    delete ui;
}

void Yhcc::on_yhcExitButton_clicked()
{
    close();
}

void Yhcc::on_speedDownButton_pressed()
{
    ui->speedDownButton->setStyleSheet("QPushButton#speedDownButton{background: transparent; background-image: url(:/pic/-01.png);}");
}

void Yhcc::on_speedDownButton_released()
{
    ui->speedDownButton->setStyleSheet("QPushButton#speedDownButton{background: transparent; background-image: url(:/pic/_02.png);}");
}

void Yhcc::on_speedUpButton_pressed()
{
    ui->speedUpButton->setStyleSheet("QPushButton#speedUpButton{background: transparent; background-image: url(:/pic/+01.png);}");
}

void Yhcc::on_speedUpButton_released()
{
    ui->speedUpButton->setStyleSheet("QPushButton#speedUpButton{background: transparent; background-image: url(:/pic/+02.png);}");
}

void Yhcc::on_yhDownButton_pressed()
{
    //ui->yhDownButton->setStyleSheet("QPushButton#yhDownButton{background: transparent; background-image: url(:/pic/-01.png);}");
}

void Yhcc::on_yhDownButton_released()
{
    //ui->yhDownButton->setStyleSheet("QPushButton#yhDownButton{background: transparent; background-image: url(:/pic/_02.png);}");
}

void Yhcc::on_yhUpButton_pressed()
{
    //ui->yhUpButton->setStyleSheet("QPushButton#yhUpButton{background: transparent; background-image: url(:/pic/+01.png);}");
}

void Yhcc::on_yhUpButton_released()
{
    //ui->yhUpButton->setStyleSheet("QPushButton#yhUpButton{background: transparent; background-image: url(:/pic/+02.png);}");
}

void Yhcc::netStatChecked(QString type, bool state)
{
    if(type == "wlan")
    {
        if(!state)
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
        }
    }
    else if(type == "eth")
    {
        if(!state)
        {
            ui->ethLabel->setStyleSheet("QLabel#ethLabel{background: transparent;"
                                         "background-position:center;"
                                         "background-repeat:no-repeat;"
                                         "background-image: url(:/pic/no_signal.png);}");
        }
        else
        {
            ui->ethLabel->setStyleSheet("QLabel#ethLabel{background: transparent;"
                                         "background-position:center;"
                                         "background-repeat:no-repeat;"
                                         "background-image: url(:/pic/信号2.png);}");
        }
    }
}

void Yhcc::getNetState()
{
    //emit checkNetState("wlan");
    emit checkNetState("eth");
}

void Yhcc::showEvent(QShowEvent *)
{
    if(!netManageThread.isRunning())
    {
        nsmWorker = new NetStateManageWorker;
        nsmWorker->moveToThread(&netManageThread);
        connect(&netManageThread, &QThread::finished, nsmWorker, &QObject::deleteLater);
        connect(this, SIGNAL(checkNetState(QString)), nsmWorker, SLOT(checkNetState(QString)));
        connect(nsmWorker, SIGNAL(checkNetFinished(QString,bool)), this, SLOT(netStatChecked(QString,bool)));
        netManageThread.start();
    }

    QTimer::singleShot(5000, this, SLOT(getNetState()));
    if(!checkNetStateTimer->isActive())
    {
        checkNetStateTimer->start(10000);
    }

    if(!testTimer->isActive())
    {
        testTimer->start(3000);
    }
}

void Yhcc::closeEvent(QCloseEvent *)
{
    if(checkNetStateTimer->isActive())
    {
        checkNetStateTimer->stop();
    }

    if(testTimer->isActive())
    {
        testTimer->stop();
    }

    //netManageThread.requestInterruption();
    //netManageThread.quit();
    //netManageThread.wait();
}

void Yhcc::handleControllerResult()
{

}


void Yhcc::handlePlcDataUpdate(QSet<int> changedDeviceSet, QMap<float,QString> dataMap)
{
    if(changedDeviceSet.count() > 0)
    {
        parseYhcData(dataMap);
        parseYhcRunCtrData(dataMap);
    }
}

void Yhcc::wirteTestData()
{
    //controller->yhcSpeedUp(deviceIndex, 2);
    //controller->yhcStart(deviceIndex, !started);
}

void Yhcc::updateWatchs(QMap<float,QString> dataMap)
{
    /*DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(_deviceIndex);
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Yhc_Total_Voltage");
    float address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Yhc_Total_Voltage value: " << dataMap.value(address);
    float totalVoltage = dataMap.value(address).toFloat();

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Total_Ampere");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Yhc_Total_Ampere value: " << dataMap.value(address);
    float totalAmpere = dataMap.value(address).toFloat();

    ui->yhcWatchsWidget->updateDydl(totalVoltage, totalAmpere);

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Hs_Tempture");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Yhc_Hs_Tempture value: " << dataMap.value(address);
    float hsTempture = dataMap.value(address).toFloat();

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Hs_Oil_Level");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Yhc_Hs_Oil_Level value: " << dataMap.value(address);
    float oilLevel = dataMap.value(address).toFloat();

    ui->yhcWatchsWidget->updateWdyw(hsTempture, oilLevel);

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Pressure");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Yhc_Pressure value: " << dataMap.value(address);
    float yhcPress = dataMap.value(address).toFloat();*/

    /*deviceNode = Global::getYhcNodeInfoByName("Yhc_Speed");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Yhc_Speed value: " << dataMap.value(address);
    float yhcSpeed = dataMap.value(address).toFloat();*/

    //ui->yhcWatchsWidget->updateYlzs(yhcPress, 0);
}

void Yhcc::showData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {
        emit serverDataReceived(data);
    }
}

void Yhcc::localServerConnected(bool isConnected)
{
    if(isServerConnected != isConnected)
    {
        isServerConnected = isConnected;
        emit serverConnectionChanged(isConnected);
    }

    if(isConnected)
    {
        StreamPack bpack;
        bpack = {sizeof(StreamPack),YHC,0,r_AllCacheData,String,0,0,0,0,0,0};
        getAllDataTcpClient->sendRequest(bpack);
    }
}

void Yhcc::dispatchData(QSet<int> changedDeviceSet, QMap<float, QString> dataMap)
{
    emit dataUpdate(changedDeviceSet,dataMap);
    qDebug() << "Dispatch server data";

    uint startAddrss = Global::yhcDeviceInfo.Runctr_Address;
    uint valueNumber = Global::yhcDeviceInfo.Runctr_Num;
    QVector<bool> boolValues;
    for(uint i=0; i < valueNumber; i++)
    {
        uint step = i / 8;
        uint temp = i % 8;
        float index = float(temp) / 10;
        float dictAddress = index + startAddrss + step;
        QVariant tempValue = dataMap[dictAddress];
        boolValues.append(tempValue.toBool());

        DeviceNode deviceNode = Global::getYhcNodeInfoByRunctrAddress(dictAddress);

        if(!(deviceNode.Name=="NULL"))
        {
            if(!Global::currentYhcDataMap.contains(dictAddress))
            {
                Global::currentYhcDataMap.insert(dictAddress,dataMap[dictAddress]);
            }
            else
            {
                if(Global::currentYhcDataMap[dictAddress] != dataMap[dictAddress])
                {
                    uint tankIndex = i / Global::yhcDeviceInfo.RunCtr_Block_Size;
                    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(tankIndex);

                    try
                    {
                        QList<QStandardItem *> newItemList;
                        QList<QStandardItem *> newSimpleItemList;
                        Global::alertIndex += 1;
                        QString simpleAlert;

                        qDebug() << "QDateTime::currentDateTime()" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                        newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                        newItemList.append(new QStandardItem(QString::number((tankIndex + info.startIndex)+1)));
                        if(tempValue.toBool())
                        {
                            newItemList.append(new QStandardItem(Global::yhcRunCtrDeviceNodes[i % Global::yhcDeviceInfo.RunCtr_Block_Size].Alert1));
                            simpleAlert = QString::number(Global::alertIndex) + ": " +
                                    QString::number(tankIndex+1) + "#" +
                                    Global::yhcRunCtrDeviceNodes[i % Global::yhcDeviceInfo.RunCtr_Block_Size].Alert1 + " " +
                                    QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                        }
                        else
                        {
                            newItemList.append(new QStandardItem(Global::yhcRunCtrDeviceNodes[i % Global::yhcDeviceInfo.RunCtr_Block_Size].Alert0));
                            simpleAlert = QString::number(Global::alertIndex) + ": " +
                                    QString::number(tankIndex+1) + "#" +
                                    Global::yhcRunCtrDeviceNodes[i % Global::yhcDeviceInfo.RunCtr_Block_Size].Alert0 + " " +
                                    QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

                        }
                        QStandardItem *simpleAlertItem = new QStandardItem(simpleAlert);
                        newSimpleItemList.append(simpleAlertItem);

                        if(Identity::getInstance()->getUser() != Q_NULLPTR)
                        {
                            newItemList.append(new QStandardItem(Identity::getInstance()->getUser()->getUsername()));
                        }
                        else
                        {
                            newItemList.append(new QStandardItem(""));
                        }


                        if(UiGlobal::simpleAlertsModel->rowCount() > 200)
                        {
                           UiGlobal::simpleAlertsModel->removeRow(UiGlobal::simpleAlertsModel->rowCount() - 1);
                        }
                        UiGlobal::simpleAlertsModel->insertRow(0, newSimpleItemList);


                        if(Global::getYhcNodeInfoByRunctrAddress(dictAddress).Priority == 1)
                        {
                            if(UiGlobal::alertsModel->rowCount() > 200)
                            {
                               UiGlobal::alertsModel->removeRow(UiGlobal::alertsModel->rowCount() - 1);
                            }
                            UiGlobal::alertsModel->insertRow(0, newItemList);
                        }
                    }
                    catch(exception ex)
                    {
                        qDebug() << "ex.what" << ex.what();
                    }

                    Global::currentYhcDataMap[dictAddress] = dataMap[dictAddress];
                }
            }
        }

    }
}

void Yhcc::updateData(QSet<int> changedDeviceSet, QMap<float, QString> dataMap)
{
    if(changedDeviceSet.count() > 0)
    {
        parseYhcData(dataMap);
        parseYhcRunCtrData(dataMap);
    }
}

void Yhcc::readData()
{
    getServerConnectStateTcpClient->sendTestConnectRequest();
}

void Yhcc::parseYhcData(QMap<float, QString> dataMap)
{
    /*DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(_deviceIndex);
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Yhc_Walking_Speed");
    float address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Yhc_Walking_Speed value: " << dataMap.value(address);
    currentSpeed = dataMap.value(address).toFloat();
    ui->speedLabel->setText(dataMap.value(address));

    updateWatchs(dataMap);*/
}

void Yhcc::parseYhcRunCtrData(QMap<float, QString> dataMap)
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

void Yhcc::on_speedDownButton_clicked()
{
    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(_deviceIndex);

    StreamPack bpack;
    bpack = {sizeof(StreamPack),6,0,W_Send_Control,UShort,0,0,1,0,0,0};
    //Length of ushort address and value, plus length of scrc.
    bpack.bDataLength = 1;
    bpack.bStreamLength += (2+2)*1 + 4;

    QList<ushort> addrs;
    QList<ushort> values;
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Yhc_Walking_Speed");
    ushort addr = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    addrs.append(addr);
    ushort data = currentSpeed - 1;
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

    foreach(ushort item, values)
    {
        out << item;
    }

    SData.insert(0, allPackData);

    uint scrc = actionTcpClient->StreamLen_CRC32(SData);

    QDataStream out1(&crcData,QIODevice::WriteOnly);
    out1.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out1.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out1.setByteOrder(QDataStream::LittleEndian);
    out1 << scrc;

    SData.append(crcData);

    actionTcpClient->sendRequestWithResults(SData);
}

void Yhcc::on_speedUpButton_clicked()
{
    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(_deviceIndex);

    StreamPack bpack;
    bpack = {sizeof(StreamPack),6,0,W_Send_Control,UShort,0,0,1,0,0,0};
    //Length of ushort address and value, plus length of scrc.
    bpack.bDataLength = 1;
    bpack.bStreamLength += (2+2)*1 + 4;

    QList<ushort> addrs;
    QList<ushort> values;
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Yhc_Walking_Speed");
    ushort addr = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    addrs.append(addr);
    ushort data = currentSpeed + 1;
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

    foreach(ushort item, values)
    {
        out << item;
    }

    SData.insert(0, allPackData);

    uint scrc = actionTcpClient->StreamLen_CRC32(SData);

    QDataStream out1(&crcData,QIODevice::WriteOnly);
    out1.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out1.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out1.setByteOrder(QDataStream::LittleEndian);
    out1 << scrc;

    SData.append(crcData);

    actionTcpClient->sendRequestWithResults(SData);
}

void Yhcc::on_historyButton_clicked()
{
    hisDlg->show();
}

void Yhcc::updateCharts()
{
    /*QDateTime currentdt = QDateTime::currentDateTime();
    uint stime =currentdt.toTime_t();
    HistData data;

    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(_deviceIndex);
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Yhc_Pressure");
    float address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index = Global::convertAddressToIndex(address, deviceNode.DataType);
    float yhcPress = Global::currentYhcDataMap.value(address).toFloat();

    data.address = address;
    strcpy(data.dataType, deviceNode.DataType.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.deviceIndex = index;
    data.address = address;
    data.index = 0;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Yhc_Pressure").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);*/

    /*deviceNode = Global::getYhcNodeInfoByName("Yhc_Speed");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    float yhcSpeed = Global::currentYhcDataMap.value(address).toFloat();

    data.address = address;
    strcpy(data.dataType, deviceNode.DataType.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.address = address;
    data.deviceIndex = index;
    data.index = 0;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Yhc_Speed").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);*/

    /*ui->ylzsChartWidget->updateUI(yhcPress, 0);

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Total_Voltage");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    float totalVotage = Global::currentYhcDataMap.value(address).toFloat();

    data.address = address;
    strcpy(data.dataType, deviceNode.DataType.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.address = address;
    data.deviceIndex = index;
    data.index = 0;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Yhc_Total_Voltage").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);

    deviceNode = Global::getYhcNodeInfoByName("Yhc_Total_Ampere");
    address = deviceNode.Offset + (info.offset + _deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    float totalAmpre = Global::currentYhcDataMap.value(address).toFloat();

    data.address = address;
    strcpy(data.dataType, deviceNode.DataType.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.address = address;
    data.deviceIndex = index;
    data.index = 0;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Yhc_Total_Ampere").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);

    ui->dydlChartWidget->updateUI(totalVotage, totalAmpre);*/
}

void Yhcc::on_alertButton_clicked()
{
    alertHisDlg->show();
}

void Yhcc::on_assistButton_clicked()
{
    runStatusDlg->show();
}
