#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include <QObject>
#include <QTcpSocket>
#include <QSet>
#include <QMap>

class DataReceiver : public QObject
{
    Q_OBJECT
public:
    explicit DataReceiver(QObject *parent = 0);
    explicit DataReceiver(QTcpSocket* tcpSocket, QObject *parent = 0);

    ~DataReceiver();

signals:

public slots:
    void dataReceive();

signals:
    void dataChanged(QSet<int>, QMap<float,QString> dataMap);

private:
    QTcpSocket* _tcpSocket;
    bool status;
    quint32 StreamLength = 0;
    QByteArray sData;
    void clear();
    quint32 StreamLen_CRC32(QByteArray &data);
    void write(QString replyMsg);
};

#endif // DATARECEIVER_H
