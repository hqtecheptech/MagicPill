#include "taskmanager.h"
#include "data.h"
#include "QDataStream"
#include "datareceiver.h"

TaskManager::TaskManager(QObject *parent) : QObject(parent)
{

}

TaskManager::TaskManager(int port, QObject *parent) : QObject(parent)
{
    _port = port;
    tcpServer = new QTcpServer();
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

void TaskManager::listeningTask()
{
    if(tcpServer->listen(QHostAddress::Any, _port))
    {
       qDebug()<<"tcpServer init";
       connect(tcpServer, SIGNAL(newConnection()), this, SLOT(processNewConnection()));
    }
    else
    {
       qDebug()<<tcpServer->errorString();
    }
}
