#include "mixerdlg.h"
#include "ui_mixerdlg.h"

#include <QMap>
#include <QVector>

MixerDlg::MixerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MixerDlg)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    deviceIndex = 0;

    qRegisterMetaType<Plc_Db>("Plc_Db");
    qRegisterMetaType<HistData>("HistData");

    QString eixtStyleStr="QPushButton#exitButton{background: transparent; background-image: url(:/pic/退出.png);}"
                         "QPushButton#exitButton:hover{background: transparent; background-image: url(:/pic/退出.png);}"
                         "QPushButton#exitButton:pressed{background: transparent; background-image: url(:/pic/退出.png);}";
    ui->exitButton->setStyleSheet(eixtStyleStr);

    checkNetStateTimer = new QTimer(this);
    connect(checkNetStateTimer, SIGNAL(timeout()), this, SLOT(getNetState()));

    testTimer = new QTimer(this);
    connect(testTimer, SIGNAL(timeout()), this, SLOT(wirteTestData()));

    controller = Syscontroller::getInstance(Global::systemConfig.deviceType, Global::systemConfig.deviceGroup);
    if(controller != Q_NULLPTR)
    {
        connect(controller, SIGNAL(resultReady()), this, SLOT(handleControllerResult()));
        connect(controller, &Syscontroller::plcDbUpdated, this, &MixerDlg::handlePlcDataUpdate);
    }
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
    emit checkNetState("wlan");
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
    controller->yhcSpeedUp(deviceIndex, 2);
    controller->yhcStart(deviceIndex, !started);
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

    if(!testTimer->isActive())
    {
        testTimer->start(3000);
    }
}

void MixerDlg::closeEvent(QCloseEvent *)
{
    if(checkNetStateTimer->isActive())
    {
        checkNetStateTimer->stop();
    }

    if(testTimer->isActive())
    {
        testTimer->stop();
    }
}

