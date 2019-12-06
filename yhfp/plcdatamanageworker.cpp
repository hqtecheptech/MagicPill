#include "plcdatamanageworker.h"
#include "global.h"
#include "datasender.h"
#include <QDateTime>

PlcDataManageWorker::PlcDataManageWorker(QObject *parent) : QObject(parent)
{
    QString sharePath = Global::systemConfig.controlSharePath;
    int shareId = Global::systemConfig.controlShareKey;
    key_t shareKey = ShareHelper::GenKey(sharePath.toLatin1(), shareId);
    ctrlSh = new ShareHelper(shareKey, sizeof(Ctr_Block));

    sharePath = Global::systemConfig.dataSharePath;
    shareId = Global::systemConfig.dataShareKey;
    shareKey = ShareHelper::GenKey(sharePath.toLatin1(), shareId);
    dbSh = new ShareHelper(shareKey, sizeof(Plc_Db));

    qRegisterMetaType<Plc_Db>("Plc_Db");
}

PlcDataManageWorker::~PlcDataManageWorker()
{
    delete ctrlSh;
    delete dbSh;
}

void PlcDataManageWorker::getSharedDatas(DeviceType dataName, int groupId)
{
    Plc_Db plcdata;
    Ctr_Block ctrlBlock;

    ctrlSh->LockShare();
    ctrlSh->GetShardMemory((void *)&ctrlBlock, sizeof(Ctr_Block));

    //To do
    if(ctrlBlock.fromPru[1] == 1 && ctrlBlock.fromPru[0] == 1)
    {
        ctrlBlock.fromPru[0] = 0;
        ctrlSh->SetSharedMemory((void *)&ctrlBlock, sizeof(Ctr_Block));

        dbSh->LockShare();
        dbSh->GetShardMemory((void *)&plcdata, sizeof(Plc_Db));
        dbSh->UnlockShare();

        DeviceGroupInfo groupInfo;
        switch(dataName)
        {
            case YHC:
                groupInfo = Global::getYhcDeviceGroupInfoByGroupId(groupId);
                parseYhcServerData(groupInfo, plcdata);

                break;
            case FER:
                groupInfo = Global::getFerDeviceGroupInfoByGroupId(groupId);
                parseFerServerData(groupInfo, plcdata);
                break;
            case MIX:
                groupInfo = Global::getMixDeviceGroupInfoByGroupId(groupId);
                parseMixServerData(groupInfo, plcdata);
                break;
            default:
                break;
        }

        //sendPlcdataToServer(dataName, groupInfo, plcdata);
    }

    ctrlSh->UnlockShare();
}

