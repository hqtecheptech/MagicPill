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
    dbShare = new ShareHelper(shareKey, semKey);

    //注册PRU Update 信号函数
    signal(SIGFRPU,sig_handler_rpuData);

    pdmWorker = new PlcDataManageWorker;
    pdmWorker->moveToThread(&plcdataManageThread);
    connect(&plcdataManageThread, &QThread::finished, pdmWorker, &QObject::deleteLater);
    connect(this, SIGNAL(pollingDatas()), pdmWorker, SLOT(getSharedDatas()));
    connect(pdmWorker, &PlcDataManageWorker::plcDbUpdated, this, &Syscontroller::handlePlcDbUpdated, Qt::QueuedConnection);
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

Plc_Db Syscontroller::getPlcDb()
{
    return plcDb;
}

Syscontroller::~Syscontroller()
{
    plcdataManageThread.requestInterruption();
    plcdataManageThread.quit();
    plcdataManageThread.wait();
}

void Syscontroller::updateSysStatus()
{
    emit pollingDatas();

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

        msgStatus = mHD_Read_Msg_Cmd(yhfpsw, &pid, &cmd);
        if(msgStatus == -1)
        {
            return;
        }

        if(cmd == Msg_Updata_Data)
        {
            if(dbShare->GetShardMemory((void*)&plcDb, sizeof(Plc_Db)))
            {
                emit pollingDatas();
            }
        }
    }
}

void Syscontroller::handlePlcDbUpdated(QSet<int> changedDeviceSet, QMap<float, QString> dataMap)
{
    emit plcDbUpdated(changedDeviceSet, dataMap);
}

Syscontroller* Syscontroller::instance = Q_NULLPTR;
QMutex* Syscontroller::mutex = new QMutex();
