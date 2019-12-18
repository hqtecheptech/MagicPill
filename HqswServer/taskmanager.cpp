#include "taskmanager.h"
#include "data.h"
#include "QDataStream"
#include "datareceiver.h"
#include "datasender.h"

TaskManager::TaskManager(QObject *parent) : QObject(parent)
{

}

TaskManager::TaskManager(int port, QObject *parent) : QObject(parent)
{
    _port = port;
    tcpServer = new QTcpServer();
    hbTimer = new QTimer();
    connect(hbTimer, SIGNAL(timeout()), this, SLOT(sendHeartbeat()));
}

TaskManager::~TaskManager()
{
    tcpServer->close();
    tcpServer->deleteLater();
}

void TaskManager::processNewConnection()
{
    qDebug()<<"processNewConnection";
    QTcpSocket* tcpSocket = tcpServer->nextPendingConnection();
    DataReceiver* dr = new DataReceiver(tcpSocket, this);
}

void TaskManager::handleAcceptError(QAbstractSocket::SocketError error)
{
    qDebug() << "SocketError" << error;
}

void TaskManager::sendHeartbeat()
{
    DataSender *ds = new DataSender();
    connect(ds, SIGNAL(updateConnectState(bool)), this, SLOT(handleConnectionUpdate(bool)));
    QByteArray allPackData, SData, crcData;
    QDataStream out(&SData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out.setByteOrder(QDataStream::LittleEndian);

    StreamPack pack;
    pack = {sizeof(StreamPack),0,0,W_Heart_Beat,0,0,0,0,0,0,0};
    pack.bStreamLength += 4;

    allPackData.append((char*)&pack, sizeof(pack));
    SData.insert(0, allPackData);
    int len = SData.length();

    uint scrc = ds->StreamLen_CRC32(SData);

    QDataStream out1(&crcData,QIODevice::WriteOnly);
    out1.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out1.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out1.setByteOrder(QDataStream::LittleEndian);
    out1 << scrc;

    SData.append(crcData);
    len = SData.length();

    ds->sendRequestWithResults(SData);
}

void TaskManager::handleConnectionUpdate(bool connected)
{
    if(!connected)
    {
        QTcpServer *oldTcpServer = tcpServer;
        oldTcpServer->deleteLater();
        tcpServer = new QTcpServer();
        listeningTask();
    }
}

void TaskManager::listeningTask()
{
    if(tcpServer->listen(QHostAddress::Any, _port))
    {
       qDebug()<<"tcpServer init";
       connect(tcpServer, SIGNAL(newConnection()), this, SLOT(processNewConnection()));
       connect(tcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)),
               this, SLOT(handleAcceptError(QAbstractSocket::SocketError)));
       hbTimer->start(10000);
    }
    else
    {
       qDebug()<<tcpServer->errorString();
    }
}