void PlcDataManageWorker::parseYhcServerData(DeviceGroupInfo groupInfo, const Plc_Db dbData)
{
    Plc_Db newPlcDb = dbData;

    bool diff = false;
    QMap<float,QString> dataMap;
    QVector<float> changedAddressArray;
    float address = 0;

    for(int i=0; i < DB_FLOAT_LEN; i++)
    {
        address = Global::convertIndexToAddress(i, "r");
        dataMap.insert(address,QString::number(newPlcDb.f_data[i]));
        if(!Global::currentYhcDataMap.contains(address))
        {
            diff = true;
            Global::currentYhcDataMap.insert(address,QString::number(newPlcDb.f_data[i]));
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentYhcDataMap[address] != QString::number(newPlcDb.f_data[i]))
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentYhcDataMap[address] = QString::number(newPlcDb.f_data[i]);
            }
        }
    }

    for(int i=0; i < DB_INT_LEN; i++)
    {
        address = Global::convertIndexToAddress(i, "di");
        dataMap.insert(address,QString::number(newPlcDb.i_data[i]));
        if(!Global::currentYhcDataMap.contains(address))
        {
            diff = true;
            Global::currentYhcDataMap.insert(address,QString::number(newPlcDb.i_data[i]));
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentYhcDataMap[address] != QString::number(newPlcDb.i_data[i]))
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentYhcDataMap[address] = QString::number(newPlcDb.i_data[i]);
            }
        }
    }

    for(int i=0; i < DB_UINT32_LEN; i++)
    {
        address = Global::convertIndexToAddress(i, "dw");
        dataMap.insert(address,QString::number(newPlcDb.dw_data[i]));
        if(!Global::currentYhcDataMap.contains(address))
        {
            diff = true;
            Global::currentYhcDataMap.insert(address,QString::number(newPlcDb.dw_data[i]));
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentYhcDataMap[address] != QString::number(newPlcDb.dw_data[i]))
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentYhcDataMap[address] = QString::number(newPlcDb.dw_data[i]);
            }
        }
    }

    for(int i=0; i < DB_UINT16_LEN; i++)
    {
        address = Global::convertIndexToAddress(i, "w");
        dataMap.insert(address,QString::number(newPlcDb.w_data[i]));
        if(!Global::currentYhcDataMap.contains(address))
        {
            diff = true;
            Global::currentYhcDataMap.insert(address,QString::number(newPlcDb.w_data[i]));
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentYhcDataMap[address] != QString::number(newPlcDb.w_data[i]))
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentYhcDataMap[address] = QString::number(newPlcDb.w_data[i]);
            }
        }
    }

    for(int i=0; i < DB_BOOL_LEN; i++)
    {
        QString strValue = "false";
        if(newPlcDb.b_data[i] == 1)
        {
            strValue = "true";
        }

        address = Global::getRunctrAddressByIndex(i);

        dataMap.insert(address,strValue);
        if(!Global::currentYhcDataMap.contains(address))
        {
            diff = true;
            Global::currentYhcDataMap.insert(address,strValue);
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentYhcDataMap.value(address) != strValue)
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentYhcDataMap[address] = strValue;

                uint tankIndex = i / Global::yhcDeviceInfo.RunCtr_Block_Size;
                DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(tankIndex);

                QList<QStandardItem *> newItemList;
                QList<QStandardItem *> newSimpleItemList;
                Global::alertIndex += 1;
                QString simpleAlert;

                newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                newItemList.append(new QStandardItem(QString::number((tankIndex + info.startIndex)+1)));
                if(QVariant(strValue).toBool())
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

                /*if(Identity::getInstance()->getUser() != Q_NULLPTR)
                {
                    newItemList.append(new QStandardItem(Identity::getInstance()->getUser()->getUsername()));
                }
                else
                {
                    newItemList.append(new QStandardItem(""));
                }*/

                int count = Global::simpleAlertsModel->rowCount(QModelIndex()) - ALERT_COUNT;
                if(count > 50)
                {
                    Global::simpleAlertsModel->removeRows(ALERT_COUNT, count, QModelIndex());
                }
                Global::simpleAlertsModel->insertRow(0, newSimpleItemList);
                Global::alertsModel->insertRow(0, newItemList);
            }
        }
    }

    int startIndex = groupInfo.startIndex;

    //qDebug() << "startIndex = " << startIndex;
    //qDebug() << "diff = " << diff;

    if(startIndex >=0 && diff)
    {
        QSet<int> changedDeviceSet;
        foreach(float ca, changedAddressArray)
        {
            if(ca < Global::yhcDeviceInfo.Runctr_Address)
            {
                changedDeviceSet.insert(startIndex + Global::getYhcDeviceIndexByAddress(ca));
            }
            else
            {
                changedDeviceSet.insert(startIndex + Global::getYhcDeviceIndexByRunctrAddress(ca));
            }
        }

        emit plcDbUpdated(changedDeviceSet, dataMap);
    }
}

