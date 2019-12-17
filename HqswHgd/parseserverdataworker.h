#ifndef PARSESERVERDATAWORKER_H
#define PARSESERVERDATAWORKER_H

#include <QObject>
#include <QMap>

class ParseServerDataWorker : public QObject
{
    Q_OBJECT
public:
    explicit ParseServerDataWorker(QObject *parent = 0);
    bool deoDataFileSaved = false;
    bool ferDataFileSaved = false;

signals:
    void resultReady(QSet<int>, QMap<float,QString> dataMap);

public slots:
    void parseFerServerData(QByteArray data);
    void parseDeoServerData(QByteArray data);
};

#endif // PARSESERVERDATAWORKER_H
