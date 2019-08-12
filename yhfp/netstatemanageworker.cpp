#include "netstatemanageworker.h"
#include <QProcess>


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
        //qDebug() << m_curIpStr <<"is online!\n";
        emit checkNetFinished(true);
    }
    else
    {
        emit checkNetFinished(false);
    }
}