void PlcDataManageWorker::parseFerServerData(DeviceGroupInfo groupInfo, const Plc_Db dbData)
{
    Plc_Db newPlcDb = dbData;

    bool diff = false;
    QMap<float,QString> dataMap;
    QVector<float> changedAddressArray;
    float address = 0;

    for(int i=0; i < DB_FLOAT_LEN; i++)
    {
        address = Global::convertIndexToAddress(i, "r");
        dataMap.insert(address,QString::number(newPlcDb.f_data[i]));
        if(!Global::currentFermenationDataMap.contains(address))
        {
            diff = true;
            Global::currentFermenationDataMap.insert(address,QString::number(newPlcDb.f_data[i]));
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentFermenationDataMap[address] != QString::number(newPlcDb.f_data[i]))
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentFermenationDataMap[address] = QString::number(newPlcDb.f_data[i]);
            }
        }
    }

    for(int i=0; i < DB_INT_LEN; i++)
    {
        address = Global::convertIndexToAddress(i, "di");
        dataMap.insert(address,QString::number(newPlcDb.i_data[i]));
        if(!Global::currentFermenationDataMap.contains(address))
        {
            diff = true;
            Global::currentFermenationDataMap.insert(address,QString::number(newPlcDb.i_data[i]));
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentFermenationDataMap[address] != QString::number(newPlcDb.i_data[i]))
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentFermenationDataMap[address] = QString::number(newPlcDb.i_data[i]);
            }
        }
    }

    for(int i=0; i < DB_UINT32_LEN; i++)
    {
        address = Global::convertIndexToAddress(i, "dw");
        dataMap.insert(address,QString::number(newPlcDb.dw_data[i]));
        if(!Global::currentFermenationDataMap.contains(address))
        {
            diff = true;
            Global::currentFermenationDataMap.insert(address,QString::number(newPlcDb.dw_data[i]));
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentFermenationDataMap[address] != QString::number(newPlcDb.dw_data[i]))
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentFermenationDataMap[address] = QString::number(newPlcDb.dw_data[i]);
            }
        }
    }

    for(int i=0; i < DB_UINT16_LEN; i++)
    {
        address = Global::convertIndexToAddress(i, "w");
        dataMap.insert(address,QString::number(newPlcDb.w_data[i]));
        if(!Global::currentFermenationDataMap.contains(address))
        {
            diff = true;
            Global::currentFermenationDataMap.insert(address,QString::number(newPlcDb.w_data[i]));
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentFermenationDataMap[address] != QString::number(newPlcDb.w_data[i]))
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentFermenationDataMap[address] = QString::number(newPlcDb.w_data[i]);
            }
        }
    }

    for(int i=0; i < DB_BOOL_LEN; i++)
    {
        QString strValue = "false";
        if(newPlcDb.b_data[i] == 1)
        {
            strValue = "true";
        }

        address = Global::getRunctrAddressByIndex(i);

        dataMap.insert(address,strValue);
        if(!Global::currentFermenationDataMap.contains(address))
        {
            diff = true;
            Global::currentFermenationDataMap.insert(address,strValue);
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentFermenationDataMap.value(address) != strValue)
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentFermenationDataMap[address] = strValue;

                uint tankIndex = i / Global::ferDeviceInfo.RunCtr_Block_Size;
                DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);

                QList<QStandardItem *> newItemList;
                QList<QStandardItem *> newSimpleItemList;
                Global::alertIndex += 1;
                QString simpleAlert;

                newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                newItemList.append(new QStandardItem(QString::number((tankIndex + info.startIndex)+1)));
                if(QVariant(strValue).toBool())
                {
                    newItemList.append(new QStandardItem(Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert1));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            QString::number(tankIndex+1) + "#" +
                            Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert1 + " " +
                            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                }
                else
                {
                    newItemList.append(new QStandardItem(Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert0));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            QString::number(tankIndex+1) + "#" +
                            Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert0 + " " +
                            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

                }
                QStandardItem *simpleAlertItem = new QStandardItem(simpleAlert);
                newSimpleItemList.append(simpleAlertItem);

                /*if(Identity::getInstance()->getUser() != Q_NULLPTR)
                {
                    newItemList.append(new QStandardItem(Identity::getInstance()->getUser()->getUsername()));
                }
                else
                {
                    newItemList.append(new QStandardItem(""));
                }*/

                int count = Global::simpleAlertsModel->rowCount(QModelIndex()) - ALERT_COUNT;
                if(count > 50)
                {
                    Global::simpleAlertsModel->removeRows(ALERT_COUNT, count, QModelIndex());
                }
                Global::simpleAlertsModel->insertRow(0, newSimpleItemList);
                Global::alertsModel->insertRow(0, newItemList);
            }
        }
    }

    int startIndex = groupInfo.startIndex;

    //qDebug() << "startIndex = " << startIndex;
    //qDebug() << "diff = " << diff;

    if(startIndex >=0 && diff)
    {
        QSet<int> changedDeviceSet;
        foreach(float ca, changedAddressArray)
        {
            if(ca < Global::ferDeviceInfo.Runctr_Address)
            {
                changedDeviceSet.insert(startIndex + Global::getFerDeviceIndexByAddress(ca));
            }
            else
            {
                changedDeviceSet.insert(startIndex + Global::getFerDeviceIndexByRunctrAddress(ca));
            }
        }

        emit plcDbUpdated(changedDeviceSet, dataMap);
    }
}

