#ifndef SWSERVER_H
#define SWSERVER_H

#include <QObject>

#include <QThread>
#include <QTimer>
#include "taskmanager.h"
#include "syscontroller.h"

class SwServer : public QObject
{
    Q_OBJECT
public:
    explicit SwServer(QObject *parent = 0);

signals:
    void startListenTask();

public slots:
    void writeTestData();

private:
    QThread taskManageThread;
    TaskManager* taskManager;
    //QTimer* pruCheckTimer;
    Syscontroller *controller;
    QTimer* testTimer;
    bool started;
};

#endif // SWSERVER_H
