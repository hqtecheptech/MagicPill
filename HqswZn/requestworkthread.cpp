#include "requestworkthread.h"
#include "tcpclientsocket.h"
#include "centerwidget.h"

#include <QtDebug>

RequestWorkThread::RequestWorkThread(QObject *parent) : QThread(parent)
{

}

RequestWorkThread::~RequestWorkThread()
{

}

void RequestWorkThread::run()
{
    tcpClient = new TcpClientSocket();
    tcpClient->abort();
    disconnect(tcpClient,0,0,0);
    connect(tcpClient, SIGNAL(updateConnectState(bool)),this,SLOT(serverConnected(bool)));
    connect(tcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(requestFinished(QByteArray)));
    if(_len == 0)
    {
        tcpClient->sendRequest(_pack);
    }
    else
    {
        tcpClient->sendRequestWithResult(_pack, _dataToSend, _len);
    }
    this->exec();
}

QVariant RequestWorkThread::getDataToSend() const
{
    return _dataToSend;
}

void RequestWorkThread::setDataToSend(const QVariant &value)
{
    _dataToSend = value;
}

int RequestWorkThread::len() const
{
    return _len;
}

void RequestWorkThread::setLen(int len)
{
    _len = len;
}

uchar *RequestWorkThread::data() const
{
    return _data;
}

void RequestWorkThread::setData(uchar *data)
{
    _data = data;
}

StreamPack RequestWorkThread::pack() const
{
    return _pack;
}

void RequestWorkThread::setPack(const StreamPack &pack)
{
    _pack = pack;
}

void RequestWorkThread::requestFinished(QByteArray result)
{
    tcpClient->close();
    emit updateParentThread(result);
}


void RequestWorkThread::serverConnected(bool isConnected)
{
    emit updateServerConnected(isConnected);
}

void RequestWorkThread::receiveTest(int value)
{
    emit sendTest(value);
}
