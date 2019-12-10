#include "syscontroller.h"
#include <QMutexLocker>
#include <QProcess>
#include <QDebug>
#include <QString>
#include <QStringList>

Syscontroller::Syscontroller(DeviceType dataType, int groupId, QObject *parent) : QObject(parent)
{
    _dataType = dataType;
    _groupId = groupId;

    qRegisterMetaType<DeviceType>("msgname");

    QString sharePath = Global::systemConfig.controlSharePath;
    int shareId = Global::systemConfig.controlShareKey;
    key_t shareKey = ShareHelper::GenKey(sharePath.toLatin1(), shareId);
    ctrlShare = new ShareHelper(shareKey, sizeof(Ctr_Block));

    sharePath = Global::systemConfig.dataSharePath;
    shareId = Global::systemConfig.dataShareKey;
    shareKey = ShareHelper::GenKey(sharePath.toLatin1(), shareId);
    dbShare = new ShareHelper(shareKey, sizeof(Plc_Db));

    /*Plc_Db data;
    yhcCtrlShare->GetShardMemory((void*)&data, sizeof(Plc_Db));
    yhcCtrlShare->LockShare();
    data.dw_link[16] = (uint32_t)getpid();
    yhcCtrlShare->SetSharedMemory((void*)&data, sizeof(Plc_Db));
    yhcCtrlShare->UnlockShare();*/

    pdmWorker = new PlcDataManageWorker;
    pdmWorker->moveToThread(&plcdataManageThread);
    connect(&plcdataManageThread, &QThread::finished, pdmWorker, &QObject::deleteLater);
    connect(this, SIGNAL(pollingDatas(DeviceType,int)), pdmWorker, SLOT(getSharedDatas(DeviceType,int)));
    connect(pdmWorker, &PlcDataManageWorker::plcDbUpdated, this, &Syscontroller::handlePlcDbUpdated, Qt::QueuedConnection);
    plcdataManageThread.start();

    updateStatusTimer = new QTimer(this);
    connect( updateStatusTimer, SIGNAL(timeout()), this, SLOT(updateSysStatus()) );
    updateStatusTimer->start(1000);
}

Syscontroller *Syscontroller::getInstance(DeviceType dataType, int groupId)
{
    if (instance == Q_NULLPTR)
    {
        QMutexLocker locker(mutex);
        if (instance == Q_NULLPTR)
        {
            instance = new Syscontroller(dataType, groupId);
        }
    }

    return instance;
}

ControllerInfo Syscontroller::getControllerStatus()
{
    return ctrlInfo;
}

Plc_Db Syscontroller::getPlcDataDb()
{
    Plc_Db data;
    dbShare->GetShardMemory((void*)&data, sizeof(Plc_Db));
    //yhcDbShare->GetShardMemory((void*)&data, sizeof(Plc_Db));
    return data;
}

void Syscontroller::setPlcControlDb(Plc_Db data)
{
    //ctrlShare->LockShare();
    //ctrlShare->SetSharedMemory((void*)&data, sizeof(Plc_Db));
    //ctrlShare->UnlockShare();
}

void Syscontroller::yhcSpeedUp(int deviceIndex, float value)
{
    //qDebug() << "Start Press Speed Up!";

    /*DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Speed");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index = Global::convertAddressToIndex(address, "r");

    //qDebug() << "handle Press Speed Up!";
    Plc_Db db;
    //yhcDbShare->LockShare();
    dbShare->LockShare();
    dbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
    //yhcDbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
    db.f_data[index] = db.f_data[index] + value;
    dbShare->SetSharedMemory((void*)&db, sizeof(Plc_Db));
    dbShare->UnlockShare();
    //yhcDbShare->UnlockShare();

    //applyControlRequest();

    //qDebug() << "Press Speed Up!";
    */

    DeviceGroupInfo info = Global::getMixDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getMixNodeInfoByName("ING_SPIRAL_RATE_SETTING");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index1 = Global::convertAddressToIndex(address, deviceNode.DataType);

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("TOTAL_CURRENT");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index2 = Global::convertAddressToIndex(address, deviceNode.DataType);

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_SPIRAL_PRESSURE");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index3 = Global::convertAddressToIndex(address, deviceNode.DataType);

    info = Global::getMixDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getMixNodeInfoByName("SLUG_BIN_OUTPUT_QUANTITY");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index4 = Global::convertAddressToIndex(address, deviceNode.DataType);

    //qDebug() << "handle mix setting data!";
    Plc_Db db;
    dbShare->LockShare();
    dbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
    db.w_data[index1] = db.w_data[index1] + value;
    db.i_data[index2] = db.i_data[index2] + value;
    db.i_data[index3] = db.i_data[index2] + value;
    db.i_data[index4] = db.i_data[index2] + value + 10;
    dbShare->SetSharedMemory((void*)&db, sizeof(Plc_Db));
    //Set data changed flag.
    Ctr_Block cb;
    ctrlShare->LockShare();
    ctrlShare->GetShardMemory((void*)&cb, sizeof(Ctr_Block));
    cb.toPru[0] = 1;
    ctrlShare->SetSharedMemory((void*)&cb, sizeof(Ctr_Block));
    ctrlShare->UnlockShare();
    //
    dbShare->UnlockShare();
}

