#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTcpServer>
#include <QAbstractSocket>
#include <QTimer>

class TaskManager : public QObject
{
    Q_OBJECT
public:
    explicit TaskManager(QObject *parent = 0);
    explicit TaskManager(int port, QObject *parent = 0);

    ~TaskManager();

signals:


private slots:
    //处理客户端连接
    void processNewConnection();
    void handleAcceptError(QAbstractSocket::SocketError error);
    void sendHeartbeat();
    void handleConnectionUpdate(bool connected);

public slots:
    void listeningTask();

private:
    QTcpServer* tcpServer;
    bool status;
    int _port;
    quint32 StreamLength = 0;
    QByteArray sData;
    QTimer* hbTimer;
};

#endif // TASKMANAGER_H
