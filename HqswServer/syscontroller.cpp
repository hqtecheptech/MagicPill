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

void Syscontroller::changeDataValue(int deviceIndex, float value)
{
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_MT_R");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index1 = Global::convertAddressToIndex(address, deviceNode.DataType);

    info = Global::getFerDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getFermenationNodeInfoByName("FER_WT_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index2 = Global::convertAddressToIndex(address, deviceNode.DataType);

    info = Global::getFerDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getFermenationNodeInfoByName("TOTAL_CURRENT_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index3 = Global::convertAddressToIndex(address, deviceNode.DataType);

    info = Global::getFerDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getFermenationNodeInfoByName("FER_TOT_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index4 = Global::convertAddressToIndex(address, deviceNode.DataType);

    Ctr_Block cb;
    ctrlShare->LockShare();
    qDebug() << "changeDataValue ctrlShare locked!";

    Plc_Db db;
    dbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
    db.f_data[index1] = db.f_data[index1] + value;
    db.f_data[index2] = db.f_data[index2] + value;
    db.f_data[index3] = db.f_data[index3] + value;
    db.dw_data[index4] = db.dw_data[index4] + 100;
    dbShare->SetSharedMemory((void*)&db, sizeof(Plc_Db));

    cb.toPru[0] = 1;
    ctrlShare->SetSharedMemory((void*)&cb, sizeof(Ctr_Block));
    ctrlShare->UnlockShare();
    qDebug() << "changeDataValue ctrlShare unlocked!";
}

void Syscontroller::changeRunctrlValue(int deviceIndex, bool value)
{
    Ctr_Block cb;
    ctrlShare->LockShare();
    qDebug() << "changeRunctrlValue ctrlShare locked!";

    Plc_Db db;
    dbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
    if(value)
    {
        db.b_data[Global::getFerDataIndexByName("FAN_FAULT_BOOL", deviceIndex)] = 1;
    }
    else
    {
        db.b_data[Global::getFerDataIndexByName("FAN_FAULT_BOOL", deviceIndex)] = 0;
    }
    dbShare->SetSharedMemory((void*)&db, sizeof(Plc_Db));

    //Set data changed flag.
    cb.toPru[0] = 1;
    ctrlShare->SetSharedMemory((void*)&cb, sizeof(Ctr_Block));
    ctrlShare->UnlockShare();
    qDebug() << "changeRunctrlValue ctrlShare unlocked!";
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
    qDebug() << "Data Changed!";
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
    Ctr_Block ctrBlock;
    ctrlShare->LockShare();
    qDebug() << "handlePlcControl ctrlShare locked!";
    ctrlShare->GetShardMemory((void*)&ctrBlock, sizeof(Ctr_Block));

    foreach(float key, dataMap.keys())
    {
        qDebug() << "handlePlcControl dataMap key: " << key << "; value: " << dataMap[key];
    }

    //if(ctrBlock.fromPru[0] == 1)
    //{
        Plc_Db db;
        dbShare->LockShare();
        dbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
        resetDataShare(pack.bDataType, dataMap, &db);
        dbShare->SetSharedMemory((void*)&db, sizeof(Plc_Db));
        dbShare->UnlockShare();

        ctrBlock.toPru[0] = 1;
        ctrlShare->SetSharedMemory((void*)&ctrBlock, sizeof(Ctr_Block));
    //}
    ctrlShare->UnlockShare();
    qDebug() << "handlePlcControl ctrlShare unlocked!";

    //for test
    /*int pid = Global::getPruPid();
    if(pid > 0)
    {
        kill(pid, SIG_TO_REMOTE);
    }*/
}

void Syscontroller::resetDataShare(int dataType, QMap<float, QString> controlData, Plc_Db* controlDb)
{
    switch (dataType) {
    case Bool:
        foreach(float address, controlData.keys())
        {
            int index = Global::convertAddressToIndex(address, "x0");
            qDebug() << "controlData.value(" << address << "): " << controlData[address];
            if(controlData[address] == "true")
            {
               controlDb->b_data[index] = 1;
            }
            else
            {
               controlDb->b_data[index] = 0;
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
