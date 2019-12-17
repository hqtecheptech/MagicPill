#include "parseserverdataworker.h"
#include "global.h"
#include "identity.h"

ParseServerDataWorker::ParseServerDataWorker(QObject *parent) : QObject(parent)
{

}

void ParseServerDataWorker::parseFerServerData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QByteArray byteValues = data.mid(sizeof(bDevice), data.length() - sizeof(bDevice) - 4 * bDevice.bDataLength);
    QTextCodec *codec = QTextCodec::codecForLocale();
    QString strValues = codec->toUnicode(byteValues);
    QStringList strValueList = strValues.split(",");
    QVector<QString> strArray = strValueList.toVector();

    byteValues = data.mid(data.length() - 4 * bDevice.bDataLength, 4 * bDevice.bDataLength);

    bool diff = false;
    QMap<float,QString> dataMap;
    QVector<float> addressArray;
    QVector<float> changedAddressArray;
    for(quint16 i=0; i<bDevice.bDataLength; ++i)
    {
        QByteArray value = byteValues.mid(i*4,4);
        float temp = 0;
        memcpy(&temp,value,4);
        addressArray.append(temp);
        dataMap.insert(temp,strArray[i]);
        if(!Global::currentFermenationDataMap.contains(temp))
        {
            diff = true;
            Global::currentFermenationDataMap.insert(temp,strArray[i]);
            changedAddressArray.append(temp);
        }
        else
        {
            if(Global::currentFermenationDataMap[temp] != strArray[i])
            {
                diff = true;
                changedAddressArray.append(temp);
                if(temp < Global::ferDeviceInfo.Runctr_Address)
                {
                    Global::currentFermenationDataMap[temp] = strArray[i];
                }
            }
        }
    }

    // TestCode
    /*if(!ferDataFileSaved)
    {
        QFile dataFile("ferdatafile0.txt");
        if(!dataFile.open(QIODevice::WriteOnly))
        {

        }
        QTextStream out(&dataFile);


        for(int row = 0; row < addressArray.length(); row++)
        {
            out << addressArray[row] << " " << strArray[row] << endl;
        }

        ferDataFileSaved = true;
    }*/
    // TestCode
    if(addressArray.length() > 609)
    {
        qDebug() <<  "addressArray[609]: " << addressArray[609];
        qDebug() <<  "strArray[609]: " << strArray[609];
    }

    int startIndex = Global::getFerDeviceStartIndex(bDevice.bDeviceId, bDevice.bDeviceGroup);

    if(startIndex >=0 && diff)
    {
        QSet<int> changedDeviceSet;
        foreach(float address, changedAddressArray)
        {
            if(address < Global::ferDeviceInfo.Runctr_Address)
            {
                changedDeviceSet.insert(startIndex + Global::getFerDeviceIndexByAddress(address));
            }
            else
            {
                changedDeviceSet.insert(startIndex + Global::getFerDeviceIndexByRunctrAddress(address));
            }
        }

        emit resultReady(changedDeviceSet, dataMap);
    }
}

void ParseServerDataWorker::parseDeoServerData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QByteArray byteValues = data.mid(sizeof(bDevice), data.length() - sizeof(bDevice) - 4 * bDevice.bDataLength);
    QString strValues(byteValues);
    QStringList strValueList = strValues.split(",");
    QVector<QString> strArray = strValueList.toVector();

    byteValues = data.mid(data.length() - 4 * bDevice.bDataLength, 4 * bDevice.bDataLength);

    bool diff = false;
    QMap<float,QString> dataMap;
    QVector<float> addressArray;
    QVector<float> changedAddressArray;
    for(quint16 i=0; i<bDevice.bDataLength; ++i)
    {
        QByteArray value = byteValues.mid(i*4,4);
        float temp = 0;
        memcpy(&temp,value,4);
        addressArray.append(temp);
        dataMap.insert(temp,strArray[i]);

        if(!Global::currentDeodorationDataMap.contains(temp))
        {
            diff = true;
            Global::currentDeodorationDataMap.insert(temp,strArray[i]);
            changedAddressArray.append(temp);
        }
        else
        {
            if(Global::currentDeodorationDataMap[temp] != strArray[i])
            {
                diff = true;
                changedAddressArray.append(temp);
                if(temp < Global::deoDeviceInfo.Runctr_Address)
                {
                    Global::currentDeodorationDataMap[temp] = strArray[i];
                }
            }
        }
    }

    // TestCode
    /*if(!deoDataFileSaved)
    {
        QFile dataFile("deodatafile0.txt");
        if(!dataFile.open(QIODevice::WriteOnly))
        {

        }
        QTextStream out(&dataFile);


        for(int row = 0; row < addressArray.length(); row++)
        {
            out << addressArray[row] << " " << strArray[row] << endl;
        }

        deoDataFileSaved = true;
    }*/
    // TestCode

    int startIndex = Global::getDeoDeviceStartIndex(bDevice.bDeviceId, bDevice.bDeviceGroup);

    if(startIndex >=0 && diff)
    {
        QSet<int> changedDeviceSet;
        foreach(float address, changedAddressArray)
        {
            if(address < Global::deoDeviceInfo.Runctr_Address)
            {
                changedDeviceSet.insert(startIndex + Global::getDeoDeviceIndexByAddress(address));
            }
            else
            {
                changedDeviceSet.insert(startIndex + Global::getDeoDeviceIndexByRunctrAddress(address));
            }
        }

        emit resultReady(changedDeviceSet, dataMap);
    }
}
