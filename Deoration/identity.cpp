#include "identity.h"
#include <QMutexLocker>

Identity::Identity()
{
    overDueTimer = new QTimer(this);
    connect( overDueTimer, SIGNAL(timeout()), this, SLOT(reset()) );
}

Identity::Identity(const Identity &)
{

}

User *Identity::getUser()
{
    return user;
}

void Identity::setUser(User *value)
{
    if(overDueTimer->isActive())
    {
        overDueTimer->stop();
    }
    user = value;
    overDueTimer->start(300000);
}

void Identity::reset()
{
    if(overDueTimer->isActive())
    {
        overDueTimer->stop();
    }
    delete user;
    user = Q_NULLPTR;
    emit logout();
}

Identity* Identity::instance = Q_NULLPTR;
QMutex* Identity::mutex = new QMutex();

Identity* Identity::getInstance()
{
    if (instance == Q_NULLPTR)
    {
        QMutexLocker locker(mutex);
        if (instance == Q_NULLPTR)
        {
            instance = new Identity();
        }
    }
    return instance;
}
