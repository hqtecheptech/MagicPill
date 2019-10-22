#ifndef DATARECEIVER_H
#define DATARECEIVER_H

#include <QObject>
#include <QTcpSocket>
#include <QSet>
#include <QMap>

#include "data.h"
#include "dataformat.h"
#include "syscontroller.h"

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
    void dataChanged(StreamPack, QSet<int>, QMap<float,QString>);

private:
    QTcpSocket* _tcpSocket;
    bool status;
    quint32 StreamLength = 0;
    QByteArray sData;
    Syscontroller* controller;

    void clear();
    quint32 StreamLen_CRC32(QByteArray &data);
    void write(QString replyMsg);
    void sendReply(StreamPack pack, QString result);
};

#endif // DATARECEIVER_H
