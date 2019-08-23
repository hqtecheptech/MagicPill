#ifndef SYSCONTROLLER_H
#define SYSCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include "data.h"
#include "global.h"
#include "sharehelper.h"
#include "../mhd_lib/inc/Share_Memory.h"

class Syscontroller : public QObject
{
    Q_OBJECT
public:
    static Syscontroller* getInstance();
    ControllerInfo getControllerStatus();
    Plc_Db getPlcDb();
    void parseFerServerData(Plc_Db dbData);

signals:
    void resultReady();
    void plcDbUpdated(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);

public slots:
    void updateSysStatus();

private:
    Syscontroller(QObject *parent = 0);
    static Syscontroller* instance;
    static QMutex* mutex;
    QTimer *updateStatusTimer;
    ShareHelper *ctrlShare, *dbShare;
    ControllerInfo ctrlInfo;
    Plc_Db plcDb;
};

#endif // SYSCONTROLLER_H
