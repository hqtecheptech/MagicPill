#include "databaseworker.h"

DatabaseWorker::DatabaseWorker(QObject *parent) : QObject(parent)
{
    SQLiteHelper sqlHelper;
    dbCreated = sqlHelper.createTable(db, table, schema);
    if(dbCreated)
    {
        emit dbError("OK");
    }
}

void DatabaseWorker::saveHistData(HistData hist)
{
    QString strSql = "insert into curve_hist([histId], [deviceId], [deviceGroup], [deviceIndex], [name]"
                     ", [dataType], [address], [value], [index], [insertTime]) values "
                     "(null, 6, 0, 1, 'Speed', 'di', '16', '0.6', 20, '1546393448');";
    bool result = dbHelper.exec(db, table, strSql);
    if(result)
    {
        qDebug() << "mission completed !";
    }
    else
    {
        qDebug() << "mission failed !";
    }
}

void DatabaseWorker::queryHistDatas(HistData hist)
{

}
