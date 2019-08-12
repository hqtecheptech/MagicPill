#include "plcdatamanageworker.h"
#include "QDateTime"

PlcDataManageWorker::PlcDataManageWorker(QObject *parent) : QObject(parent)
{
    serverDataSh = new ShareHelper(serverDataSharedKey);
}

void PlcDataManageWorker::getSharedDatas()
{
    PlcData plcdata;
    serverDataSh->LockShare();
    plcdata = serverDataSh->GetShardMemory();
    serverDataSh->UnlockShare();

    emit sharedDatasReady(plcdata);

    sendPlcdataToServer(plcdata);
}

void PlcDataManageWorker::sendPlcdataToServer(const PlcData data)
{
    StreamPack bpack;
    QDateTime currentdt = QDateTime::currentDateTime();
    uint stime =currentdt.toTime_t();
    uint etime =currentdt.toTime_t();

    quint16 strlen= sizeof(bpack);
    char *p = new char[strlen];
    QByteArray SData;
    memcpy(p,reinterpret_cast<char*>(&bpack),sizeof(bpack));
    QDataStream out(&SData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out.setByteOrder(QDataStream::LittleEndian);
    for(int i=0;i<strlen;i++) {out << *(p+i);}
    delete p;
}
