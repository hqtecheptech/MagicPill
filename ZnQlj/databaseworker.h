#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H

#include <QObject>
#include "data.h"
#include "sqlitehelper.h"

#include "QStandardItem"
#include "QList"

class DatabaseWorker : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseWorker(QObject *parent = 0);

signals:
    void resultReady(HistData* hist);
    void dbError(QString message);

public slots:
    void saveHistData(HistData hist);
    void queryHistDatas(HistData hist);
    void clearAlertItem(QList<QStandardItem*> alert);
    void saveAlert(QString alert);

private:
    SQLiteHelper dbHelper;
    bool dbCreated = false;
    QString db = "hq_yhfp_data.db";
    QString table = "curve_hist";
    QString schema = "create table curve_hist([histId] integer PRIMARY KEY AUTOINCREMENT, "
                     "[deviceId] integer NOT NULL, "
                     "[deviceGroup] integer NOT NULL, "
                     "[deviceIndex] integer NOT NULL, "
                     "[name] varchar(50) NOT NULL, "
                     "[dataType] varchar(5) NOT NULL, "
                     "[address] real NOT NULL, "
                     "[value] varchar(20) NOT NULL, "
                     "[index] integer NOT NULL, "
                     "[insertTime] varchar(30) NOT NULL);";

    QString simpleAlertTable = "simple_alert";
    QString schema1 = "create table simple_alert([alertId] integer PRIMARY KEY AUTOINCREMENT, "
                     "[value] varchar(100) NOT NULL);";
};

#endif // DATABASEWORKER_H
