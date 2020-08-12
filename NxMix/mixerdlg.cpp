#include "mixerdlg.h"
#include "ui_mixerdlg.h"

#include <QMap>
#include <QVector>
#include <QLabel>
#include "keyboard.h"

MixerDlg::MixerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MixerDlg)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    ui->update_xml_pushButton->setVisible(false);
    ui->settingButton->setVisible(false);

    deviceIndex = 0;

    qRegisterMetaType<Plc_Db>("Plc_Db");
    qRegisterMetaType<HistData>("HistData");

    QString eixtStyleStr="QPushButton#exitButton{background: transparent; background-image: url(:/pic/退出.png);}"
                         "QPushButton#exitButton:hover{background: transparent; background-image: url(:/pic/退出.png);}"
                         "QPushButton#exitButton:pressed{background: transparent; background-image: url(:/pic/退出.png);}";
    //ui->exitButton->setStyleSheet(eixtStyleStr);

    getServerConnectStateTcpClient = new TcpClientSocket(this);
    connect(getServerConnectStateTcpClient, SIGNAL(updateConnectState(bool)), this, SLOT(localServerConnected(bool)));
    getAllDataTcpClient = new TcpClientSocket(this);
    connect(getAllDataTcpClient, SIGNAL(updateClients(QByteArray)), this, SLOT(showData(QByteArray)));
    actionTcpClient = new TcpClientSocket(this);

    connect(this, SIGNAL(dataUpdate(QSet<int>, QMap<float,QString>)), this,SLOT(updateData(QSet<int>, QMap<float,QString>)));

    checkNetStateTimer = new QTimer(this);
    connect(checkNetStateTimer, SIGNAL(timeout()), this, SLOT(getNetState()));

    psWorker = new ParseServerDataWorker;
    psWorker->moveToThread(&psThread);
    connect(&psThread, &QThread::finished, psWorker, &QObject::deleteLater);
    connect(this, SIGNAL(serverDataReceived(QByteArray)), psWorker, SLOT(parseMixServerData(QByteArray)), Qt::QueuedConnection);
    connect(psWorker, &ParseServerDataWorker::resultReady, this, &MixerDlg::dispatchData, Qt::QueuedConnection);
    psThread.start();

    testTimer = new QTimer(this);
    connect(testTimer, SIGNAL(timeout()), this, SLOT(wirteTestData()));

    switchStateTimer = new QTimer(this);
    connect(switchStateTimer, SIGNAL(timeout()), this, SLOT(switchState()));

    readDataTimer = new QTimer(this);
    connect(readDataTimer, SIGNAL(timeout()), this, SLOT(readData()));
    readDataTimer->start(500);

    mixSettingDlg = new MixSettingDialog();
    alertHisDlg = new AlertHistoryDialog(this);
    csDlg = new ControStatusDialog();
    connect(this, SIGNAL(dataUpdate(QSet<int>,QMap<float,QString>)),csDlg,SLOT(handlePlcDataUpdate(QSet<int>,QMap<float,QString>)));

    mixRangeSetDlg = new MixDataRangeSetDialog();

    //controller = Syscontroller::getInstance(Global::systemConfig.deviceType, Global::systemConfig.deviceGroup);
    //if(controller != Q_NULLPTR)
    //{
        //connect(controller, SIGNAL(resultReady()), this, SLOT(handleControllerResult()));
        //connect(controller, &Syscontroller::plcDbUpdated, this, &MixerDlg::handlePlcDataUpdate);
    //}
}

MixerDlg::~MixerDlg()
{
    delete ui;
}

void MixerDlg::netStatChecked(QString type, bool state)
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

void MixerDlg::getNetState()
{
    //emit checkNetState("wlan");
    emit checkNetState("eth");
}

void MixerDlg::handleControllerResult()
{

}

void MixerDlg::handlePlcDataUpdate(QSet<int> changedDeviceSet, QMap<float, QString> dataMap)
{
    if(changedDeviceSet.count() > 0)
    {
        parseData(dataMap);
        parseRunCtrData(dataMap);
    }
}

void MixerDlg::wirteTestData()
{
    //controller->yhcSpeedUp(deviceIndex, 2);
    //started = !started;
    //controller->yhcStart(deviceIndex, started);
}

void MixerDlg::switchState()
{
    if(yxywLow)
    {
        if(stateFlag)
        {
            ui->ING_BIN_HS_OIL_LEVEL_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
        }
        else
        {
            ui->ING_BIN_HS_OIL_LEVEL_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 255, 0)");
        }
    }


    stateFlag = !stateFlag;
}

