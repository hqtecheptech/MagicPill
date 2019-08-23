#ifndef PLCDATAMANAGEWORKER_H
#define PLCDATAMANAGEWORKER_H

#include <QObject>

#include <stdio.h>
#include <signal.h>
#include <unistd.h>//write function
#include <sys/types.h>

#include "data.h"
#include "dataformat.h"
#include "sharehelper.h"
#include "datasender.h"

class PlcDataManageWorker : public QObject
{
    Q_OBJECT
public:
    explicit PlcDataManageWorker(QObject *parent = 0);
    ~PlcDataManageWorker();

signals:
    void sharedDatasReady(const Plc_Db datas);
    void plcDbUpdated(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);

public slots:
    void getSharedDatas();

private:
    ShareHelper* serverDataSh;
    DataSender ds;

    void sendPlcdataToServer(const Plc_Db data);
    void parseYhcServerData(Plc_Db dbData);
};

#endif // PLCDATAMANAGEWORKER_H
