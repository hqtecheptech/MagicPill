#ifndef REQUESTWORKER_H
#define REQUESTWORKER_H

#include <QObject>

#include "dataformat.h"

class RequestWorker : public QObject
{
    Q_OBJECT
public:
    explicit RequestWorker(QObject *parent = 0);

public slots:
    void doWork(StreamPack pack);

signals:
    void resultReady(QByteArray result);
};

#endif // REQUESTWORKER_H
