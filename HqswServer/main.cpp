#include <QCoreApplication>
#include "swserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SwServer *swServer = new SwServer();

    return a.exec();
}
