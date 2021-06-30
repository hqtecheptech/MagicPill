#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include "dataformat.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <QObject>
#include <QWidget>
#include <QtNetwork>

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
private:
    bool status;
    int port;
    QHostAddress *serverIP;
    QTcpSocket *tcpSocket;
    quint32 StreamLength = 0;
    QByteArray sData;
    void clear();

public:
    explicit TcpClientSocket(QObject *parent=0);
    void sendTestConnectRequest();
    void sendRequest(StreamPack bpack);
    int sendRequestWithResult(StreamPack requestPack, QVariant dataToSend, int dataLength);
    int sendRequestWithResults(QByteArray data);
    int sendRequestWithResults(const QString strData);
    quint32 StreamLen_CRC32(QByteArray &data);

signals:
    void updateClients(QByteArray);
    void updateConnectState(bool);
    void test(int);

protected slots:
    void dataReceived();
};

#endif // TCPCLIENTSOCKET_H
