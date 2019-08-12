#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datasender.h"
#include "data.h"
#include <QVariant>
#include <QTextDecoder>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);


    QString yhccStyleStr="QPushButton#yhccButton{background: transparent; background-image: url(:/pic/车1..png);}"
                         "QPushButton#yhccButton:hover{background: transparent; background-image: url(:/pic/车2.png);}"
                         "QPushButton#yhccButton:pressed{background: transparent; background-image: url(:/pic/车2.png);}";

    QString fpjcStyleStr="QPushButton#fpjcButton{background: transparent; background-image: url(:/pic/翻1.png);}"
                         "QPushButton#fpjcButton:hover{background: transparent; background-image: url(:/pic/翻2.png);}"
                         "QPushButton#fpjcButton:pressed{background: transparent; background-image: url(:/pic/翻2.png);}";

    QString syssStyleStr="QPushButton#syssButton{background: transparent; background-image: url(:/pic/设置1.png);}"
                         "QPushButton#syssButton:hover{background: transparent; background-image: url(:/pic/设置2.png);}"
                         "QPushButton#syssButton:pressed{background: transparent; background-image: url(:/pic/设置2.png);}";

    QString eixtStyleStr="QPushButton#exitButton{background: transparent; background-image: url(:/pic/退出1.png);}"
                         "QPushButton#exitButton:hover{background: transparent; background-image: url(:/pic/退出2.png);}"
                         "QPushButton#exitButton:pressed{background: transparent; background-image: url(:/pic/退出2.png);}";


    ui->yhccButton->setStyleSheet(yhccStyleStr);
    ui->fpjcButton->setStyleSheet(fpjcStyleStr);
    ui->syssButton->setStyleSheet(syssStyleStr);
    ui->exitButton->setStyleSheet(eixtStyleStr);

    yhc = new Yhcc(this);
    //ui->yhccButton->setStyleSheet("QPushButton{background: transparent; background-image: url(:/pic/车1..png);}");
    //ui->fpjcButton->setStyleSheet("QPushButton{background: transparent; background-image: url(:/pic/翻1.png);}");

    taskManager = new TaskManager(8000, this);
    taskManager->moveToThread(&taskManageThread);
    connect(&taskManageThread, SIGNAL(finished()), taskManager, SLOT(deleteLater()));
    connect(this, SIGNAL(startListenTask()), taskManager, SLOT(listeningTask()));

    emit(startListenTask());
}

MainWindow::~MainWindow()
{
    taskManageThread.requestInterruption();
    taskManageThread.quit();
    taskManageThread.wait();

    delete ui;
}

void MainWindow::on_yhccButton_clicked()
{
    yhc->show();
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::quit();
}

void MainWindow::on_syssButton_clicked()
{
    DataSender ds;
    StreamPack bpack;
    bpack = {sizeof(StreamPack),1,0,r_Report,String,0,0,10,0,0,0};
    QTextCodec *codec = QTextCodec::codecForLocale();
    QByteArray data = codec->fromUnicode(QString("1234567890"));
    QVariant var_data = QVariant(data);
    bpack.bStreamLength = 4 + data.length();

    quint16 strlen= sizeof(bpack);
    char *p = new char[strlen];
    QByteArray SData;
    memcpy(p,reinterpret_cast<char*>(&bpack),sizeof(bpack));
    QDataStream out(&SData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out.setByteOrder(QDataStream::LittleEndian);
    for(int i=0;i<strlen;i++) {out << *(p+i);}
    delete p;

    int len = SData.length();
    out << var_data;

    len = SData.length();
    uint scrc = ds.StreamLen_CRC32(SData);
    out << scrc;

    ds.sendRequestWithResults(SData);
}
