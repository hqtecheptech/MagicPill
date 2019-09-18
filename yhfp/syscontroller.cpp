#include "syscontroller.h"
#include <QMutexLocker>

//接收信号更新共享内存数据到本地Dev_data
static void sig_handler_rpuData(int sig)
{
    if(sig == SIGFRPU)
    {
        mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey,&Dev_data);  //读共享内存
    }
}

Syscontroller::Syscontroller(QObject *parent) : QObject(parent)
{
    QString sharePath = Global::systemConfig.controlSharePath;
    int shareId = Global::systemConfig.controlShareKey;
    QString semPath = Global::systemConfig.controlSemPath;
    int semId = Global::systemConfig.controlSemKey;
    QString msgPath = Global::systemConfig.controlMsgPath;
    int msgId = Global::systemConfig.controlMsgKey;

    key_t shareKey = ShareHelper::GenKey(sharePath.toLatin1(), shareId);
    key_t semKey = ShareHelper::GenKey(semPath.toLatin1(), semId);
    key_t msgKey = ShareHelper::GenKey(msgPath.toLatin1(), msgId);
    ctrlShare = new ShareHelper(shareKey, semKey);

    Run_data.Shmkey = shareKey;  //共享内存键值
    Run_data.Semkey = semKey;  //信号键值
    Run_data.Msgkey = msgKey;  //消息队列键值
    Run_data.Pid[0] = getpid();

    sharePath = Global::systemConfig.dataSharePath;
    shareId = Global::systemConfig.dataShareKey;
    semPath = Global::systemConfig.dataSemPath;
    semId = Global::systemConfig.dataSemKey;
    msgPath = Global::systemConfig.dataMsgPath;
    msgId = Global::systemConfig.dataMsgKey;

    shareKey = ShareHelper::GenKey(sharePath.toLatin1(), shareId);
    semKey = ShareHelper::GenKey(semPath.toLatin1(), semId);
    msgKey = ShareHelper::GenKey(msgPath.toLatin1(), msgId);
    yhcDbShare = new ShareHelper(shareKey, semKey);

    sharePath = Global::systemConfig.yhcControlSharePath;
    shareId = Global::systemConfig.yhcControlShareKey;
    semPath = Global::systemConfig.yhcControlSemPath;
    semId = Global::systemConfig.yhcControlSemKey;
    msgPath = Global::systemConfig.yhcControlMsgPath;
    msgId = Global::systemConfig.yhcControlMsgKey;

    shareKey = ShareHelper::GenKey(sharePath.toLatin1(), shareId);
    semKey = ShareHelper::GenKey(semPath.toLatin1(), semId);
    msgKey = ShareHelper::GenKey(msgPath.toLatin1(), msgId);
    yhcCtrlShare = new ShareHelper(shareKey, semKey);

    //注册PRU Update 信号函数
    signal(SIGFRPU,sig_handler_rpuData);

    pdmWorker = new PlcDataManageWorker;
    pdmWorker->moveToThread(&plcdataManageThread);
    connect(&plcdataManageThread, &QThread::finished, pdmWorker, &QObject::deleteLater);
    connect(this, SIGNAL(pollingDatas()), pdmWorker, SLOT(getSharedDatas()));
    connect(pdmWorker, &PlcDataManageWorker::plcDbUpdated, this, &Syscontroller::handleYhcPlcDbUpdated, Qt::QueuedConnection);
    plcdataManageThread.start();

    updateStatusTimer = new QTimer(this);
    connect( updateStatusTimer, SIGNAL(timeout()), this, SLOT(updateSysStatus()) );
    updateStatusTimer->start(2000);
}

