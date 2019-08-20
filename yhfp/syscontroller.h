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

signals:

public slots:
    void checkSysStatus();

private:
    Syscontroller();
    Syscontroller(const Syscontroller&);
    static Syscontroller* instance;
    static QMutex* mutex;
    QTimer *updateStatusTimer;
    ShareHelper* sp;
    ControllerInfo ctrlInfo;
};

#endif // SYSCONTROLLER_H
