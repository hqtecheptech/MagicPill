#include "mytimerthread.h"

MyTimerThread::MyTimerThread(int interval, QObject *parent)
{
    _interval = interval;
}

void MyTimerThread::run()
{
    while(true)
    {
        sleep(_interval);
        emit timeout();
        //exec();
    }
}
