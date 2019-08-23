#include "yhcc.h"
#include "ui_yhcc.h"
#include "syscontroller.h"

#include <QMap>
#include <QVector>

Yhcc::Yhcc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Yhcc)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    deviceIndex = 0;

    qRegisterMetaType<Plc_Db>("Hq_Plc_Db");

    QString eixtStyleStr="QPushButton#yhcExitButton{background: transparent; background-image: url(:/pic/退出.png);}"
                         "QPushButton#yhcExitButton:hover{background: transparent; background-image: url(:/pic/退出.png);}"
                         "QPushButton#yhcExitButton:pressed{background: transparent; background-image: url(:/pic/退出.png);}";
    ui->yhcExitButton->setStyleSheet(eixtStyleStr);

    ui->speedDownButton->setStyleSheet("QPushButton#speedDownButton{background: transparent; background-image: url(:/pic/_02.png);}");
    ui->speedUpButton->setStyleSheet("QPushButton#speedUpButton{background: transparent; background-image: url(:/pic/+02.png);}");

    ui->yhDownButton->setStyleSheet("QPushButton#yhDownButton{background: transparent; background-image: url(:/pic/_02.png);}");
    ui->yhUpButton->setStyleSheet("QPushButton#yhUpButton{background: transparent; background-image: url(:/pic/+02.png);}");

    QString speedLabelStyleStr = "background: transparent;"
                                 "background-position:center;"
                                 "background-repeat:no-repeat;"
                                 "background-image: url(:/pic/输入框.png);"
                                 "color:white;";
    ui->speedLabel->setStyleSheet(speedLabelStyleStr);
    ui->yhChoiseLabel->setStyleSheet(speedLabelStyleStr);

    //QString titleLabelStypeStr = "background: transparent;"
     //                            "color:rgb(200, 200, 200);";

    //ui->titleLabel->setStyleSheet(titleLabelStypeStr);

    checkNetStateTimer = new QTimer(this);
    connect(checkNetStateTimer, SIGNAL(timeout()), this, SLOT(getNetState()));

    pollDatasTimer = new QTimer(this);
    //connect(pollDatasTimer, SIGNAL(timeout()), this, SLOT(pollPlcDatas()));

    controller = Syscontroller::getInstance();
    if(controller != Q_NULLPTR)
    {
        connect(controller, SIGNAL(resultReady()), this, SLOT(handleControllerResult()));
        connect(controller, &Syscontroller::plcDbUpdated, this, &Yhcc::handlePlcDataUpdate);
    }
}

Yhcc::~Yhcc()
{
    delete ui;
}

void Yhcc::on_yhcExitButton_clicked()
{
    close();
}

void Yhcc::on_speedDownButton_pressed()
{
    ui->speedDownButton->setStyleSheet("QPushButton#speedDownButton{background: transparent; background-image: url(:/pic/-01.png);}");
}

void Yhcc::on_speedDownButton_released()
{
    ui->speedDownButton->setStyleSheet("QPushButton#speedDownButton{background: transparent; background-image: url(:/pic/_02.png);}");
}

void Yhcc::on_speedUpButton_pressed()
{
    ui->speedUpButton->setStyleSheet("QPushButton#speedUpButton{background: transparent; background-image: url(:/pic/+01.png);}");
}

void Yhcc::on_speedUpButton_released()
{
    ui->speedUpButton->setStyleSheet("QPushButton#speedUpButton{background: transparent; background-image: url(:/pic/+02.png);}");
}

void Yhcc::on_yhDownButton_pressed()
{
    ui->yhDownButton->setStyleSheet("QPushButton#yhDownButton{background: transparent; background-image: url(:/pic/-01.png);}");
}

void Yhcc::on_yhDownButton_released()
{
    ui->yhDownButton->setStyleSheet("QPushButton#yhDownButton{background: transparent; background-image: url(:/pic/_02.png);}");
}

void Yhcc::on_yhUpButton_pressed()
{
    ui->yhUpButton->setStyleSheet("QPushButton#yhUpButton{background: transparent; background-image: url(:/pic/+01.png);}");
}