void MixerDlg::showData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {
        emit serverDataReceived(data);
    }
}

void MixerDlg::localServerConnected(bool isConnected)
{
    if(isServerConnected != isConnected)
    {
        isServerConnected = isConnected;
        emit serverConnectionChanged(isConnected);
    }

    if(isConnected)
    {
        StreamPack bpack;
        bpack = {sizeof(StreamPack),MIX,0,r_AllCacheData,String,0,0,0,0,0,0};
        getAllDataTcpClient->sendRequest(bpack);
    }
}

void MixerDlg::dispatchData(QSet<int> changedDeviceSet, QMap<float, QString> dataMap)
{
    emit dataUpdate(changedDeviceSet,dataMap);
    qDebug() << "Dispatch server data";

    uint startAddrss = Global::mixDeviceInfo.Runctr_Address;
    uint valueNumber = Global::mixDeviceInfo.Runctr_Num;
    QVector<bool> boolValues;
    for(uint i=0; i < valueNumber; i++)
    {
        uint step = i / 8;
        uint temp = i % 8;
        float index = float(temp) / 10;
        float dictAddress = index + startAddrss + step;
        QVariant tempValue = dataMap[dictAddress];
        boolValues.append(tempValue.toBool());

        if(!Global::currentMixDataMap.contains(dictAddress))
        {
            Global::currentMixDataMap.insert(dictAddress,dataMap[dictAddress]);
        }
        else
        {
            if(Global::currentMixDataMap[dictAddress] != dataMap[dictAddress]
                    && Global::getMixNodeInfoByRunctrAddress(dictAddress).Priority == 1)
            {
                uint tankIndex = i / Global::mixDeviceInfo.RunCtr_Block_Size;
                DeviceGroupInfo info = Global::getMixDeviceGroupInfo(tankIndex);

                QList<QStandardItem *> newItemList;
                QList<QStandardItem *> newSimpleItemList;
                Global::alertIndex += 1;
                QString simpleAlert;

                try
                {
                    qDebug() << "QDateTime::currentDateTime()" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                    newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                    newItemList.append(new QStandardItem(QString::number((tankIndex + info.startIndex)+1)));
                    if(tempValue.toBool())
                    {
                        newItemList.append(new QStandardItem(Global::mixRunCtrDeviceNodes[i % Global::mixDeviceInfo.RunCtr_Block_Size].Alert1));
                        simpleAlert = QString::number(Global::alertIndex) + ": " +
                                QString::number(tankIndex+1) + "#" +
                                Global::mixRunCtrDeviceNodes[i % Global::mixDeviceInfo.RunCtr_Block_Size].Alert1 + " " +
                                QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                    }
                    else
                    {
                        newItemList.append(new QStandardItem(Global::mixRunCtrDeviceNodes[i % Global::mixDeviceInfo.RunCtr_Block_Size].Alert0));
                        simpleAlert = QString::number(Global::alertIndex) + ": " +
                                QString::number(tankIndex+1) + "#" +
                                Global::mixRunCtrDeviceNodes[i % Global::mixDeviceInfo.RunCtr_Block_Size].Alert0 + " " +
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

                    if(UiGlobal::alertsModel->rowCount() > 200)
                    {
                       UiGlobal::alertsModel->removeRow(UiGlobal::alertsModel->rowCount() - 1);
                    }
                    UiGlobal::alertsModel->insertRow(0, newItemList);
                }
                catch(exception ex)
                {
                    qDebug() << "ex.what" << ex.what();
                }
            }
            Global::currentMixDataMap[dictAddress] = dataMap[dictAddress];
        }
    }
}

void MixerDlg::updateData(QSet<int> changedDeviceSet, QMap<float, QString> dataMap)
{
    if(changedDeviceSet.count() > 0)
    {
        parseData(dataMap);
        parseRunCtrData(dataMap);
    }
}

void MixerDlg::readData()
{
    getServerConnectStateTcpClient->sendTestConnectRequest();
}

void MixerDlg::showEvent(QShowEvent *)
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

    /*if(!testTimer->isActive())
    {
        testTimer->start(5000);
    }*/

    if(!switchStateTimer->isActive())
    {
        switchStateTimer->start(200);
    }
}

