#include <QCoreApplication>
#include "swserver.h"
#include "global.h"
#include <QFile>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SwServer *swServer = new SwServer();

    int pid = Global::getPid("HqswServer");

    QFile file("hqsw_server.pid");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Open log file failed!";
    }
    else
    {
        file.write(QString::number(pid).toLatin1());
        file.close();
    }

    return a.exec();
}
