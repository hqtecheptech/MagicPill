#ifndef DATASENDER_H
#define DATASENDER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QByteArray>

class DataSender : QObject
{
    Q_OBJECT
public:
    DataSender();
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
    QHostAddress *serverIP;
    QTcpSocket* _tcpSocket;
    bool status;
    int port;
    int StreamLength = 0;
    QByteArray sData;
    void clear();
};

#endif // DATASENDER_H
