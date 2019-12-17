#ifndef REQUESTWORKTHREAD_H
#define REQUESTWORKTHREAD_H

#include <QThread>
#include "tcpclientsocket.h"
#include "dataformat.h"

class RequestWorkThread : public QThread
{
    Q_OBJECT
public:
    RequestWorkThread(QObject *parent = 0);
    ~RequestWorkThread();

    StreamPack pack() const;
    void setPack(const StreamPack &pack);

    uchar *data() const;
    void setData(uchar *data);

    int len() const;
    void setLen(int len);

    QVariant getDataToSend() const;
    void setDataToSend(const QVariant &value);

protected:
    void run() override;

private:
    StreamPack _pack;
    uchar* _data = 0;
    int _len = 0;
    QVariant _dataToSend;
    TcpClientSocket *tcpClient;

signals:
    void updateParentThread(QByteArray result);
    void updateServerConnected(bool);
    void sendTest(int);

private slots:
    void requestFinished(QByteArray result);
    void serverConnected(bool);
    void receiveTest(int);

};

#endif // REQUESTWORKTHREAD_H