void Yhcc::on_yhUpButton_released()
{
    ui->yhUpButton->setStyleSheet("QPushButton#yhUpButton{background: transparent; background-image: url(:/pic/+02.png);}");
}

void Yhcc::netStatChecked(bool state)
{
    if(!state)
    {
        ui->wlanLabel->setStyleSheet("QLabel#wlanLabel{background: transparent;"
                                     "background-position:center;"
                                     "background-repeat:no-repeat;"
                                     "background-image: url(:/pic/警告.png);}");
    }
    else
    {
        ui->wlanLabel->setStyleSheet("QLabel#wlanLabel{background: transparent;"
                                     "background-position:center;"
                                     "background-repeat:no-repeat;"
                                     "background-image: url(:/pic/信号4.png);}");
    }
}

void Yhcc::getNetState()
{
    emit checkNetState("wlan");
}

void Yhcc::pollPlcDatas()
{
    uca ++;
    if(uca == 5)
    {
        qsrand(time(NULL));
        int n = qrand() % 20 + 5;
        ui->widget_2->updateUI(n);

        int leftValue = qrand() % 400 + 100;
        int rightValue = qrand() % 400 + 100;
        ui->yhcWatchsWidget->updateDydl(leftValue, rightValue);

        leftValue = qrand() % 120 - 30;
        rightValue = qrand() % 1000 + 300;
        ui->yhcWatchsWidget->updateWdyw(leftValue, rightValue);

        uca = 0;
    }

    emit pollingDatas();
}

void Yhcc::showEvent(QShowEvent *)
{
    if(!netManageThread.isRunning())
    {
        nsmWorker = new NetStateManageWorker;
        nsmWorker->moveToThread(&netManageThread);
        connect(&netManageThread, &QThread::finished, nsmWorker, &QObject::deleteLater);
        connect(this, SIGNAL(checkNetState(QString)), nsmWorker, SLOT(checkNetState(QString)));
        connect(nsmWorker, SIGNAL(checkNetFinished(bool)), this, SLOT(netStatChecked(bool)));
        netManageThread.start();
    }
    if(!plcdataManageThread.isRunning())
    {
        pdmWorker = new PlcDataManageWorker;
        pdmWorker->moveToThread(&plcdataManageThread);
        connect(&plcdataManageThread, &QThread::finished, pdmWorker, &QObject::deleteLater);
        connect(this, SIGNAL(pollingDatas()), pdmWorker, SLOT(getSharedDatas()));
        connect(pdmWorker, SIGNAL(sharedDatasReady(Plc_Db)), this, SLOT(updateUI(Plc_Db)));
        plcdataManageThread.start();
    }

    QTimer::singleShot(5000, this, SLOT(getNetState()));
    if(!checkNetStateTimer->isActive())
    {
        checkNetStateTimer->start(60000);
    }

    if(!pollDatasTimer->isActive())
    {
        pollDatasTimer->start(2000);
    }
}

void Yhcc::closeEvent(QCloseEvent *)
{
    if(checkNetStateTimer->isActive())
    {
        checkNetStateTimer->stop();
    }
    if(pollDatasTimer->isActive())
    {
        pollDatasTimer->stop();
    }

    netManageThread.requestInterruption();
    netManageThread.quit();
    netManageThread.wait();

    plcdataManageThread.requestInterruption();
    plcdataManageThread.quit();
    plcdataManageThread.wait();
}

void Yhcc::updateUI(const Plc_Db newDatas)
{
    ui->test_label->setText(QString::number(newDatas.f_data[0]));
}

void Yhcc::handleControllerResult()
{

}


void Yhcc::handlePlcDataUpdate(QSet<int> changedDeviceSet, QMap<float,QString> dataMap)
{
    if(changedDeviceSet.contains(deviceIndex))
    {
        parseYhcData(dataMap);
        parseYhcRunCtrData(dataMap);
    }
}

void Yhcc::parseYhcData(QMap<float, QString> dataMap)
{
    pollPlcDatas();
}

void Yhcc::parseYhcRunCtrData(QMap<float, QString> dataMap)
{

}