void Syscontroller::yhcStart(int deviceIndex, bool value)
{
    // To do: using a test name temporary.
    /*int index = Global::getYhcDataIndexByName("FAN_VALVE_HAND_OPEN", deviceIndex);

    //qDebug() << "Begin yhc start or stop!";
    Plc_Db db;
    //yhcDbShare->LockShare();
    dbShare->LockShare();
    dbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
    //yhcDbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
    if(value)
    {
        db.b_data[index] = 0;
    }
    else
    {
        db.b_data[index] = 1;
    }
    dbShare->SetSharedMemory((void*)&db, sizeof(Plc_Db));
    dbShare->UnlockShare();
    //yhcDbShare->UnlockShare();

    //applyControlRequest();

    //qDebug() << "End yhc start or stop!";
    */

    qDebug() << "Begin mix data change!";
    Plc_Db db;
    dbShare->LockShare();
    dbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
    if(value)
    {
        db.b_data[Global::getMixDataIndexByName("SLUG_SPIRAL_EM_FAULT", deviceIndex)] = 1;
        db.b_data[Global::getMixDataIndexByName("SLUG_WHEEL_EM_FAULT", deviceIndex)] = 1;
        db.b_data[Global::getMixDataIndexByName("CY_1_CORO", deviceIndex)] = 1;
        db.b_data[Global::getMixDataIndexByName("CY_1_INVE", deviceIndex)] = 1;
        db.b_data[Global::getMixDataIndexByName("ING_WHEEL_CORO", deviceIndex)] = 1;
        db.b_data[Global::getMixDataIndexByName("ING_WHEEL_INVE", deviceIndex)] = 1;
        db.b_data[Global::getMixDataIndexByName("SLUG_BIN_HEATER_FAULT", deviceIndex)] = 1;
        db.b_data[Global::getMixDataIndexByName("ING_BIN_HEATER_FAULT", deviceIndex)] = 1;
        db.b_data[Global::getMixDataIndexByName("SLUG_BIN_LEVEL_SIG", deviceIndex)] = 1;
        db.b_data[Global::getMixDataIndexByName("ING_BIN_LEVEL_SIG", deviceIndex)] = 1;
    }
    else
    {
        db.b_data[Global::getMixDataIndexByName("SLUG_SPIRAL_EM_FAULT", deviceIndex)] = 0;
        db.b_data[Global::getMixDataIndexByName("SLUG_WHEEL_EM_FAULT", deviceIndex)] = 0;
        db.b_data[Global::getMixDataIndexByName("CY_1_CORO", deviceIndex)] = 0;
        db.b_data[Global::getMixDataIndexByName("CY_1_INVE", deviceIndex)] = 0;
        db.b_data[Global::getMixDataIndexByName("ING_WHEEL_CORO", deviceIndex)] = 0;
        db.b_data[Global::getMixDataIndexByName("ING_WHEEL_INVE", deviceIndex)] = 0;
        db.b_data[Global::getMixDataIndexByName("SLUG_BIN_HEATER_FAULT", deviceIndex)] = 0;
        db.b_data[Global::getMixDataIndexByName("ING_BIN_HEATER_FAULT", deviceIndex)] = 0;
        db.b_data[Global::getMixDataIndexByName("SLUG_BIN_LEVEL_SIG", deviceIndex)] = 0;
        db.b_data[Global::getMixDataIndexByName("ING_BIN_LEVEL_SIG", deviceIndex)] = 0;
    }
    dbShare->SetSharedMemory((void*)&db, sizeof(Plc_Db));
    //Set data changed flag.
    Ctr_Block cb;
    ctrlShare->LockShare();
    ctrlShare->GetShardMemory((void*)&cb, sizeof(Ctr_Block));
    cb.toPru[0] = 1;
    ctrlShare->SetSharedMemory((void*)&cb, sizeof(Ctr_Block));
    ctrlShare->UnlockShare();
    //
    dbShare->UnlockShare();
}

DeviceType Syscontroller::getDataType()
{
    return _dataType;
}

