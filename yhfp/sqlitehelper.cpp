#include "sqlitehelper.h"
#include <stdio.h>
#include <iostream>

int SQLiteHelper::counts;

int SQLiteHelper::select_all_callback(void * data, int col_count, char ** col_values, char ** col_Name)
{
    // 每条记录回调一次该函数,有多少条就回调多少次
    int i;
    char s[50];
    for( i=0; i < col_count; i++)
    {
        //printf( "%s = %s\n", col_Name[i], col_values[i] == 0 ? "NULL" : col_values[i] );
        sprintf( s, "%s = %s", col_Name[i], col_values[i] == 0 ? "NULL" : col_values[i] );
        qDebug() << s;
    }

    return 0;
}

int SQLiteHelper::select_counts_callback(void *data, int col_count, char **col_values, char **col_Name)
{
    if(col_count == 1 && QString(col_Name[0]) == "counts")
    {
        counts = QString(col_values[0]).toInt();
    }

    return 0;
}

SQLiteHelper::SQLiteHelper()
{
    delete db;
}

SQLiteHelper::SQLiteHelper(QString hostName, QString dbName, QString userName, QString password)
{
    _hostName = hostName;
    _dbName = dbName;
    _userName = userName;
    _password = password;
}

bool SQLiteHelper::createTable(QString dbName, QString tableName, QString tableSchema)
{
    char* zErrMsg = 0;
    int rc;
    bool ret;
    QByteArray ba = dbName.toLatin1();
    QByteArray bb = tableName.toLatin1();
    QByteArray bc = tableSchema.toLatin1();

    rc = sqlite3_open(ba.data(), &db);

    if(rc != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db);
        ret = false;
    }
    else
    {
        qDebug() << "you have opened a sqlite3 database name"
             << ba.data() << "successfully!";
        //const char* sql = "select * from sqlite_master where name='hist1'";
        const char* sql = "select * from " + bb + " where 1=0;";
        rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
        if(rc != SQLITE_OK)
        {
            qDebug() << zErrMsg;
            sqlite3_free(zErrMsg);

            /*sql = "create table hist2([histId] integer PRIMARY KEY AUTOINCREMENT, "
                  "[insertTime] TimeStamp NOT NULL DEFAULT (datetime('now','localtime')), "
                  "[value] integer);";*/
            sql = bc.data();
            rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
            if(rc != SQLITE_OK)
            {
                qDebug() << "In create " + bb + " table: " << zErrMsg;
                sqlite3_free(zErrMsg);
                ret = false;
            }
            else
            {
                qDebug() << bb + " table has been created successfully!";
                ret = true;
            }
        }
        else
        {
            qDebug() << bb + " table is already exist!";
            ret = true;
        }
    }
    return ret;
}

bool SQLiteHelper::exec(QString dbName, QString tableName, QString strQuery)
{
    char* zErrMsg = 0;
    int rc;
    bool ret;
    QByteArray ba = dbName.toLatin1();
    QByteArray bb = tableName.toLatin1();
    QByteArray bc = strQuery.toLatin1();

    rc = sqlite3_open(ba.data(), &db);

    if(rc != SQLITE_OK)
    {
        qDebug() << sqlite3_errmsg(db);
        ret = false;
    }
    else
    {
        qDebug() << "you have opened a sqlite3 database name"
             << ba.data() << "successfully!";
        //const char* sql = "select * from sqlite_master where name='hist1'";
        const char* sql = "select * from " + bb + " where 1=0;";
        rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
        if(rc == SQLITE_OK)
        {
            sql = "select count(*) counts from " + bb + ";";
            sqlite3_exec(db, sql, select_counts_callback, 0, &zErrMsg);
            qDebug() << "Records number : " << counts;
            if(counts > 1000)
            {

            }

            //sql = "insert into hist1 ([histId],[value]) values (null, 20);";
            sql = bc.data();
            rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
            if(rc != SQLITE_OK)
            {
                qDebug() << "In insert " + bb + " table: " << zErrMsg;
                sqlite3_free(zErrMsg);
                ret = false;
            }
            else
            {
                qDebug() << "insert " + bb + " ok";
                ret = true;
            }

            /*sql = "select * from " + bb + ";";
            rc = sqlite3_exec(db, sql, select_all_callback, 0, &zErrMsg);
            if(rc != SQLITE_OK)
            {
                qDebug() << "In select " + bb + " table: " << zErrMsg;
                sqlite3_free(zErrMsg);
                ret = false;
            }
            else
            {
                qDebug() << "select " + bb + " ok";
                ret = true;
            }*/
        }
        else
        {
            qDebug() << "In exec sql: " << zErrMsg;
            sqlite3_free(zErrMsg);
            ret = false;
        }
    }
    return true;
}
