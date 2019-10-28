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

    controller = Syscontroller::getInstance(fjsw, 0);
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

    updateWatchsTimer = new QTimer(this);
    connect(updateWatchsTimer, SIGNAL(timeout()), this, SLOT(updateWatchs()));
    updateWatchsTimer->start(5000);

    hisDlg = new HistoryDlg(this);
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

void Yhcc::netStatChecked(QString type, bool state)
{
    if(type == "wlan")
    {
        if(!state)
        {
            ui->wlanLabel->setStyleSheet("QLabel#wlanLabel{background: transparent;"
                                         "background-position:center;"
                                         "background-repeat:no-repeat;"
                                         "background-image: url(:/pic/no_wifi.png);}");
        }
        else
        {
            ui->wlanLabel->setStyleSheet("QLabel#wlanLabel{background: transparent;"
                                         "background-position:center;"
                                         "background-repeat:no-repeat;"
                                         "background-image: url(:/pic/信号4.png);}");
        }
    }
    else if(type == "eth")
    {
        if(!state)
        {
            ui->ethLabel->setStyleSheet("QLabel#ethLabel{background: transparent;"
                                         "background-position:center;"
                                         "background-repeat:no-repeat;"
                                         "background-image: url(:/pic/no_signal.png);}");
        }
        else
        {
            ui->ethLabel->setStyleSheet("QLabel#ethLabel{background: transparent;"
                                         "background-position:center;"
                                         "background-repeat:no-repeat;"
                                         "background-image: url(:/pic/信号2.png);}");
        }
    }
}

void Yhcc::getNetState()
{
    emit checkNetState("wlan");
    emit checkNetState("eth");
}

void Yhcc::showEvent(QShowEvent *)
{
    if(!netManageThread.isRunning())
    {
        nsmWorker = new NetStateManageWorker;
        nsmWorker->moveToThread(&netManageThread);
        connect(&netManageThread, &QThread::finished, nsmWorker, &QObject::deleteLater);
        connect(this, SIGNAL(checkNetState(QString)), nsmWorker, SLOT(checkNetState(QString)));
        connect(nsmWorker, SIGNAL(checkNetFinished(QString,bool)), this, SLOT(netStatChecked(QString,bool)));
        netManageThread.start();
    }

    QTimer::singleShot(5000, this, SLOT(getNetState()));
    if(!checkNetStateTimer->isActive())
    {
        checkNetStateTimer->start(10000);
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

void Yhcc::handleControllerResult()
{

}


void Yhcc::handlePlcDataUpdate(QSet<int> changedDeviceSet, QMap<float,QString> dataMap)
{
    if(changedDeviceSet.count() > 0)
    {
        parseYhcData(dataMap);
        parseYhcRunCtrData(dataMap);
    }
}

void Yhcc::wirteTestData()
{
    //controller->yhcSpeedUp(deviceIndex, 5);
    controller->yhcStart(0, !started);
}

void Yhcc::updateWatchs()
{
    /*qsrand(time(NULL));
    int rs = qrand() % 20 + 5;
    int prs = qrand() % 20 + 5;
    ui->widget_2->updateUI(rs, prs);*/

    int leftValue = qrand() % 400 + 100;
    int rightValue = qrand() % 400 + 100;
    ui->yhcWatchsWidget->updateDydl(leftValue, rightValue);

    leftValue = qrand() % 120 - 30;
    rightValue = qrand() % 1000 + 300;
    ui->yhcWatchsWidget->updateWdyw(leftValue, rightValue);

    QDateTime currentdt = QDateTime::currentDateTime();
    uint stime =currentdt.toTime_t();
    HistData data;

    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Speed");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index = Global::convertAddressToIndex(address, deviceNode.DataType);
    //ui->test_label->setText(Global::currentYhcDataMap.value(address, "false"));

    data.address = address;
    strcpy(data.dataType, deviceNode.Name.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.deviceIndex = index;
    data.index = 10;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Speed").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);

    deviceNode = Global::getYhcNodeInfoByName("RevolvingSpeed");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    int rs = Global::currentYhcDataMap.value(address).toFloat();

    data.address = address;
    strcpy(data.dataType, deviceNode.Name.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.deviceIndex = index;
    data.index = 10;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("RevolvingSpeed").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);

    deviceNode = Global::getYhcNodeInfoByName("Ampere1");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    int prs = Global::currentYhcDataMap.value(address).toFloat();

    data.address = address;
    strcpy(data.dataType, deviceNode.Name.toLatin1().data());
    data.deviceGroup = info.groupId;
    data.deviceId = deviceNode.Id;
    data.deviceIndex = index;
    data.index = 10;
    strcpy(data.insertTime, QString::number(stime).toLatin1().data());
    strcpy(data.name, QString("Ampere1").toLatin1().data());
    strcpy(data.value, Global::currentYhcDataMap.value(address).toLatin1().data());
    emit histDataReady(data);

    ui->widget_2->updateUI(rs, prs);
}

void Yhcc::parseYhcData(QMap<float, QString> dataMap)
{
    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getYhcNodeInfoByName("Tempture");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int index = Global::convertAddressToIndex(address, deviceNode.DataType);
    //ui->test_label->setText(Global::currentYhcDataMap[address]);

    deviceNode = Global::getYhcNodeInfoByName("Speed");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    qDebug() << "Speed value: " << Global::currentYhcDataMap[address];
    ui->speedLabel->setText(Global::currentYhcDataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("FER_Hand_RunTime");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    index = Global::convertAddressToIndex(address, deviceNode.DataType);
    qDebug() << "FER_Hand_RunTime: " << Global::currentFermenationDataMap[address];
    ui->test_label->setText(Global::currentFermenationDataMap[address]);
}

void Yhcc::parseYhcRunCtrData(QMap<float, QString> dataMap)
{
    bool value = Global::getFerRunctrValueByName(deviceIndex, "FER_Auto_BOOL", dataMap);
    ui->test_label_2->setText(QString::number(value));
    value = Global::getFerRunctrValueByName(deviceIndex + 1, "FER_Auto_BOOL", dataMap);
    ui->test_label_3->setText(QString::number(value));
    value = Global::getFerRunctrValueByName(deviceIndex + 5, "FER_Auto_BOOL", dataMap);
    ui->test_label_4->setText(QString::number(value));
    // to do: just for test temporay.
    if(value == 0)
    {
        started = true;
    }
    else
    {
        started = false;
    }
}

void Yhcc::on_speedDownButton_clicked()
{
    controller->yhcSpeedUp(deviceIndex, -1);
}

void Yhcc::on_speedUpButton_clicked()
{
    controller->yhcSpeedUp(deviceIndex, 1);
}

void Yhcc::on_historyButton_clicked()
{
    hisDlg->show();
}