int Syscontroller::getGroupId()
{
    return _groupId;
}

Syscontroller::~Syscontroller()
{
    plcdataManageThread.requestInterruption();
    plcdataManageThread.quit();
    plcdataManageThread.wait();
}

void Syscontroller::updateSysStatus()
{
    emit pollingDatas(_dataType, _groupId);
}

void Syscontroller::handlePlcDbUpdated(QSet<int> changedDeviceSet, QMap<float, QString> dataMap)
{
    emit plcDbUpdated(changedDeviceSet, dataMap);
}

void Syscontroller::applyControlRequest()
{
    /*Plc_Db data;
    yhcCtrlShare->LockShare();
    yhcDbShare->LockShare();
    yhcCtrlShare->GetShardMemory((void*)&data, sizeof(Plc_Db));
    yhcDbShare->SetSharedMemory((void*)&data, sizeof(Plc_Db));
    yhcDbShare->UnlockShare();
    yhcCtrlShare->UnlockShare();

    Plc_Db data1;
    yhcDbShare->LockShare();
    yhcDbShare->GetShardMemory((void*)&data1, sizeof(Plc_Db));
    yhcDbShare->UnlockShare();*/
}

void Syscontroller::handlePlcControl(StreamPack pack, QSet<int> changedDeviceSet, QMap<float, QString> dataMap)
{
    Plc_Db db;
    Ctr_Block ctrBlock;
    //int pid = 0;
    /*switch (pack.bDeviceId) {
    case YHC:
        //yhcDbShare->LockShare();
        yhcCtrlShare->LockShare();
        yhcCtrlShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
        //yhcDbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
        resetControlShare(pack.bDataType, dataMap, &db);
        yhcCtrlShare->SetSharedMemory((void*)&db, sizeof(Plc_Db));
        yhcCtrlShare->UnlockShare();
        //yhcDbShare->UnlockShare();
        //Remove after test.
        //applyControlRequest();
        break;
    case FER:
        //yhcDbShare->LockShare();
        yhcCtrlShare->LockShare();
        yhcCtrlShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
        //yhcDbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
        resetControlShare(pack.bDataType, dataMap, &db);
        yhcCtrlShare->SetSharedMemory((void*)&db, sizeof(Plc_Db));
        yhcCtrlShare->UnlockShare();
        //yhcDbShare->UnlockShare();
        //Remove after test.
        //applyControlRequest();

        break;
    default:
        break;
    }*/
    dbShare->LockShare();
    dbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
    resetControlShare(pack.bDataType, dataMap, &db);
    dbShare->SetSharedMemory((void*)&db, sizeof(Plc_Db));
    ctrlShare->LockShare();
    ctrlShare->GetShardMemory((void*)&ctrBlock, sizeof(Ctr_Block));
    ctrBlock.toPru[0] = 1;
    ctrlShare->SetSharedMemory((void*)&ctrBlock, sizeof(Ctr_Block));
    ctrlShare->UnlockShare();
    dbShare->UnlockShare();

    //for test
    /*int pid = Global::getPruPid();
    if(pid > 0)
    {
        kill(pid, SIG_TO_REMOTE);
    }*/
}

void Syscontroller::resetControlShare(int dataType, QMap<float, QString> controlData, Plc_Db* controlDb)
{
    switch (dataType) {
    case Bool:
        foreach(float address, controlData.keys())
        {
            int index = Global::convertAddressToIndex(address, "x0");
            if(controlData.value(address) == "1")
            {
               controlDb->b_data[index] = true;
            }
            else
            {
               controlDb->b_data[index] = false;
            }
        }
        break;
    case Float:
        foreach(float address, controlData.keys())
        {
            int index = Global::convertAddressToIndex(address, "r");
            controlDb->f_data[index] = controlData.value(address).toFloat();
        }
        break;
    case Int:
        foreach(float address, controlData.keys())
        {
            int index = Global::convertAddressToIndex(address, "di");
            controlDb->i_data[index] = controlData.value(address).toInt();
        }
        break;
    case UInt:
        foreach(float address, controlData.keys())
        {
            int index = Global::convertAddressToIndex(address, "dw");
            controlDb->dw_data[index] = controlData.value(address).toUInt();
        }
        break;
    case UShort:
        foreach(float address, controlData.keys())
        {
            int index = Global::convertAddressToIndex(address, "w");
            controlDb->w_data[index] = controlData.value(address).toUShort();
        }
        break;
    default:
        break;
    }
}

Syscontroller* Syscontroller::instance = Q_NULLPTR;
QMutex* Syscontroller::mutex = new QMutex();
