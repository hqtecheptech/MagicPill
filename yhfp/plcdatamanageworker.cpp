#include "plcdatamanageworker.h"
#include "global.h"
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
    qDebug() << "plcdata.f_data[0] = " << plcdata.f_data[0];
    serverDataSh->LockShare();
    qDebug() << "sizeof(Plc_Db) = " << sizeof(Plc_Db);
    serverDataSh->SetSharedMemory((void *)&plcdata, sizeof(Plc_Db));
    serverDataSh->UnlockShare();
    qDebug() << "End set memory ----";

    emit sharedDatasReady(plcdata);

    sendPlcdataToServer(plcdata);
}

void PlcDataManageWorker::sendPlcdataToServer(const Plc_Db data)
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
