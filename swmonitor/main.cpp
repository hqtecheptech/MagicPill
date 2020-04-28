#include <QCoreApplication>
#include <QDebug>
#include <QString>

#include "data.h"
#include "global.h"
#include "sharehelper.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ShareHelper *ctrShare, *dbShare;

    QString sharePath = Global::systemConfig.controlSharePath;
    int shareId = Global::systemConfig.controlShareKey;
    key_t shareKey = ShareHelper::GenKey(sharePath.toLatin1(), shareId);
    ctrShare = new ShareHelper(shareKey, sizeof(Ctr_Block));

    sharePath = Global::systemConfig.dataSharePath;
    shareId = Global::systemConfig.dataShareKey;
    shareKey = ShareHelper::GenKey(sharePath.toLatin1(), shareId);
    dbShare = new ShareHelper(shareKey, sizeof(Plc_Db));

    Plc_Db db;
    dbShare->GetShardMemory(&db, sizeof(Plc_Db));

    DeviceNode node;
    float address = -1;
    ushort offset = -1;
    int index = -1;
    int deviceNum = 0;
    int runctrAddr = -1;
    int blockSize = 0;

    if(argc == 3)
    {
        int startAddress = -1;
        int startIndex = -1;
        for(int i=0; i<argc; i++)
        {
            qDebug() << "Argv[" << i << "]:" << argv[i];
        }

        switch(QString(argv[1]).toInt())
        {
        case FER:
            node = Global::getFermenationNodeInfoByName(argv[2]);
            deviceNum = Global::ferDeviceInfo.Device_Number;
            runctrAddr = Global::ferDeviceInfo.Runctr_Address;
            blockSize = Global::ferDeviceInfo.RunCtr_Block_Size;
            break;
        case DEO:
            node = Global::getDeodorationNodeInfoByName(argv[2]);
            deviceNum = Global::deoDeviceInfo.Device_Number;
            runctrAddr = Global::deoDeviceInfo.Runctr_Address;
            blockSize = Global::deoDeviceInfo.RunCtr_Block_Size;
            break;
        case MIX:
            node = Global::getMixNodeInfoByName(argv[2]);
            deviceNum = Global::mixDeviceInfo.Device_Number;
            runctrAddr = Global::mixDeviceInfo.Runctr_Address;
            blockSize = Global::mixDeviceInfo.RunCtr_Block_Size;
            break;
        case YHC:
            node = Global::getYhcNodeInfoByName(argv[2]);
            deviceNum = Global::yhcDeviceInfo.Device_Number;
            runctrAddr = Global::yhcDeviceInfo.Runctr_Address;
            blockSize = Global::yhcDeviceInfo.RunCtr_Block_Size;
            break;
        default:
            break;
        }

        if(node.Cname != "NULL")
        {
            if(node.DataType != "x0")
            {
                startAddress = node.Offset;
                //+ QString(argv[3]).toInt() * Global::getLengthByDataType(node.DataType);
            }
            else
            {
                startAddress = runctrAddr;
                //+ Global::ferDeviceInfo.RunCtr_Block_Size / 8 * QString(argv[3]).toInt();
                offset = node.Offset;
                uint step = offset / 8;
                uint temp = offset % 8;
                float index = float(temp) / 10;
                address = index + startAddress + step;
            }
            startIndex = Global::convertAddressToIndex(startAddress, node.DataType);
            qDebug() << "type:" << node.DataType;
            qDebug() << "start address:" << startAddress;
            qDebug() << "index:" << startIndex;
        }
        else
        {
            qDebug() << "name " << argv[2] << " not exist!";
            return -1;
        }

        if(node.DataType == "r")
        {
            for(int i=0; i<deviceNum; i++)
            {
                index = startIndex + i;
                qDebug() << node.Name << ": [" << i << "]" << db.f_data[index];
            }
            return 1;
        }
        else if(node.DataType == "dw")
        {
            for(int i=0; i<deviceNum; i++)
            {
                index = startIndex + i;
                qDebug() << node.Name << ": [" << i << "]" << db.dw_data[index];
            }
            return 1;
        }
        else if(node.DataType == "di")
        {
            for(int i=0; i<deviceNum; i++)
            {
                index = startIndex + i;
                qDebug() << node.Name << ": [" << i << "]" << db.i_data[index];
            }
            return 1;
        }
        else if(node.DataType == "w")
        {
            for(int i=0; i<deviceNum; i++)
            {
                index = startIndex + i;
                qDebug() << node.Name << ": [" << i << "]" << db.w_data[index];
            }
            return 1;
        }
        else if(node.DataType == "x0")
        {
            qDebug() << node.Name;
            for(int i=0; i<deviceNum; i++)
            {
                float temp = address + blockSize / 8 * (float)i;
                index = Global::convertAddressToIndex(temp, node.DataType);
                qDebug() << "Index[" << index << "] Value: " << db.b_data[index] << "; Address:" << temp;
            }
            return 1;
        }
    }
    else if(argc == 4)
    {
        for(int i=0; i<argc; i++)
        {
            qDebug() << "Argv[" << i << "]:" << argv[i];
        }

        switch(QString(argv[1]).toInt())
        {
        case FER:
            node = Global::getFermenationNodeInfoByName(argv[2]);
            deviceNum = Global::ferDeviceInfo.Device_Number;
            runctrAddr = Global::ferDeviceInfo.Runctr_Address;
            blockSize = Global::ferDeviceInfo.RunCtr_Block_Size;
            break;
        case DEO:
            node = Global::getDeodorationNodeInfoByName(argv[2]);
            deviceNum = Global::deoDeviceInfo.Device_Number;
            runctrAddr = Global::deoDeviceInfo.Runctr_Address;
            blockSize = Global::deoDeviceInfo.RunCtr_Block_Size;
            break;
        case MIX:
            node = Global::getMixNodeInfoByName(argv[2]);
            deviceNum = Global::mixDeviceInfo.Device_Number;
            runctrAddr = Global::mixDeviceInfo.Runctr_Address;
            blockSize = Global::mixDeviceInfo.RunCtr_Block_Size;
            break;
        case YHC:
            node = Global::getYhcNodeInfoByName(argv[2]);
            deviceNum = Global::yhcDeviceInfo.Device_Number;
            runctrAddr = Global::yhcDeviceInfo.Runctr_Address;
            blockSize = Global::yhcDeviceInfo.RunCtr_Block_Size;
            break;
        default:
            break;
        }

        if(node.Cname != "NULL")
        {
            if(node.DataType != "x0")
            {
                address = node.Offset + QString(argv[3]).toInt() * Global::getLengthByDataType(node.DataType);
            }
            else
            {
                int startAddrss = runctrAddr + blockSize / 8 * QString(argv[3]).toInt();
                offset = node.Offset;
                uint step = offset / 8;
                uint temp = offset % 8;
                float index = float(temp) / 10;
                address = index + startAddrss + step;
            }
            index = Global::convertAddressToIndex(address, node.DataType);
            qDebug() << "type:" << node.DataType;
            qDebug() << "address:" << address;
            qDebug() << "index:" << index;
        }
        else
        {
            qDebug() << "name " << argv[2] << " not exist!";
            return -1;
        }

        if(node.DataType == "r")
        {
            qDebug() << node.Name << ": " << db.f_data[index];
            return 1;
        }
        else if(node.DataType == "dw")
        {
            qDebug() << node.Name << ": " << db.dw_data[index];
            return 1;
        }
        else if(node.DataType == "di")
        {
            qDebug() << node.Name << ": " << db.i_data[index];
            return 1;
        }
        else if(node.DataType == "w")
        {
            qDebug() << node.Name << ": " << db.w_data[index];
            return 1;
        }
        else if(node.DataType == "x0")
        {
            qDebug() << node.Name << ": " << db.b_data[index];
            return 1;
        }
    }
    else
    {
        qDebug() << "Parameter number incorrect.";
        return -1;
    }

    return a.exec();
}
