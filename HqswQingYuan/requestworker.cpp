#include "requestworker.h"

RequestWorker::RequestWorker(QObject *parent) : QObject(parent)
{

}

void RequestWorker::doWork(StreamPack pack)
{
    QByteArray result;
    emit resultReady(result);
}
