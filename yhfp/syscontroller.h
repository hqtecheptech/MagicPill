#ifndef SYSCONTROLLER_H
#define SYSCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QThread>
#include <QSet>
#include <QMap>
#include "plcdatamanageworker.h"
#include "data.h"
#include "global.h"
#include "sharehelper.h"
#include "../dev_app/mhd_lib/inc/Share_Memory.h"

class Syscontroller : public QObject
{
    Q_OBJECT
public:
    static Syscontroller* getInstance(msgname dataType, int groupId);
    ControllerInfo getControllerStatus();
    Plc_Db getPlcDataDb();
    void setPlcControlDb(Plc_Db data);
    void yhcSpeedUp(int deviceIndex, float value);
    void yhcStart(int deviceIndex, bool value);
    msgname getDataType();
    int getGroupId();

    ~Syscontroller();

signals:
    void resultReady();
    void pollingDatas(msgname dataName, int groupId);
    void plcDbUpdated(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);

public slots:
    void updateSysStatus();
    void handlePlcDbUpdated(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);
    void applyControlRequest();
    void handlePlcControl(StreamPack pack, QSet<int> changedDeviceSet, QMap<float, QString> dataMap);

private:
    Syscontroller(msgname dataType, int groupId, QObject *parent = 0);
    static Syscontroller* instance;
    static QMutex* mutex;
    QTimer *updateStatusTimer;
    ShareHelper *ctrlShare, *yhcDbShare, *yhcCtrlShare;
    ControllerInfo ctrlInfo;
    Plc_Db plcDataDb, plcControlDb;
    QThread plcdataManageThread;
    PlcDataManageWorker* pdmWorker;
    msgname _dataType;
    int _groupId;

    void resetControlShare(int dataType, QMap<float, QString> controlData, Plc_Db* controlDb);
};

#endif // SYSCONTROLLER_H