Syscontroller *Syscontroller::getInstance()
{
    if (instance == Q_NULLPTR)
    {
        QMutexLocker locker(mutex);
        if (instance == Q_NULLPTR)
        {
            instance = new Syscontroller();
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
    yhcDbShare->GetShardMemory((void*)&data, sizeof(Plc_Db));
    return data;
}

void Syscontroller::setPlcControlDb(Plc_Db data)
{
    ctrlShare->LockShare();
    ctrlShare->SetSharedMemory((void*)&data, sizeof(Plc_Db));
    ctrlShare->UnlockShare();
}

void Syscontroller::yhcSpeedUp(int deviceIndex, float value)
{
    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Speed");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index = Global::convertYhcAddressToIndex(address, "r");

    Plc_Db db;
    yhcDbShare->LockShare();
    yhcCtrlShare->LockShare();
    yhcDbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
    db.f_data[index] = db.f_data[index] + value;
    yhcCtrlShare->SetSharedMemory((void*)&db, sizeof(Plc_Db));
    yhcCtrlShare->UnlockShare();
    yhcDbShare->UnlockShare();

    applyControlRequest();
}

Syscontroller::~Syscontroller()
{
    plcdataManageThread.requestInterruption();
    plcdataManageThread.quit();
    plcdataManageThread.wait();
}

void Syscontroller::updateSysStatus()
{
    //Start test
    Plc_Db db;
    yhcDbShare->LockShare();
    yhcDbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));

    qsrand(time(NULL));
    int rs = qrand() % 20 + 5;
    int prs = qrand() % 20 + 5;
    //ui->widget_2->updateUI(rs, prs);

    int leftValue = qrand() % 400 + 100;
    int rightValue = qrand() % 400 + 100;
    //ui->yhcWatchsWidget->updateDydl(leftValue, rightValue);

    leftValue = qrand() % 120 - 30;
    rightValue = qrand() % 1000 + 300;
    //ui->yhcWatchsWidget->updateWdyw(leftValue, rightValue);

    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(0);
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("RevolvingSpeed");
    float address = deviceNode.Offset + (info.offset + 0 - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index = Global::convertYhcAddressToIndex(address, deviceNode.DataType);
    db.f_data[index] = (float)rs;

    deviceNode = Global::getYhcNodeInfoByName("Ampere1");
    address = deviceNode.Offset + (info.offset + 0 - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertYhcAddressToIndex(address, deviceNode.DataType);
    db.f_data[index] = (float)prs;

    yhcDbShare->SetSharedMemory((void*)&db, sizeof(Plc_Db));
    yhcDbShare->UnlockShare();

    emit pollingDatas();

    //End test


    int pid;
    int cmd;
    int msgStatus;

    ctrlShare->GetShardMemory((void *)&Dev_data, sizeof(Dev_data));
    if(!(Dev_data.Pru.RPid[1] !=0 && Dev_data.Pru.RPidLink[1] ==1))
    {
        ctrlInfo.isPruConnected = false;
    }
    else
    {
        ctrlInfo.isPruConnected = true;

        msgStatus = mHD_Read_Msg_Cmd(yhfpsw-6, &pid, &cmd);
        if(msgStatus == -1)
        {
            return;
        }

        ctrlInfo.pid = pid;
        ctrlInfo.cmd = cmd;
        ctrlInfo.msgStatus = msgStatus;
        Run_data.Pid[1]  = pid;

        //mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey,Run_data.Semkey,&Dev_data);

        //    if(cmd ==  Msg_SoftInitComplete)    //PRU发来的消息
        //    if(cmd == Msg_Updata_Data)
        //     if(cmd == Msg_WriteParaComplete)
        //     if(cmd == Msg_ReadParaComplete)
        //     if(cmd == Msg_ScanStartStatus)
        //     if(cmd == Msg_ScanStopStatus)

        if(cmd == Msg_Updata_Data)
        {
            emit resultReady();
        }

        /*msgStatus = mHD_Read_Msg_Cmd(yhfpsw, &pid, &cmd);
        if(msgStatus == -1)
        {
            return;
        }

        if(cmd == Msg_Updata_Data)
        {
            if(dbShare->GetShardMemory((void*)&plcDataDb, sizeof(Plc_Db)))
            {
                emit pollingDatas();
            }
        }*/
    }
}

void Syscontroller::handleYhcPlcDbUpdated(QSet<int> changedDeviceSet, QMap<float, QString> dataMap)
{
    emit plcDbUpdated(changedDeviceSet, dataMap);
}

void Syscontroller::applyControlRequest()
{
    Plc_Db data;
    yhcCtrlShare->LockShare();
    yhcDbShare->LockShare();
    yhcCtrlShare->GetShardMemory((void*)&data, sizeof(Plc_Db));
    yhcDbShare->SetSharedMemory((void*)&data, sizeof(Plc_Db));
    yhcDbShare->UnlockShare();
    yhcCtrlShare->UnlockShare();
}

void Syscontroller::handlePlcControl(StreamPack pack, QSet<int> changedDeviceSet, QMap<float, QString> dataMap)
{
    Plc_Db db;
    switch (pack.bDeviceId) {
    case YHC:
        yhcDbShare->LockShare();
        yhcCtrlShare->LockShare();
        yhcDbShare->GetShardMemory((void*)&db, sizeof(Plc_Db));
        resetControlShare(pack.bDataType, dataMap, &db);
        yhcCtrlShare->SetSharedMemory((void*)&db, sizeof(Plc_Db));
        yhcCtrlShare->UnlockShare();
        yhcDbShare->UnlockShare();
        //Remove after test.
        applyControlRequest();
        break;
    case FER:
        break;
    default:
        break;
    }
}

void Syscontroller::resetControlShare(int dataType, QMap<float, QString> controlData, Plc_Db* controlDb)
{
    switch (dataType) {
    case Bool:
        foreach(float address, controlData.keys())
        {
            int index = Global::convertYhcAddressToIndex(address, "x0");
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
            int index = Global::convertYhcAddressToIndex(address, "r");
            controlDb->f_data[index] = controlData.value(address).toFloat();
        }
        break;
    default:
        break;
    }
}

Syscontroller* Syscontroller::instance = Q_NULLPTR;
QMutex* Syscontroller::mutex = new QMutex();
