#ifndef IDENTITY_H
#define IDENTITY_H

#include "user.h"
#include "QTimer"

#include <QMutex>

class Identity : public QObject
{ 
    Q_OBJECT
public:
    static Identity* getInstance();

    User *getUser();
    void setUser(User *value);

private:
    Identity();
    Identity(const Identity&);
    //Identity& operator=(const Identity&);
    static Identity* instance;
    static QMutex* mutex;
    User* user = Q_NULLPTR;
    QTimer *overDueTimer;

public slots:
    void reset();

signals:
    void logout();
};

#endif // IDENTITY_H