void MixerDlg::parseData(QMap<float, QString> dataMap)
{
    DeviceGroupInfo info = Global::getMixDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getMixNodeInfoByName("BM_BIN_CURRENT_WEIGHT");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->BM_BIN_CURRENT_WEIGHT_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_BIN_OUTPUT_QUANTITY");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->BM_BIN_OUTPUT_QUANTITY_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_BIN_OUTPUT_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->BM_BIN_OUTPUT_RATE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_SPIRAL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->BM_SPIRAL_PRESSURE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_SPIRAL_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->BM_SPIRAL_RATE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUShort() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_WHEEL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->BM_WHEEL_PRESSURE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_WHEEL_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->BM_WHEEL_RATE_label->setText(QString::number((float) Global::currentMixDataMap[address].toShort() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("BM_SPIRAL_RATE_SETTING");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->BM_SPIRAL_RATE_SETTING_label->setText(QString::number((float) Global::currentMixDataMap[address].toUShort() / 100.0));


    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_BIN_CURRENT_WEIGHT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->SLUG_BIN_CURRENT_WEIGHT_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_BIN_OUTPUT_QUANTITY");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->SLUG_BIN_OUTPUT_QUANTITY_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_BIN_OUTPUT_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->SLUG_BIN_OUTPUT_RATE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_SPIRAL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->SLUG_SPIRAL_PRESSURE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("CY_1_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->CY_1_PRESSURE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("CY_2_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->CY_2_PRESSURE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_WHEEL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->SLUG_WHEEL_PRESSURE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_CARG_RATE_SETTING");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->SLUG_CARG_RATE_SETTING_label->setText(QString::number((float) Global::currentMixDataMap[address].toUShort() / 100.0));


    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_BIN_CURRENT_WEIGHT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->ING_BIN_CURRENT_WEIGHT_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_BIN_OUTPUT_QUANTITY");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->ING_BIN_OUTPUT_QUANTITY_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_BIN_OUTPUT_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->ING_BIN_OUTPUT_RATE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_SPIRAL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->ING_SPIRAL_PRESSURE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_SPIRAL_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->ING_SPIRAL_RATE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUShort() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_WHEEL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->ING_WHEEL_PRESSURE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_WHEEL_RATE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->ING_WHEEL_RATE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUShort() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_SPIRAL_RATE_SETTING");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->ING_SPIRAL_RATE_SETTING_label->setText(QString::number((float) Global::currentMixDataMap[address].toUShort() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_BIN_FT_TEMPTURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->SLUG_BIN_FT_TEMPTURE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("ING_BIN_FT_TEMPTURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->ING_BIN_FT_TEMPTURE_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("TOTAL_CURRENT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    ui->TOTAL_CURRENT_label->setText(QString::number((float) Global::currentMixDataMap[address].toUInt() / 100.0));
}

void MixerDlg::parseRunCtrData(QMap<float, QString> dataMap)
{
    bool value = Global::getMixRunctrValueByName(deviceIndex, "SLUG_BIN_LEVEL_SIG", dataMap);
    if(value == 1)
    {
        ui->SLUG_BIN_LEVEL_SIG_label->setText(QStringLiteral("低"));
        ui->SLUG_BIN_LEVEL_SIG_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
    }
    else
    {
        ui->SLUG_BIN_LEVEL_SIG_label->setText(QStringLiteral("正常"));
        ui->SLUG_BIN_LEVEL_SIG_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
    }

    value = Global::getMixRunctrValueByName(deviceIndex, "SLUG_BIN_HEATER_FAULT", dataMap);
    if(value == 1)
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

    value = Global::getMixRunctrValueByName(deviceIndex, "ING_BIN_LEVEL_SIG", dataMap);
    if(value == 1)
    {
        ui->ING_BIN_LEVEL_SIG_label->setText(QStringLiteral("低"));
        ui->ING_BIN_LEVEL_SIG_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
    }
    else
    {
        ui->ING_BIN_LEVEL_SIG_label->setText(QStringLiteral("正常"));
        ui->ING_BIN_LEVEL_SIG_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
    }

    value = Global::getMixRunctrValueByName(deviceIndex, "ING_BIN_HEATER_FAULT", dataMap);
    if(value == 1)
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

    value = Global::getMixRunctrValueByName(deviceIndex, "CY_1_EM_FAULT", dataMap);
    if(value == 1)
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

    value = Global::getMixRunctrValueByName(deviceIndex, "SLUG_WHEEL_EM_FAULT", dataMap);
    if(value == 1)
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


    value = Global::getMixRunctrValueByName(deviceIndex, "ING_WHEEL_EM_FAULT", dataMap);
    if(value == 1)
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

    value = Global::getMixRunctrValueByName(deviceIndex, "SLUG_SPIRAL_EM_FAULT", dataMap);
    if(value == 1)
    {
        ui->wnlsdj_2_1_label->setStyleSheet("QLabel#wnlsdj_2_1_label{background-image:url(:/pic/em_fault.png)}");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "SLUG_SPIRAL_EM_RUN", dataMap);
        if(value == 1)
        {
            ui->wnlsdj_2_1_label->setStyleSheet("QLabel#wnlsdj_2_1_label{background-image:url(:/pic/em_run.png)}");
        }
        else
        {
            ui->wnlsdj_2_1_label->setStyleSheet("QLabel#wnlsdj_2_1_label{background-image:url(:/pic/em_stop.png)}");
        }
    }

    value = Global::getMixRunctrValueByName(deviceIndex, "BM_SPIRAL_EM_FAULT", dataMap);
    if(value == 1)
    {
        ui->fhlsdj_2_2_label->setStyleSheet("QLabel#fhlsdj_2_2_label{background-image:url(:/pic/em_fault.png)}");
    }
    else
    {
        value = Global::getMixRunctrValueByName(deviceIndex, "BM_SPIRAL_EM_RUN", dataMap);
        if(value == 1)
        {
            ui->fhlsdj_2_2_label->setStyleSheet("QLabel#fhlsdj_2_2_label{background-image:url(:/pic/em_run.png)}");
        }
        else
        {
            ui->fhlsdj_2_2_label->setStyleSheet("QLabel#fhlsdj_2_2_label{background-image:url(:/pic/em_stop.png)}");
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
        ui->wnzp_1_1_label->setStyleSheet("QLabel#wnzp_1_1_label{background-image:url(:/pic/wheel_stop.png)}");
    }
    else
    {
        ui->wnzp_1_1_label->setStyleSheet("QLabel#wnzp_1_1_label{background-image:url(:/pic/wheel_run.png)}");
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
        ui->flzp_2_1_label->setStyleSheet("QLabel#flzp_2_1_label{background-image:url(:/pic/wheel_stop.png)}");
    }
    else
    {
        ui->flzp_2_1_label->setStyleSheet("QLabel#flzp_2_1_label{background-image:url(:/pic/wheel_run.png)}");
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
        ui->fhzp_2_1_label->setStyleSheet("QLabel#fhzp_2_1_label{background-image:url(:/pic/wheel_stop.png)}");
    }
    else
    {
        ui->fhzp_2_1_label->setStyleSheet("QLabel#fhzp_2_1_label{background-image:url(:/pic/wheel_run.png)}");
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
        ui->fllx_2_1_label->setStyleSheet("QLabel#fllx_2_1_label{background-image:url(:/pic/wheel_stop.png)}");
    }
    else
    {
        ui->fllx_2_1_label->setStyleSheet("QLabel#fllx_2_1_label{background-image:url(:/pic/wheel_run.png)}");
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
        ui->wnlx_2_1_label->setStyleSheet("QLabel#wnlx_2_1_label{background-image:url(:/pic/wheel_stop.png)}");
    }
    else
    {
        ui->wnlx_2_1_label->setStyleSheet("QLabel#wnlx_2_1_label{background-image:url(:/pic/wheel_run.png)}");
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
        ui->fhlx_2_1_label->setStyleSheet("QLabel#fhlx_2_1_label{background-image:url(:/pic/wheel_stop.png)}");
    }
    else
    {
        ui->fhlx_2_1_label->setStyleSheet("QLabel#fhlx_2_1_label{background-image:url(:/pic/wheel_run.png)}");
    }

}

void MixerDlg::on_exitButton_clicked()
{
    close();
}