void MixerDlg::closeEvent(QCloseEvent *)
{
    if(checkNetStateTimer->isActive())
    {
        checkNetStateTimer->stop();
    }

    /*if(testTimer->isActive())
    {
        testTimer->stop();
    }*/

    if(switchStateTimer->isActive())
    {
        switchStateTimer->stop();
    }
}

void MixerDlg::parseData(QMap<float, QString> dataMap)
{
    float value = 0.0;
    DeviceGroupInfo info = Global::getMixDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getMixNodeInfoByName("ING_BIN_CURRENT_WEIGHT");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->ING_BIN_CURRENT_WEIGHT_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_BIN_OUTPUT_WEIGHT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->ING_BIN_OUTPUT_WEIGHT_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_BIN_OUTPUT_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->ING_BIN_OUTPUT_RATE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_SPIRAL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->ING_SPIRAL_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_SPIRAL_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->ING_SPIRAL_RATE_label->setText(dataMap.value(address));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_WHEEL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->ING_WHEEL_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_WHEEL_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->ING_WHEEL_RATE_label->setText(dataMap.value(address));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_SPIRAL_RATE_SETTING");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->ING_SPIRAL_RATE_SETTING_label->setText(dataMap.value(address));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_BIN_CURRENT_WEIGHT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->BM_BIN_CURRENT_WEIGHT_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_BIN_OUTPUT_WEIGHT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->BM_BIN_OUTPUT_WEIGHT_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_BIN_OUTPUT_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->BM_BIN_OUTPUT_RATE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_SPIRAL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->BM_SPIRAL_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_SPIRAL_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->BM_SPIRAL_RATE_label->setText(dataMap.value(address));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_WHEEL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->BM_WHEEL_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_WHEEL_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->BM_WHEEL_RATE_label->setText(dataMap.value(address));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_SPIRAL_RATE_SETTING");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->BM_SPIRAL_RATE_SETTING_label->setText(dataMap.value(address));



    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_BIN_1_CURRENT_WEIGHT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->SLUG_BIN_1_CURRENT_WEIGHT_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_BIN_1_OUTPUT_WEIGHT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->SLUG_BIN_1_OUTPUT_WEIGHT_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_BIN_1_CURRENT_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->SLUG_BIN_1_CURRENT_RATE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_BIN_2_CURRENT_WEIGHT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->SLUG_BIN_2_CURRENT_WEIGHT_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_BIN_2_OUTPUT_WEIGHT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->SLUG_BIN_2_OUTPUT_WEIGHT_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_BIN_2_CURRENT_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->SLUG_BIN_2_CURRENT_RATE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_BIN_HS_FT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->ING_BIN_HS_FT_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("TOTAL_CURRENT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->TOTAL_CURRENT_label->setText(QString::number(value, 'f', 3));
}

