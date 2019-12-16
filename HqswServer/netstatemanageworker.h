#ifndef NETSTATEMANAGEWORKER_H
#define NETSTATEMANAGEWORKER_H

#include <QObject>

class NetStateManageWorker : public QObject
{
    Q_OBJECT
public:
    explicit NetStateManageWorker(QObject *parent = 0);

signals:
    void checkNetFinished(QString type, bool state);

public slots:
    void checkNetState(QString type);
};

#endif // NETSTATEMANAGEWORKER_H
