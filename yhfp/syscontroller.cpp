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

void Syscontroller::parseFerServerData(Plc_Db dbData)
{
    Plc_Db newPlcDb = dbData;

    bool diff = false;
    QMap<float,QString> dataMap;
    QVector<float> changedAddressArray;

    int temp = 0;
    for(int i=0; i < DB_FLOAT_LEN; i++, temp++)
    {
        dataMap.insert(i,QString::number(newPlcDb.f_data[i]));
        if(!Global::currentYhcDataMap.contains(i))
        {
            diff = true;
            Global::currentYhcDataMap.insert(temp,QString::number(newPlcDb.f_data[i]));
            changedAddressArray.append(temp);
        }
        else
        {
            if(Global::currentYhcDataMap[temp] != QString::number(newPlcDb.f_data[i]))
            {
                diff = true;
                changedAddressArray.append(temp);
                Global::currentYhcDataMap[temp] = QString::number(newPlcDb.f_data[i]);
            }
        }
    }

    for(int i=0; i < DB_INT_LEN; i++, temp++)
    {
        dataMap.insert(i,QString::number(newPlcDb.i_data[i]));
        if(!Global::currentYhcDataMap.contains(i))
        {
            diff = true;
            Global::currentYhcDataMap.insert(temp,QString::number(newPlcDb.i_data[i]));
            changedAddressArray.append(temp);
        }
        else
        {
            if(Global::currentYhcDataMap[temp] != QString::number(newPlcDb.i_data[i]))
            {
                diff = true;
                changedAddressArray.append(temp);
                Global::currentYhcDataMap[temp] = QString::number(newPlcDb.i_data[i]);
            }
        }
    }

    for(int i=0; i < DB_UINT16_LEN; i++, temp++)
    {
        dataMap.insert(i,QString::number(newPlcDb.dw_data[i]));
        if(!Global::currentYhcDataMap.contains(i))
        {
            diff = true;
            Global::currentYhcDataMap.insert(temp,QString::number(newPlcDb.dw_data[i]));
            changedAddressArray.append(temp);
        }
        else
        {
            if(Global::currentYhcDataMap[temp] != QString::number(newPlcDb.dw_data[i]))
            {
                diff = true;
                changedAddressArray.append(temp);
                Global::currentYhcDataMap[temp] = QString::number(newPlcDb.dw_data[i]);
            }
        }
    }

    for(int i=0; i < DB_UINT8_LEN; i++, temp++)
    {
        dataMap.insert(i,QString::number(newPlcDb.w_data[i]));
        if(!Global::currentYhcDataMap.contains(i))
        {
            diff = true;
            Global::currentYhcDataMap.insert(temp,QString::number(newPlcDb.w_data[i]));
            changedAddressArray.append(temp);
        }
        else
        {
            if(Global::currentYhcDataMap[temp] != QString::number(newPlcDb.w_data[i]))
            {
                diff = true;
                changedAddressArray.append(temp);
                Global::currentYhcDataMap[temp] = QString::number(newPlcDb.w_data[i]);
            }
        }
    }

    for(int i=0; i < DB_BOOL_LEN; i++, temp++)
    {
        QString strValue = "false";
        if(newPlcDb.b_data[i] == 1)
        {
            strValue = "true";
        }

        dataMap.insert(i,strValue);
        if(!Global::currentYhcDataMap.contains(i))
        {
            diff = true;
            Global::currentYhcDataMap.insert(temp,strValue);
            changedAddressArray.append(temp);
        }
        else
        {
            if(Global::currentYhcDataMap[temp] != strValue)
            {
                diff = true;
                changedAddressArray.append(temp);
                Global::currentYhcDataMap[temp] = strValue;
            }
        }
    }

    int startIndex = Global::getYhcDeviceStartIndex(
                Global::getYhcDeviceGroupInfo(0).deviceId, Global::getYhcDeviceGroupInfo(0).groupId);
    if(startIndex >=0 && diff)
    {
        QSet<int> changedDeviceSet;
        foreach(float address, changedAddressArray)
        {
            if(address < Global::yhcDeviceInfo.Runctr_Address)
            {
                changedDeviceSet.insert(startIndex + Global::getYhcDeviceIndexByAddress(address));
            }
            else
            {
                changedDeviceSet.insert(startIndex + Global::getYhcDeviceIndexByRunctrAddress(address));
            }
        }

        emit plcDbUpdated(changedDeviceSet, dataMap);
    }
}

void Syscontroller::updateSysStatus()
{
    QMap<float,QString> dataMap;
    QSet<int> ch;
    ch.insert(0);
    emit plcDbUpdated(ch, dataMap);

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
                parseFerServerData(plcDb);
            }
        }
    }
}

Syscontroller* Syscontroller::instance = Q_NULLPTR;
QMutex* Syscontroller::mutex = new QMutex();
