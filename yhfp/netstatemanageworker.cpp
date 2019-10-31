#include "netstatemanageworker.h"
#include <QProcess>
#include <QDebug>

NetStateManageWorker::NetStateManageWorker(QObject *parent) : QObject(parent)
{

}

void NetStateManageWorker::checkNetState(QString type)
{
    QProcess *cmd = new QProcess;
    QString strArg;
    if(type == "wlan")
    {
        strArg = "ping -s 1 -c 1 192.168.0.200";
    }
    else if(type == "eth")
    {
        strArg = "ping -s 1 -c 1 192.168.0.90";
    }
    /*#ifdef _TTY_ARMV4_
    QString strArg = "ping -s 1 -c 1 " + "192.168.0.200";        //linux平台下的格式
    #else
    QString strArg = "ping " + b_tmpIpStr + " -n 1 -w " + QString::number(m_timeoutInt) ;  //windows下的格式
    #endif*/
    cmd->start(strArg);
    cmd->waitForReadyRead();
    cmd->waitForFinished();
    QString retStr = cmd->readAll();
    if (retStr.toUpper().indexOf("TTL") != -1)
    {
        //qDebug() << type <<"is online!\n";
        emit checkNetFinished(type, true);
    }
    else
    {
        emit checkNetFinished(type,false);
    }
}
