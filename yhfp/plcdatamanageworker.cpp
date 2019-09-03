#include "plcdatamanageworker.h"
#include "global.h"
#include "datasender.h"
#include <QDateTime>

PlcDataManageWorker::PlcDataManageWorker(QObject *parent) : QObject(parent)
{
    QString sharePath = Global::systemConfig.controlSharePath;
    int shareId = Global::systemConfig.controlShareKey;
    QString semPath = Global::systemConfig.controlSemPath;
    int semId = Global::systemConfig.controlSemKey;
    QString msgPath = Global::systemConfig.controlMsgPath;
    int msgId = Global::systemConfig.controlMsgKey;

    key_t shareKey = ShareHelper::GenKey(sharePath.toLatin1(), shareId);
    key_t semKey = ShareHelper::GenKey(semPath.toLatin1(), semId);
    serverDataSh = new ShareHelper(shareKey, semKey);

    qRegisterMetaType<Plc_Db>("Hq_Plc_Db");
}

PlcDataManageWorker::~PlcDataManageWorker()
{
    delete serverDataSh;
}

void PlcDataManageWorker::getSharedDatas()
{
    Plc_Db plcdata;

    serverDataSh->LockShare();
    serverDataSh->GetShardMemory((void *)&plcdata, sizeof(Plc_Db));
    serverDataSh->UnlockShare();

    qDebug() << "plcdata.f_data[0] = " << plcdata.f_data[0];
    qDebug() << "Begin set memory ----";
    plcdata.f_data[0] += 1;
    plcdata.f_data[2] += 0.5;

    /*if(plcdata.i_data[1] == 5)
    {
       plcdata.i_data[1] = -4;
    }
    else
    {
        plcdata.i_data[1] = 5;
    }*/

    plcdata.dw_data[3] += 2;

    plcdata.w_data[1] += 2;

    if(plcdata.b_data[0] == 1)
    {
       plcdata.b_data[0] = 0;
    }
    else
    {
        plcdata.b_data[0] = 1;
    }

    if(plcdata.b_data[2] == 1)
    {
       plcdata.b_data[2] = 0;
    }
    else
    {
        plcdata.b_data[2] = 1;
    }

    if(plcdata.b_data[9] == 1)
    {
       plcdata.b_data[9] = 0;
    }
    else
    {
        plcdata.b_data[9] = 1;
    }


    qDebug() << "plcdata.f_data[0] = " << plcdata.f_data[0];
    serverDataSh->LockShare();
    qDebug() << "sizeof(Plc_Db) = " << sizeof(Plc_Db);
    serverDataSh->SetSharedMemory((void *)&plcdata, sizeof(Plc_Db));
    serverDataSh->UnlockShare();
    qDebug() << "End set memory ----";

    parseYhcServerData(plcdata);
    sendPlcdataToServer(plcdata);
}

void PlcDataManageWorker::parseYhcServerData(Plc_Db dbData)
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

    for(int i=0; i < DB_UINT32_LEN; i++, temp++)
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

    for(int i=0; i < DB_UINT16_LEN; i++, temp++)
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

        float address = Global::getYhcRunctrAddressByIndex(i);

        qDebug() << "i=" << i << "; address = " << address
                 << "; index = " << Global::convertYhcAddressToIndex(address, "x0");

        dataMap.insert(address,strValue);
        if(!Global::currentYhcDataMap.contains(address))
        {
            diff = true;
            Global::currentYhcDataMap.insert(address,strValue);
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentYhcDataMap[address] != strValue)
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentYhcDataMap[address] = strValue;
            }
        }
    }

    int startIndex = Global::getYhcDeviceStartIndex(
                Global::getYhcDeviceGroupInfo(0).deviceId, Global::getYhcDeviceGroupInfo(0).groupId);

    qDebug() << "startIndex = " << startIndex;
    qDebug() << "diff = " << diff;

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

void PlcDataManageWorker::sendPlcdataToServer(const Plc_Db data)
{
    DataSender ds;
    StreamPack bpack;
    QDateTime currentdt = QDateTime::currentDateTime();
    uint stime =currentdt.toTime_t();
    uint etime =currentdt.toTime_t();

    bpack = {sizeof(StreamPack),6,0,w_Update_Plcdata,yhfpsw,0,0,sizeof(Plc_Db),0,stime,etime};
    bpack.bStreamLength += sizeof(Plc_Db) + 4;

    QByteArray allPackData, SData, crcData;
    QDataStream out(&SData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out.setByteOrder(QDataStream::LittleEndian);

    allPackData.append((char*)&bpack, sizeof(bpack));

    int len = allPackData.length();

    for(int i=0; i < DB_FLOAT_LEN; i++)
    {
        out << data.f_data[i];
    }
    for(int i=0; i < DB_INT_LEN; i++)
    {
        out << data.i_data[i];
    }
    for(int i=0; i < DB_UINT32_LEN; i++)
    {
        out << data.dw_data[i];
    }
    for(int i=0; i < DB_UINT16_LEN; i++)
    {
        out << data.w_data[i];
    }
    for(int i=0; i < DB_BOOL_LEN; i++)
    {
        out << data.b_data[i];
    }

    SData.insert(0, allPackData);
    len = SData.length();

    uint scrc = ds.StreamLen_CRC32(SData);

    QDataStream out1(&crcData,QIODevice::WriteOnly);
    out1.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out1.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out1.setByteOrder(QDataStream::LittleEndian);
    out1 << scrc;

    SData.append(crcData);

    len = SData.length();

    ds.sendRequestWithResults(SData);
}
