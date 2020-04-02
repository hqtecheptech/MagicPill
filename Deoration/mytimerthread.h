#ifndef MYTIMERTHREAD_H
#define MYTIMERTHREAD_H

#include <QThread>

class MyTimerThread : public QThread
{
        Q_OBJECT
public:
    MyTimerThread(int interval, QObject *parent = 0);

protected:
    void run() override;

signals:
    void timeout();

private:
    int _interval;

};


#endif // MYTIMERTHREAD_H
