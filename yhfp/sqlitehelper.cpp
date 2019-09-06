#include "sqlitehelper.h"

SQLiteHelper::SQLiteHelper()
{
    /*_db = QSqlDatabase::addDatabase("SQLite");
    _db.setDatabaseName("MyDataBase.db");
    if (!_db.open())
    {
        qDebug() << "drivers" << QSqlDatabase::drivers();
        qDebug() << "Error: Failed to connect database." << _db.lastError();
    }
    else
    {
        qDebug() << "Succeed to connect database." ;
    }*/
}

SQLiteHelper::SQLiteHelper(QString hostName, QString dbName, QString userName, QString password)
{
    //_db = QSqlDatabase::addDatabase("SQLite");
    _hostName = hostName;
    _dbName = dbName;
    _userName = userName;
    _password = password;
}

bool SQLiteHelper::createTable(QString dbName, QString tableName)
{
    return true;
}

bool SQLiteHelper::exec(QString strQuery)
{
    return true;
}
