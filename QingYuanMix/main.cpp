#include "mainwindow.h"
#include <signal.h>
#include <QApplication>

//接收信号更新共享内存数据
static void sig_handler_from_rpuData(int sig)
{
    if(sig == SIG_FROM_REMOTE)
    {
        qDebug() << "Received sig from remote";
        //mHD_Read_Shm_ShareMemory_DevData(Run_data.Shmkey ,Run_data.Semkey,&Dev_data);  //读共享内存
    }
}

int main(int argc, char *argv[])
{
    //注册PRU Update 信号函数
    //signal(SIG_FROM_REMOTE,sig_handler_from_rpuData);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
