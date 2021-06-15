#ifndef SQLITEHELPER_H
#define SQLITEHELPER_H

#include <QVector>
#include <qdebug.h>
#include <sqlite3.h>

class SQLiteHelper
{
public:
    SQLiteHelper();
    SQLiteHelper(QString hostName, QString dbName, QString userName, QString password);
    bool createTable(QString dbName, QString tableName, QString tableSchema);
    bool insert(QString dbName, QString tableName, QString strQuery);
    bool select(QString dbName, QString tableName, QString strQuery);
    static QVector<QString> Results;

private:
    QString _hostName;
    QString _dbName;
    QString _userName;
    QString _password;

    sqlite3 *db = NULL;
    static int Counts;
    static int select_all_callback(void * data, int col_count, char ** col_values, char ** col_Name);
    static int select_counts_callback(void * data, int col_count, char ** col_values, char ** col_Name);
};

#endif // SQLITEHELPER_H
