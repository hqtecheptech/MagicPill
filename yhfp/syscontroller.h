#ifndef SYSCONTROLLER_H
#define SYSCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QThread>
#include <QSet>
#include <QMap>
#include "netstatemanageworker.h"
#include "plcdatamanageworker.h"
#include "data.h"
#include "global.h"
#include "sharehelper.h"
#include "../dev_app/mhd_lib/inc/Share_Memory.h"

class Syscontroller : public QObject
{
    Q_OBJECT
public:
    static Syscontroller* getInstance();
    ControllerInfo getControllerStatus();
    Plc_Db getPlcDataDb();
    void setPlcControlDb(Plc_Db data);
    void yhcSpeedUp(int deviceIndex, float value);
    ~Syscontroller();

signals:
    void resultReady();
    void pollingDatas();
    void plcDbUpdated(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);

public slots:
    void updateSysStatus();
    void handleYhcPlcDbUpdated(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);
    void applyControlRequest();
    void handlePlcControl(StreamPack pack, QSet<int> changedDeviceSet, QMap<float, QString> dataMap);

private:
    Syscontroller(QObject *parent = 0);
    static Syscontroller* instance;
    static QMutex* mutex;
    QTimer *updateStatusTimer;
    ShareHelper *ctrlShare, *yhcDbShare, *yhcCtrlShare;
    ControllerInfo ctrlInfo;
    Plc_Db plcDataDb, plcControlDb;
    QThread plcdataManageThread;
    PlcDataManageWorker* pdmWorker;

    void resetControlShare(int dataType, QMap<float, QString> controlData, Plc_Db* controlDb);
};

#endif // SYSCONTROLLER_H
