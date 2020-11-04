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

    //ui->update_xml_pushButton->setVisible(false);
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
    m_nTimerId = startTimer(1000);
}

MixerDlg::~MixerDlg()
{
    if ( m_nTimerId != 0 )
            killTimer(m_nTimerId);
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
    /*if(yxywLow)
    {
        if(stateFlag)
        {
            ui->ING_BIN_HS_OIL_LEVEL_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
        }
        else
        {
            ui->ING_BIN_HS_OIL_LEVEL_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 255, 0)");
        }
    }*/


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

void MixerDlg::timerEvent(QTimerEvent *event)
{
    ui->time_label->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->date_label->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    int weekday = QDateTime::currentDateTime().date().dayOfWeek();
    switch (weekday) {
    case 1:
        ui->week_label->setText(QStringLiteral("星期一"));
        break;
    case 2:
        ui->week_label->setText(QStringLiteral("星期二"));
        break;
    case 3:
        ui->week_label->setText(QStringLiteral("星期三"));
        break;
    case 4:
        ui->week_label->setText(QStringLiteral("星期四"));
        break;
    case 5:
        ui->week_label->setText(QStringLiteral("星期五"));
        break;
    case 6:
        ui->week_label->setText(QStringLiteral("星期六"));
        break;
    case 7:
        ui->week_label->setText(QStringLiteral("星期日"));
        break;
    default:
        break;
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
    deviceNode = Global::getMixNodeInfoByName("ING_SPIRAL_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->ING_SPIRAL_RATE_label->setText(dataMap.value(address));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_WHEEL_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->ING_WHEEL_RATE_label->setText(dataMap.value(address));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_SPIRAL_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->BM_SPIRAL_RATE_label->setText(dataMap.value(address));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_WHEEL_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->BM_WHEEL_RATE_label->setText(dataMap.value(address));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("MIXER_SPIRAL_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->MIXER_SPIRAL_RATE_label->setText(dataMap.value(address));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_SPIRAL_RATE_SETTING");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->ING_SPIRAL_RATE_SETTING_label->setText(dataMap.value(address));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_SPIRAL_RATE_SETTING");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->BM_SPIRAL_RATE_SETTING_label->setText(dataMap.value(address));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("CM_RATE_SETTING");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->CM_RATE_SETTING_label->setText(dataMap.value(address));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("TOTAL_CURRENT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->TOTAL_CURRENT_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_SPIRAL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->ING_SPIRAL_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_WHEEL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->ING_WHEEL_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_SPIRAL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->BM_SPIRAL_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_WHEEL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->BM_WHEEL_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("TM_1_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->TM_1_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("LS_1_TEMP");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->LS_1_TEMP_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("LS_1_LEVEL");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->LS_1_LEVEL_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SY_1_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->SY_1_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SY_2_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->SY_2_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SB_CP_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->SB_CP_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SB_CM_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->SB_CM_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SB_SPIRAL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->SB_SPIRAL_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("MIXER_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->MIXER_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("LS_2_TEMP");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->LS_2_TEMP_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("LS_2_LEVEL");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->LS_2_LEVEL_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("TM_2_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->TM_2_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("TM_3_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->TM_3_PRESSURE_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("LS_3_TEMP");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->LS_3_TEMP_label->setText(QString::number(value, 'f', 3));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("LS_3_LEVEL");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    value = dataMap.value(address).toFloat();
    ui->LS_3_LEVEL_label->setText(QString::number(value, 'f', 3));
}

void MixerDlg::parseRunCtrData(QMap<float, QString> dataMap)
{
    bool emRun, emFault, coro1, coro2, inve1, inve2, unloading, value;

    emRun = Global::getMixRunctrValueByName(0, "ING_SPIRAL_EM_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "ING_SPIRAL_EM_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "IB_1_SPIRAL_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "IB_1_SPIRAL_INVE", dataMap);
    coro2 = Global::getMixRunctrValueByName(0, "IB_2_SPIRAL_CORO", dataMap);
    inve2 = Global::getMixRunctrValueByName(0, "IB_2_SPIRAL_INVE", dataMap);
    ui->spiral_em_widget->setStatus(emRun, emFault, coro1, inve1,coro2, inve2);

    emRun = Global::getMixRunctrValueByName(0, "ING_WHEEL_EM_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "ING_WHEEL_EM_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "IB_1_WHEEL_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "IB_1_WHEEL_INVE", dataMap);
    coro2 = Global::getMixRunctrValueByName(0, "IB_2_WHEEL_CORO", dataMap);
    inve2 = Global::getMixRunctrValueByName(0, "IB_2_WHEEL_INVE", dataMap);
    ui->wheel_em_widget->setStatus(emRun, emFault, coro1, inve1, coro2, inve2);

    emRun = Global::getMixRunctrValueByName(0, "CONVEYER_1_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "CONVEYER_1_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "TM_1_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "TM_1_INVE", dataMap);
    unloading = (coro1 || inve1)  & emRun;
    ui->sp_em_1_widget->setStatus(emRun, emFault, coro1, inve1, unloading);

    emRun = Global::getMixRunctrValueByName(0, "SLUG_1_1_CY_EM_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "SLUG_1_1_CY_EM_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "CY_1_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "CY_1_INVE", dataMap);
    coro2 = Global::getMixRunctrValueByName(0, "CY_2_CORO", dataMap);
    inve2 = Global::getMixRunctrValueByName(0, "CY_2_INVE", dataMap);
    ui->cy_em_1_widget->setStatus(emRun, emFault, coro1, inve1,coro2, inve2);

    emRun = Global::getMixRunctrValueByName(0, "SLUG_CP_EM_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "SLUG_CP_EM_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "SLUG_CP_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "SLUG_CP_INVE", dataMap);
    unloading = (coro1 || inve1)  & emRun;
    ui->cp_em_1_widget->setStatus(emRun, emFault, coro1, inve1, unloading);

    emRun = Global::getMixRunctrValueByName(0, "SLUG_CM_EM_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "SLUG_CM_EM_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "CM_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "CM_INVE", dataMap);
    unloading = (coro1 || inve1)  & emRun;
    ui->cs_em_1_widget->setStatus(emRun, emFault, coro1, inve1, unloading);

    emRun = Global::getMixRunctrValueByName(0, "SLUG_SPIRAL_1_EM_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "SLUG_SPIRAL_1_EM_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "SLUG_SPIRAL_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "SLUG_SPIRAL_INVE", dataMap);
    unloading = (coro1 || inve1)  & emRun;
    ui->spiral_em_1_widget->setStatus(emRun, emFault, coro1, inve1, unloading);

    emRun = Global::getMixRunctrValueByName(0, "CONVEYER_2_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "CONVEYER_2_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "TM_2_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "TM_2_INVE", dataMap);
    unloading = (coro1 || inve1)  & emRun;
    ui->sp_em_1_widget->setStatus(emRun, emFault, coro1, inve1, unloading);

    emRun = Global::getMixRunctrValueByName(0, "CONVEYER_3_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "CONVEYER_3_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "TM_3_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "TM_3_INVE", dataMap);
    unloading = (coro1 || inve1)  & emRun;
    ui->sp_em_1_widget->setStatus(emRun, emFault, coro1, inve1, unloading);

    emRun = Global::getMixRunctrValueByName(0, "MIXER_EM_RUN", dataMap);
    emFault = Global::getMixRunctrValueByName(0, "MIXER_EM_FAULT", dataMap);
    coro1 = Global::getMixRunctrValueByName(0, "MIXER_CORO", dataMap);
    inve1 = Global::getMixRunctrValueByName(0, "MIXER_INVE", dataMap);
    unloading = (coro1 || inve1)  & emRun;
    ui->mix_em_widget->setStatus(emRun, emFault, coro1, inve1, unloading);

    /*yxywLow = Global::getMixRunctrValueByName(deviceIndex, "ING_BIN_HS_OIL_LEVEL", dataMap);
    if(yxywLow == 1)
    {
        ui->ING_BIN_HS_OIL_LEVEL_label->setText(QStringLiteral("低"));
        ui->ING_BIN_HS_OIL_LEVEL_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
    }
    else
    {
        ui->ING_BIN_HS_OIL_LEVEL_label->setText(QStringLiteral("正常"));
        ui->ING_BIN_HS_OIL_LEVEL_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
    }*/

    shpd_1_Fault = Global::getMixRunctrValueByName(deviceIndex, "BELT_CONVEYER_FAULT", dataMap);
    if(shpd_1_Fault == 1)
    {
        ui->BW_1_STATE_label->setStyleSheet("background-image: url(:/pic/yellow_box.png);");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "BELT_CONVEYER_RUN", dataMap);
        if(value == 1)
        {
            ui->BW_1_STATE_label->setStyleSheet("background-image: url(:/pic/green_box.png);");
        }
        else
        {
            ui->BW_1_STATE_label->setStyleSheet("background-image: url(:/pic/red_box.png);");
        }
    }
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