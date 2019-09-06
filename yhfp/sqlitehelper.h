#ifndef SQLITEHELPER_H
#define SQLITEHELPER_H

#include <qdebug.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class SQLiteHelper
{
public:
    SQLiteHelper();
    SQLiteHelper(QString hostName, QString dbName, QString userName, QString password);
    bool createTable(QString dbName, QString tableName);
    bool exec(QString strQuery);

private:
    QSqlDatabase _db;
    QSqlQuery _query;
    QString _hostName;
    QString _dbName;
    QString _userName;
    QString _password;

};

#endif // SQLITEHELPER_H
