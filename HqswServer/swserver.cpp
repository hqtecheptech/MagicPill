#include "swserver.h"

SwServer::SwServer(QObject *parent) : QObject(parent)
{
    taskManager = new TaskManager(8000, this);
    taskManager->moveToThread(&taskManageThread);
    connect(&taskManageThread, SIGNAL(finished()), taskManager, SLOT(deleteLater()));
    connect(this, SIGNAL(startListenTask()), taskManager, SLOT(listeningTask()));

    emit(startListenTask());

    controller = Syscontroller::getInstance(Global::systemConfig.deviceType, Global::systemConfig.deviceGroup);

    testTimer = new QTimer();
    connect(testTimer, SIGNAL(timeout()), this, SLOT(writeTestData()));
    testTimer->start(5000);
}

void SwServer::writeTestData()
{
    controller->changeDataValue(0, 2);
    controller->changeDataValue(5, 3);
    started = !started;
    controller->changeRunctrlValue(1, started);
}
