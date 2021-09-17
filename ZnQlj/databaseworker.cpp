#include "databaseworker.h"
#include <QStringBuilder>

DatabaseWorker::DatabaseWorker(QObject *parent) : QObject(parent)
{
    SQLiteHelper sqlHelper;
    dbCreated = sqlHelper.createTable(db, table, schema);
    if(dbCreated)
    {
        emit dbError("OK");
    }
    dbCreated = sqlHelper.createTable(db, simpleAlertTable, schema1);
    if(dbCreated)
    {
        emit dbError("OK");
    }
}

void DatabaseWorker::saveHistData(HistData hist)
{
    QString strSql;
    strSql += "insert into curve_hist([histId], [deviceId], [deviceGroup], [deviceIndex], [name]"
          ", [dataType], [address], [value], [index], [insertTime]) values (";
    strSql += "null, ";
    strSql += QString::number(hist.deviceId);
    strSql += ", ";
    strSql += QString::number(hist.deviceGroup);
    strSql += ", ";
    strSql += QString::number(hist.deviceIndex);
    strSql += ", ";
    strSql += "'" + QString(hist.name) + "'";
    strSql += ", ";
    strSql += "'" + QString(hist.dataType) + "'";
    strSql += ", ";
    strSql += QString::number(hist.address);
    strSql += ", ";
    strSql += "'" + QString(hist.value) + "'";
    strSql += ", ";
    strSql += QString::number(hist.index);
    strSql += ", ";
    strSql += "'" + QString(hist.insertTime) + "'";
    strSql += ");";

    /*QString strSql = "insert into curve_hist([histId], [deviceId], [deviceGroup], [deviceIndex], [name]"
                     ", [dataType], [address], [value], [index], [insertTime]) values "
                     "(null, 6, 0, 1, 'Speed', 'di', 16, '0.6', 20, '1546393448');";*/

    bool result = dbHelper.insert(db, table, strSql);
    if(result)
    {
        //qDebug() << "mission completed !";
    }
    else
    {
        //qDebug() << "mission failed !";
    }
}

void DatabaseWorker::queryHistDatas(HistData hist)
{

}

void DatabaseWorker::clearAlertItem(QList<QStandardItem *> alert)
{
    qDeleteAll(alert.begin(), alert.end());
    alert.clear();
}

void DatabaseWorker::saveAlert(QString alert)
{
    QString strSql;
    strSql += "insert into simple_alert([alertId], [value]) values (";
    strSql += "null, ";
    strSql += "'" + alert + "'";
    strSql += ");";

    bool result = dbHelper.insert(db, simpleAlertTable, strSql);
    if(result)
    {
        //qDebug() << "mission completed !";
    }
    else
    {
        //qDebug() << "mission failed !";
    }
}