void PlcDataManageWorker::parseMixServerData(DeviceGroupInfo groupInfo, const Plc_Db dbData)
{
    Plc_Db newPlcDb = dbData;

    bool diff = false;
    QMap<float,QString> dataMap;
    QVector<float> changedAddressArray;
    float address = 0;

    for(int i=0; i < DB_FLOAT_LEN; i++)
    {
        address = Global::convertIndexToAddress(i, "r");
        dataMap.insert(address,QString::number(newPlcDb.f_data[i]));
        if(!Global::currentMixDataMap.contains(address))
        {
            diff = true;
            Global::currentMixDataMap.insert(address,QString::number(newPlcDb.f_data[i]));
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentMixDataMap[address] != QString::number(newPlcDb.f_data[i]))
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentMixDataMap[address] = QString::number(newPlcDb.f_data[i]);
            }
        }
    }

    for(int i=0; i < DB_INT_LEN; i++)
    {
        address = Global::convertIndexToAddress(i, "di");
        dataMap.insert(address,QString::number(newPlcDb.i_data[i]));
        if(!Global::currentMixDataMap.contains(address))
        {
            diff = true;
            Global::currentMixDataMap.insert(address,QString::number(newPlcDb.i_data[i]));
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentMixDataMap[address] != QString::number(newPlcDb.i_data[i]))
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentMixDataMap[address] = QString::number(newPlcDb.i_data[i]);
            }
        }
    }

    for(int i=0; i < DB_UINT32_LEN; i++)
    {
        address = Global::convertIndexToAddress(i, "dw");
        dataMap.insert(address,QString::number(newPlcDb.dw_data[i]));
        if(!Global::currentMixDataMap.contains(address))
        {
            diff = true;
            Global::currentMixDataMap.insert(address,QString::number(newPlcDb.dw_data[i]));
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentMixDataMap[address] != QString::number(newPlcDb.dw_data[i]))
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentMixDataMap[address] = QString::number(newPlcDb.dw_data[i]);
            }
        }
    }

    for(int i=0; i < DB_UINT16_LEN; i++)
    {
        address = Global::convertIndexToAddress(i, "w");
        dataMap.insert(address,QString::number(newPlcDb.w_data[i]));
        if(!Global::currentMixDataMap.contains(address))
        {
            diff = true;
            Global::currentMixDataMap.insert(address,QString::number(newPlcDb.w_data[i]));
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentMixDataMap[address] != QString::number(newPlcDb.w_data[i]))
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentMixDataMap[address] = QString::number(newPlcDb.w_data[i]);
            }
        }
    }

    for(int i=0; i < DB_BOOL_LEN; i++)
    {
        QString strValue = "false";
        if(newPlcDb.b_data[i] == 1)
        {
            strValue = "true";
        }

        address = Global::getRunctrAddressByIndex(i);

        dataMap.insert(address,strValue);
        if(!Global::currentMixDataMap.contains(address))
        {
            diff = true;
            Global::currentMixDataMap.insert(address,strValue);
            changedAddressArray.append(address);
        }
        else
        {
            if(Global::currentMixDataMap.value(address) != strValue)
            {
                diff = true;
                changedAddressArray.append(address);
                Global::currentMixDataMap[address] = strValue;

                uint tankIndex = i / Global::mixDeviceInfo.RunCtr_Block_Size;
                DeviceGroupInfo info = Global::getMixDeviceGroupInfo(tankIndex);

                QList<QStandardItem *> newItemList;
                QList<QStandardItem *> newSimpleItemList;
                Global::alertIndex += 1;
                QString simpleAlert;

                newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                newItemList.append(new QStandardItem(QString::number((tankIndex + info.startIndex)+1)));
                if(QVariant(strValue).toBool())
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

                /*if(Identity::getInstance()->getUser() != Q_NULLPTR)
                {
                    newItemList.append(new QStandardItem(Identity::getInstance()->getUser()->getUsername()));
                }
                else
                {
                    newItemList.append(new QStandardItem(""));
                }*/

                int count = Global::simpleAlertsModel->rowCount(QModelIndex()) - ALERT_COUNT;
                if(count > 50)
                {
                    Global::simpleAlertsModel->removeRows(ALERT_COUNT, count, QModelIndex());
                }
                Global::simpleAlertsModel->insertRow(0, newSimpleItemList);
                Global::alertsModel->insertRow(0, newItemList);
            }
        }
    }

    int startIndex = groupInfo.startIndex;

    //qDebug() << "startIndex = " << startIndex;
    //qDebug() << "diff = " << diff;

    if(startIndex >=0 && diff)
    {
        QSet<int> changedDeviceSet;
        foreach(float ca, changedAddressArray)
        {
            if(ca < Global::mixDeviceInfo.Runctr_Address)
            {
                changedDeviceSet.insert(startIndex + Global::getMixDeviceIndexByAddress(ca));
            }
            else
            {
                changedDeviceSet.insert(startIndex + Global::getMixDeviceIndexByRunctrAddress(ca));
            }
        }

        emit plcDbUpdated(changedDeviceSet, dataMap);
    }
}

void PlcDataManageWorker::sendPlcdataToServer(DeviceType dataName, DeviceGroupInfo groupInfo, const Plc_Db data)
{
    DataSender ds;
    StreamPack bpack;
    QDateTime currentdt = QDateTime::currentDateTime();
    uint stime =currentdt.toTime_t();
    uint etime =currentdt.toTime_t();

    // groupInfo.deviceId, can be ignore. Because there may be more than one deviceId in a single device config db.
    // but there must be groupInfo.groupId for distinguishing same type container devices.
    bpack = {sizeof(StreamPack),(quint16)groupInfo.deviceId,(quint16)groupInfo.groupId,W_Update_PlcData,dataName,0,0,sizeof(Plc_Db),0,stime,etime};
    // minus size of controller commdata in Plc_Db (64), plus size of crc data.
    bpack.bStreamLength += sizeof(Plc_Db) - DB_COMM_DATA*4 + 4;

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
