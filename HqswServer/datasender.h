#ifndef DATASENDER_H
#define DATASENDER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>

#include "global.h"

class DataSender : public QObject
{
    Q_OBJECT
public:
    DataSender();
    DataSender(QTcpSocket *sock);
    DataSender(QHostAddress *serverIp, int port);

    ~DataSender();

    int sendRequestWithResults(const QString strData);
    int sendRequestWithResults(QByteArray data);
    quint32 StreamLen_CRC32(QByteArray &data);

signals:
    void updateClients(QByteArray);
    void updateConnectState(bool);

protected slots:
    void dataReceive();

private:
    QHostAddress *_serverIP;
    QTcpSocket* _tcpSocket;
    bool status;
    int _port;
    int StreamLength = 0;
    QByteArray sData;
    void clear();
};

#endif // DATASENDER_H
