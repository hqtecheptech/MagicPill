#ifndef PLCDATAMANAGEWORKER_H
#define PLCDATAMANAGEWORKER_H

#include <QObject>

#include <stdio.h>
#include <signal.h>
#include <unistd.h>//write function
#include <sys/types.h>

#include <data.h>
#include <sharehelper.h>
#include <datasender.h>

class PlcDataManageWorker : public QObject
{
    Q_OBJECT
public:
    explicit PlcDataManageWorker(QObject *parent = 0);

signals:
    void sharedDatasReady(const PlcData datas);

public slots:
    void getSharedDatas();

private:
    key_t serverDataSharedKey = 97;
    ShareHelper* serverDataSh;
    DataSender ds;

    void sendPlcdataToServer(const PlcData data);
};

#endif // PLCDATAMANAGEWORKER_H
