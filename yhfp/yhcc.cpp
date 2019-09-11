#include "yhcc.h"
#include "ui_yhcc.h"

#include <QMap>
#include <QVector>

Yhcc::Yhcc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Yhcc)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    deviceIndex = 0;

    st.start();

    qRegisterMetaType<Plc_Db>("Plc_Db");
    qRegisterMetaType<HistData>("HistData");

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

    testTimer = new QTimer(this);
    connect(testTimer, SIGNAL(timeout()), this, SLOT(wirteTestData()));

    controller = Syscontroller::getInstance();
    if(controller != Q_NULLPTR)
    {
        connect(controller, SIGNAL(resultReady()), this, SLOT(handleControllerResult()));
        connect(controller, &Syscontroller::plcDbUpdated, this, &Yhcc::handlePlcDataUpdate);
        connect(this, SIGNAL(requestControl()), controller, SLOT(applyControlRequest()));
    }

    dbWorker = new DatabaseWorker;
    dbWorker->moveToThread(&dbThread);
    connect(&dbThread, &QThread::finished, dbWorker, &QObject::deleteLater);
    connect(this, SIGNAL(histDataReady(HistData)), dbWorker, SLOT(saveHistData(HistData)));
    dbThread.start();
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

    QTimer::singleShot(5000, this, SLOT(getNetState()));
    if(!checkNetStateTimer->isActive())
    {
        checkNetStateTimer->start(60000);
    }

    if(!testTimer->isActive())
    {
        testTimer->start(3000);
    }
}

void Yhcc::closeEvent(QCloseEvent *)
{
    if(checkNetStateTimer->isActive())
    {
        checkNetStateTimer->stop();
    }

    if(testTimer->isActive())
    {
        testTimer->stop();
    }

    netManageThread.requestInterruption();
    netManageThread.quit();
    netManageThread.wait();
}

void Yhcc::updateUI()
{
    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Tempture");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index = Global::convertYhcAddressToIndex(address, deviceNode.DataType);
    ui->test_label->setText(Global::currentYhcDataMap[address]);

    deviceNode = Global::getYhcNodeInfoByName("Speed");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertYhcAddressToIndex(address, deviceNode.DataType);
    ui->speedLabel->setText(Global::currentYhcDataMap[address]);

    bool value = Global::getYhcRunctrValueByName(deviceIndex, "Run_Signal", Global::currentYhcDataMap);
    ui->test_label_2->setText(QString::number(value));
    value = Global::getYhcRunctrValueByName(deviceIndex, "False_Signal", Global::currentYhcDataMap);
    ui->test_label_3->setText(QString::number(value));
    value = Global::getYhcRunctrValueByName(deviceIndex, "FAN_SPAREVALVE_Opened", Global::currentYhcDataMap);
    ui->test_label_4->setText(QString::number(value));

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

void Yhcc::wirteTestData()
{
    controller->yhcSpeedUp(deviceIndex, 5);
    /*emit requestControl();*/

    /*plcdata.f_data[0] += 5;
    plcdata.f_data[2] += 0.5;

    qDebug() << "r 0 address " << Global::convertYhcIndexToAddress(0, "r");
    qDebug() << "r 2 address " << Global::convertYhcIndexToAddress(2, "r");

    if(plcdata.i_data[1] == 5)
    {
       plcdata.i_data[1] = -4;
    }
    else
    {
        plcdata.i_data[1] = 5;
    }
    qDebug() << "di 1 address " << Global::convertYhcIndexToAddress(1, "di");

    plcdata.dw_data[3] += 2;
    qDebug() << "dw 1 address " << Global::convertYhcIndexToAddress(3, "dw");

    plcdata.w_data[1] += 2;
    qDebug() << "w 1 address " << Global::convertYhcIndexToAddress(1, "w");

    if(plcdata.b_data[0] == 1)
    {
       plcdata.b_data[0] = 0;
    }
    else
    {
        plcdata.b_data[0] = 1;
    }

    if(plcdata.b_data[2] == 1)
    {
       plcdata.b_data[2] = 0;
    }
    else
    {
        plcdata.b_data[2] = 1;
    }
    qDebug() << "x0 2 address " << Global::convertYhcIndexToAddress(2, "x0");

    if(plcdata.b_data[9] == 1)
    {
       plcdata.b_data[9] = 0;
    }
    else
    {
        plcdata.b_data[9] = 1;
    }
    qDebug() << "x0 9 address " << Global::convertYhcIndexToAddress(9, "x0");

    controller->setPlcControlDb(plcdata);
    controller->unlockPlcDataDb();*/
}

void Yhcc::parseYhcData(QMap<float, QString> dataMap)
{
    updateUI();
}

void Yhcc::parseYhcRunCtrData(QMap<float, QString> dataMap)
{

}

void Yhcc::on_speedDownButton_clicked()
{
    controller->yhcSpeedUp(deviceIndex, 1);
    //emit requestControl();
}

void Yhcc::on_speedUpButton_clicked()
{
    HistData data;
    data.address = 20;
    strcpy(data.dataType, QString("r").toLatin1().data());
    data.deviceGroup = 0;
    data.deviceId = 6;
    data.deviceIndex = 1;
    data.index = 10;
    strcpy(data.insertTime, QString("2019:09:10 12:24:36").toLatin1().data());
    strcpy(data.name, QString("Pressure").toLatin1().data());
    strcpy(data.value, QString("56.8").toLatin1().data());

    emit histDataReady(data);
}