void MixerDlg::parseRunCtrData(QMap<float, QString> dataMap)
{
    bool emRun, emFault, coro1, coro2, inve1, inve2, unloading, value;

    emRun = Global::getMixRunctrValueByName(0, "SLUG_SPIRAL_1_EM_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "SLUG_SPIRAL_1_EM_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "SLUG_1_1_CY_EM_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "SLUG_1_1_CY_EM_INVE", dataMap);
    coro2 = Global::getMixRunctrValueByName(0, "SLUG_1_2_CY_EM_CORO", dataMap);
    inve2 = Global::getMixRunctrValueByName(0, "SLUG_1_2_CY_EM_INVE", dataMap);
    ui->cy_em_1_widget->setStatus(emRun, emFault, coro1, inve1,coro2, inve2);

    emRun = Global::getMixRunctrValueByName(0, "SLUG_SPIRAL_2_EM_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "SLUG_SPIRAL_2_EM_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "SLUG_2_1_CY_EM_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "SLUG_2_1_CY_EM_INVE", dataMap);
    coro2 = Global::getMixRunctrValueByName(0, "SLUG_2_2_CY_EM_CORO", dataMap);
    inve2 = Global::getMixRunctrValueByName(0, "SLUG_2_2_CY_EM_INVE", dataMap);
    ui->cy_em_2_widget->setStatus(emRun, emFault, coro1, inve1, coro2, inve2);

    emRun = Global::getMixRunctrValueByName(0, "ING_SPIRAL_EM_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "ING_SPIRAL_EM_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "ING_SPIRAL_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "ING_SPIRAL_INVE", dataMap);
    coro2 = Global::getMixRunctrValueByName(0, "BM_SPIRAL_CORO", dataMap);
    inve2 = Global::getMixRunctrValueByName(0, "BM_SPIRAL_INVE", dataMap);
    ui->spiral_em_widget->setStatus(emRun, emFault, coro1, inve1, coro2, inve2);

    emRun = Global::getMixRunctrValueByName(0, "ING_WHEEL_EM_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "ING_WHEEL_EM_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "ING_WHEEL_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "ING_WHEEL_INVE", dataMap);
    coro2 = Global::getMixRunctrValueByName(0, "BM_WHEEL_CORO", dataMap);
    inve2 = Global::getMixRunctrValueByName(0, "BM_WHEEL_INVE", dataMap);
    ui->wheel_em_widget->setStatus(emRun, emFault, coro1, inve1, coro2, inve2);

    emRun = Global::getMixRunctrValueByName(0, "BM_WHEEL_EM_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "BM_WHEEL_EM_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "MIXER_SPIRAL_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "MIXER_SPIRAL_INVE", dataMap);
    unloading = unloading & (coro1 || inve1)  & emRun;
    ui->sp_em_1_widget->setStatus(emRun, emFault, coro1, inve1, unloading);

    yxywLow = Global::getMixRunctrValueByName(deviceIndex, "ING_BIN_HS_OIL_LEVEL", dataMap);
    if(yxywLow == 1)
    {
        ui->ING_BIN_HS_OIL_LEVEL_label->setText(QStringLiteral("低"));
        ui->ING_BIN_HS_OIL_LEVEL_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
    }
    else
    {
        ui->ING_BIN_HS_OIL_LEVEL_label->setText(QStringLiteral("正常"));
        ui->ING_BIN_HS_OIL_LEVEL_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
    }

    shpd_3_Fault = Global::getMixRunctrValueByName(deviceIndex, "CONVEYER_3_FAULT", dataMap);
    if(shpd_3_Fault == 1)
    {
        ui->CONVEYER_3_RUN_label->setStyleSheet("background-image: url(:/pic/yellow_box.png);");
    }
    else
    {
        coro1 = Global::getMixRunctrValueByName(deviceIndex, "CONVEYER_3_CORO", dataMap);
        inve1 = Global::getMixRunctrValueByName(deviceIndex, "CONVEYER_3_INVE", dataMap);

        if(coro1 == 1)
        {
            ui->CONVEYER_3_RUN_label->setStyleSheet("background-image: url(:/pic/green_box.png);");
        }
        if (inve1 == 1)
        {
            ui->CONVEYER_3_RUN_label->setStyleSheet("background-image: url(:/pic/deep_green_box.png);");
        }
        if(!coro1 && !inve1)
        {
            ui->CONVEYER_3_RUN_label->setStyleSheet("background-image: url(:/pic/red_box.png);");
        }
    }

    shpd_1_Fault = Global::getMixRunctrValueByName(deviceIndex, "CONVEYER_1_EM_FAULT", dataMap);
    if(shpd_1_Fault == 1)
    {
        ui->CONVEYER_1_RUN_label->setStyleSheet("background-image: url(:/pic/yellow_box.png);");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "CONVEYER_1_EM_RUN", dataMap);
        if(value == 1)
        {
            ui->CONVEYER_1_RUN_label->setStyleSheet("background-image: url(:/pic/green_box.png);");
        }
        else
        {
            ui->CONVEYER_1_RUN_label->setStyleSheet("background-image: url(:/pic/red_box.png);");
        }
    }

    shpd_2_Fault = Global::getMixRunctrValueByName(deviceIndex, "CONVEYER_2_FAULT", dataMap);
    if(shpd_2_Fault == 1)
    {
        ui->CONVEYER_2_RUN_label->setStyleSheet("background-image: url(:/pic/yellow_box.png);");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "CONVEYER_2_RUN", dataMap);
        if(value == 1)
        {
            ui->CONVEYER_2_RUN_label->setStyleSheet("background-image: url(:/pic/green_box.png);");
        }
        else
        {
            ui->CONVEYER_2_RUN_label->setStyleSheet("background-image: url(:/pic/red_box.png);");
        }
    }

    /*bool value;

    wnyxywLow = Global::getMixRunctrValueByName(deviceIndex, "SLUG_BIN_LEVEL_SIG", dataMap);
    if(wnyxywLow == 1)
    {
        ui->SLUG_BIN_LEVEL_SIG_label->setText(QStringLiteral("低"));
        ui->SLUG_BIN_LEVEL_SIG_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
    }
    else
    {
        ui->SLUG_BIN_LEVEL_SIG_label->setText(QStringLiteral("正常"));
        ui->SLUG_BIN_LEVEL_SIG_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
    }

    wnjrFault = Global::getMixRunctrValueByName(deviceIndex, "SLUG_BIN_HEATER_FAULT", dataMap);
    if(wnjrFault == 1)
    {
        ui->SLUG_BIN_HEATER_RUN_label->setText(QStringLiteral("故障"));
        ui->SLUG_BIN_HEATER_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 255, 0)");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "SLUG_BIN_HEATER_RUN", dataMap);
        if(value == 1)
        {
            ui->SLUG_BIN_HEATER_RUN_label->setText(QStringLiteral("加热中"));
            ui->SLUG_BIN_HEATER_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->SLUG_BIN_HEATER_RUN_label->setText(QStringLiteral("加热停止"));
            ui->SLUG_BIN_HEATER_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
        }
    }

    flyxywLow = Global::getMixRunctrValueByName(deviceIndex, "ING_BIN_LEVEL_SIG", dataMap);
    if(flyxywLow == 1)
    {
        ui->ING_BIN_LEVEL_SIG_label->setText(QStringLiteral("低"));
        ui->ING_BIN_LEVEL_SIG_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
    }
    else
    {
        ui->ING_BIN_LEVEL_SIG_label->setText(QStringLiteral("正常"));
        ui->ING_BIN_LEVEL_SIG_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
    }

    hljFault = Global::getMixRunctrValueByName(deviceIndex, "MIX_MA_FAULT", dataMap);
    if(hljFault == 1)
    {
        ui->MIX_MA_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 255, 0)");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "MIX_MA_RUN", dataMap);
        if(value == 1)
        {
            ui->MIX_MA_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->MIX_MA_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
        }
    }

    flczdFault = Global::getMixRunctrValueByName(deviceIndex, "ING_BIN_RIDL_FAULT", dataMap);
    if(flczdFault == 1)
    {
        ui->ING_BIN_RIDL_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 255, 0)");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "ING_BIN_RIDL_RUN", dataMap);
        if(value == 1)
        {
            ui->ING_BIN_RIDL_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->ING_BIN_RIDL_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
        }
    }

    fhczdFault = Global::getMixRunctrValueByName(deviceIndex, "BM_BIN_RIDL_FAULT", dataMap);
    if(fhczdFault == 1)
    {
        ui->BM_BIN_RIDL_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 255, 0)");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "BM_BIN_RIDL_RUN", dataMap);
        if(value == 1)
        {
            ui->BM_BIN_RIDL_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->BM_BIN_RIDL_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
        }
    }

    shpd_1_Fault = Global::getMixRunctrValueByName(deviceIndex, "CONVEYER_1_FAULT", dataMap);
    if(shpd_1_Fault == 1)
    {
        ui->CONVEYER_1_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 255, 0)");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "CONVEYER_1_RUN", dataMap);
        if(value == 1)
        {
            ui->CONVEYER_1_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->CONVEYER_1_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
        }
    }

    shpd_2_Fault = Global::getMixRunctrValueByName(deviceIndex, "CONVEYER_2_FAULT", dataMap);
    if(shpd_2_Fault == 1)
    {
        ui->CONVEYER_2_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 255, 0)");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "CONVEYER_2_RUN", dataMap);
        if(value == 1)
        {
            ui->CONVEYER_2_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->CONVEYER_2_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
        }
    }

    shpd_3_Fault = Global::getMixRunctrValueByName(deviceIndex, "CONVEYER_3_FAULT", dataMap);
    if(shpd_3_Fault == 1)
    {
        ui->CONVEYER_3_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 255, 0)");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "CONVEYER_3_RUN", dataMap);
        if(value == 1)
        {
            ui->CONVEYER_3_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->CONVEYER_3_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
        }
    }

    fljrFault = Global::getMixRunctrValueByName(deviceIndex, "ING_BIN_HEATER_FAULT", dataMap);
    if(fljrFault == 1)
    {
        ui->ING_BIN_HEATER_RUN_label->setText(QStringLiteral("故障"));
        ui->ING_BIN_HEATER_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 255, 0)");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "ING_BIN_HEATER_RUN", dataMap);
        if(value == 1)
        {
            ui->ING_BIN_HEATER_RUN_label->setText(QStringLiteral("加热中"));
            ui->ING_BIN_HEATER_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->ING_BIN_HEATER_RUN_label->setText(QStringLiteral("加热停止"));
            ui->ING_BIN_HEATER_RUN_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
        }
    }

    wnhjdjFault = Global::getMixRunctrValueByName(deviceIndex, "CY_1_EM_FAULT", dataMap);
    if(wnhjdjFault == 1)
    {
        ui->wnhjdj_1_1_label->setStyleSheet("QLabel#wnhjdj_1_1_label{background-image:url(:/pic/em_fault.png)}");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "CY_1_EM_RUN", dataMap);
        if(value == 1)
        {
            ui->wnhjdj_1_1_label->setStyleSheet("QLabel#wnhjdj_1_1_label{background-image:url(:/pic/em_run.png)}");
        }
        else
        {
            ui->wnhjdj_1_1_label->setStyleSheet("QLabel#wnhjdj_1_1_label{background-image:url(:/pic/em_stop.png)}");
        }
    }

    wnzpdjFault = Global::getMixRunctrValueByName(deviceIndex, "SLUG_WHEEL_EM_FAULT", dataMap);
    if(wnzpdjFault == 1)
    {
        ui->wnzpdj_1_1_label->setStyleSheet("QLabel#wnzpdj_1_1_label{background-image:url(:/pic/em_fault.png)}");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "SLUG_WHEEL_EM_RUN", dataMap);
        if(value == 1)
        {
            ui->wnzpdj_1_1_label->setStyleSheet("QLabel#wnzpdj_1_1_label{background-image:url(:/pic/em_run.png)}");
        }
        else
        {
            ui->wnzpdj_1_1_label->setStyleSheet("QLabel#wnzpdj_1_1_label{background-image:url(:/pic/em_stop.png)}");
        }
    }


    flzpdjFault = Global::getMixRunctrValueByName(deviceIndex, "ING_WHEEL_EM_FAULT", dataMap);
    if(flzpdjFault == 1)
    {
        ui->flzpdj_2_1_label->setStyleSheet("QLabel#flzpdj_2_1_label{background-image:url(:/pic/em_fault.png)}");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "ING_WHEEL_EM_RUN", dataMap);
        if(value == 1)
        {
            ui->flzpdj_2_1_label->setStyleSheet("QLabel#flzpdj_2_1_label{background-image:url(:/pic/em_run.png)}");
        }
        else
        {
            ui->flzpdj_2_1_label->setStyleSheet("QLabel#flzpdj_2_1_label{background-image:url(:/pic/em_stop.png)}");
        }
    }

    fllxdjFault = Global::getMixRunctrValueByName(deviceIndex, "SLUG_SPIRAL_EM_FAULT", dataMap);
    if(fllxdjFault == 1)
    {
        ui->fllxdj_2_1_label->setStyleSheet("QLabel#fllxdj_2_1_label{background-image:url(:/pic/em_fault.png)}");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "SLUG_SPIRAL_EM_RUN", dataMap);
        if(value == 1)
        {
            ui->fllxdj_2_1_label->setStyleSheet("QLabel#fllxdj_2_1_label{background-image:url(:/pic/em_run.png)}");
        }
        else
        {
            ui->fllxdj_2_1_label->setStyleSheet("QLabel#fllxdj_2_1_label{background-image:url(:/pic/em_stop.png)}");
        }
    }

    fhlxdjFault = Global::getMixRunctrValueByName(deviceIndex, "BM_SPIRAL_EM_FAULT", dataMap);
    if(fhlxdjFault == 1)
    {
        ui->fhlxdj_2_2_label->setStyleSheet("QLabel#fhlxdj_2_2_label{background-image:url(:/pic/em_fault.png)}");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "BM_SPIRAL_EM_RUN", dataMap);
        if(value == 1)
        {
            ui->fhlxdj_2_2_label->setStyleSheet("QLabel#fhlxdj_2_2_label{background-image:url(:/pic/em_run.png)}");
        }
        else
        {
            ui->fhlxdj_2_2_label->setStyleSheet("QLabel#fhlxdj_2_2_label{background-image:url(:/pic/em_stop.png)}");
        }
    }


    bool isCoro = Global::getMixRunctrValueByName(deviceIndex, "CY_1_CORO", dataMap);
    if(isCoro == 1)
    {
        ui->hjtczz_1_1_label->setStyleSheet("QLabel#hjtczz_1_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->hjtczz_1_1_label->setStyleSheet("QLabel#hjtczz_1_1_label{background-image:url(:/pic/inve.png)}");
    }
    bool isInve = Global::getMixRunctrValueByName(deviceIndex, "CY_1_INVE", dataMap);
    if(isInve == 1)
    {
        ui->hjtcfz_1_1_label->setStyleSheet("QLabel#hjtcfz_1_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->hjtcfz_1_1_label->setStyleSheet("QLabel#hjtcfz_1_1_label{background-image:url(:/pic/inve.png)}");
    }

    isCoro = Global::getMixRunctrValueByName(deviceIndex, "CY_2_CORO", dataMap);
    if(isCoro == 1)
    {
        ui->hjtczz_1_2_label->setStyleSheet("QLabel#hjtczz_1_2_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->hjtczz_1_2_label->setStyleSheet("QLabel#hjtczz_1_2_label{background-image:url(:/pic/inve.png)}");
    }
    isInve = Global::getMixRunctrValueByName(deviceIndex, "CY_2_INVE", dataMap);
    if(isInve == 1)
    {
        ui->hjtcfz_1_2_label->setStyleSheet("QLabel#hjtcfz_1_2_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->hjtcfz_1_2_label->setStyleSheet("QLabel#hjtcfz_1_2_label{background-image:url(:/pic/inve.png)}");
    }


    isCoro = Global::getMixRunctrValueByName(deviceIndex, "SLUG_WHEEL_CORO", dataMap);
    if(isCoro == 1)
    {
        ui->wnzpzz_1_1_label->setStyleSheet("QLabel#wnzpzz_1_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->wnzpzz_1_1_label->setStyleSheet("QLabel#wnzpzz_1_1_label{background-image:url(:/pic/inve.png)}");
    }
    isInve = Global::getMixRunctrValueByName(deviceIndex, "SLUG_WHEEL_INVE", dataMap);
    if(isInve == 1)
    {
        ui->wnzpfz_1_1_label->setStyleSheet("QLabel#wnzpfz_1_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->wnzpfz_1_1_label->setStyleSheet("QLabel#wnzpfz_1_1_label{background-image:url(:/pic/inve.png)}");
    }
    if(isCoro || isInve)
    {
        ui->wnzp_1_1_label->setStyleSheet("QLabel#wnzp_1_1_label{background-image:url(:/pic/wheel_run.png)}");
    }
    else
    {
        ui->wnzp_1_1_label->setStyleSheet("QLabel#wnzp_1_1_label{background-image:url(:/pic/wheel_stop.png)}");
    }


    isCoro = Global::getMixRunctrValueByName(deviceIndex, "ING_WHEEL_CORO", dataMap);
    if(isCoro == 1)
    {
        ui->flzpzz_2_1_label->setStyleSheet("QLabel#flzpzz_2_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->flzpzz_2_1_label->setStyleSheet("QLabel#flzpzz_2_1_label{background-image:url(:/pic/inve.png)}");
    }
    isInve = Global::getMixRunctrValueByName(deviceIndex, "ING_WHEEL_INVE", dataMap);
    if(isInve == 1)
    {
        ui->flzpfz_2_1_label->setStyleSheet("QLabel#flzpfz_2_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->flzpfz_2_1_label->setStyleSheet("QLabel#flzpfz_2_1_label{background-image:url(:/pic/inve.png)}");
    }
    if(isCoro || isInve)
    {
        ui->flzp_2_1_label->setStyleSheet("QLabel#flzp_2_1_label{background-image:url(:/pic/wheel_run.png)}");
    }
    else
    {
        ui->flzp_2_1_label->setStyleSheet("QLabel#flzp_2_1_label{background-image:url(:/pic/wheel_stop.png)}");
    }


    isCoro = Global::getMixRunctrValueByName(deviceIndex, "BM_WHEEL_CORO", dataMap);
    if(isCoro == 1)
    {
        ui->fhzpzz_2_1_label->setStyleSheet("QLabel#fhzpzz_2_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->fhzpzz_2_1_label->setStyleSheet("QLabel#fhzpzz_2_1_label{background-image:url(:/pic/inve.png)}");
    }
    isInve = Global::getMixRunctrValueByName(deviceIndex, "BM_WHEEL_INVE", dataMap);
    if(isInve == 1)
    {
        ui->fhzpfz_2_1_label->setStyleSheet("QLabel#fhzpfz_2_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->fhzpfz_2_1_label->setStyleSheet("QLabel#fhzpfz_2_1_label{background-image:url(:/pic/inve.png)}");
    }
    if(isCoro || isInve)
    {
        ui->fhzp_2_1_label->setStyleSheet("QLabel#fhzp_2_1_label{background-image:url(:/pic/wheel_run.png)}");
    }
    else
    {
        ui->fhzp_2_1_label->setStyleSheet("QLabel#fhzp_2_1_label{background-image:url(:/pic/wheel_stop.png)}");
    }


    isCoro = Global::getMixRunctrValueByName(deviceIndex, "ING_SPIRAL_CORO", dataMap);
    if(isCoro == 1)
    {
        ui->fllxzz_2_1_label->setStyleSheet("QLabel#fllxzz_2_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->fllxzz_2_1_label->setStyleSheet("QLabel#fllxzz_2_1_label{background-image:url(:/pic/inve.png)}");
    }
    isInve = Global::getMixRunctrValueByName(deviceIndex, "ING_SPIRAL_INVE", dataMap);
    if(isInve == 1)
    {
        ui->fllxfz_2_1_label->setStyleSheet("QLabel#fllxfz_2_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->fllxfz_2_1_label->setStyleSheet("QLabel#fllxfz_2_1_label{background-image:url(:/pic/inve.png)}");
    }
    if(isCoro || isInve)
    {
        ui->fllx_2_1_label->setStyleSheet("QLabel#fllx_2_1_label{background-image:url(:/pic/wheel_run.png)}");
    }
    else
    {
        ui->fllx_2_1_label->setStyleSheet("QLabel#fllx_2_1_label{background-image:url(:/pic/wheel_stop.png)}");
    }


    isCoro = Global::getMixRunctrValueByName(deviceIndex, "SLUG_SPIRAL_CORO", dataMap);
    if(isCoro == 1)
    {
        ui->wnlxzz_2_1_label->setStyleSheet("QLabel#wnlxzz_2_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->wnlxzz_2_1_label->setStyleSheet("QLabel#wnlxzz_2_1_label{background-image:url(:/pic/inve.png)}");
    }
    isInve = Global::getMixRunctrValueByName(deviceIndex, "SLUG_SPIRAL_INVE", dataMap);
    if(isInve == 1)
    {
        ui->wnlxfz_2_1_label->setStyleSheet("QLabel#wnlxfz_2_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->wnlxfz_2_1_label->setStyleSheet("QLabel#wnlxfz_2_1_label{background-image:url(:/pic/inve.png)}");
    }
    if(isCoro || isInve)
    {
        ui->wnlx_2_1_label->setStyleSheet("QLabel#wnlx_2_1_label{background-image:url(:/pic/wheel_run.png)}");
    }
    else
    {
        ui->wnlx_2_1_label->setStyleSheet("QLabel#wnlx_2_1_label{background-image:url(:/pic/wheel_stop.png)}");
    }


    isCoro = Global::getMixRunctrValueByName(deviceIndex, "BM_SPIRAL_CORO", dataMap);
    if(isCoro == 1)
    {
        ui->fhlxzz_2_1_label->setStyleSheet("QLabel#fhlxzz_2_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->fhlxzz_2_1_label->setStyleSheet("QLabel#fhlxzz_2_1_label{background-image:url(:/pic/inve.png)}");
    }
    isInve = Global::getMixRunctrValueByName(deviceIndex, "BM_SPIRAL_INVE", dataMap);
    if(isInve == 1)
    {
        ui->fhlxfz_2_1_label->setStyleSheet("QLabel#fhlxfz_2_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->fhlxfz_2_1_label->setStyleSheet("QLabel#fhlxfz_2_1_label{background-image:url(:/pic/inve.png)}");
    }
    if(isCoro || isInve)
    {
        ui->fhlx_2_1_label->setStyleSheet("QLabel#fhlx_2_1_label{background-image:url(:/pic/wheel_run.png)}");
    }
    else
    {
        ui->fhlx_2_1_label->setStyleSheet("QLabel#fhlx_2_1_label{background-image:url(:/pic/wheel_stop.png)}");
    }*/

}

void MixerDlg::on_settingButton_clicked()
{
    mixSettingDlg->closeMsgBox();
    mixSettingDlg->close();
    mixSettingDlg->show();
    Keyboard::getInstance()->close();
}

void MixerDlg::on_faultHisButton_clicked()
{
    alertHisDlg->show();
}

void MixerDlg::on_debugButton_clicked()
{
    csDlg->close();
    csDlg->show();
}

void MixerDlg::on_update_xml_pushButton_clicked()
{
    mixRangeSetDlg->close();
    mixRangeSetDlg->show();
}
