#ifndef SQLITEHELPER_H
#define SQLITEHELPER_H

#include <qdebug.h>
#include <sqlite3.h>

class SQLiteHelper
{
public:
    SQLiteHelper();
    SQLiteHelper(QString hostName, QString dbName, QString userName, QString password);
    bool createTable(QString dbName, QString tableName, QString tableSchema);
    bool exec(QString dbName, QString tableName, QString strQuery);

private:
    QString _hostName;
    QString _dbName;
    QString _userName;
    QString _password;

    sqlite3 *db = NULL;
};

#endif // SQLITEHELPER_H
