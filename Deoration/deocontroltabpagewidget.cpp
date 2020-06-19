#include "deocontroltabpagewidget.h"
#include "ui_deocontroltabpagewidget.h"
#include "global.h"
#include "identity.h"
#include "basepartwidget.h"

DeoControlTabPageWidget::DeoControlTabPageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeoControlTabPageWidget)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("除臭控制"));

    worker = new ParseServerDataWorker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &DeoControlTabPageWidget::serverDataReceived, worker, &ParseServerDataWorker::parseDeoServerData);
    connect(worker, &ParseServerDataWorker::resultReady, this, &DeoControlTabPageWidget::dispatchDeoData, Qt::QueuedConnection);

    greenArrowImg.load("://image/new/thin_wind_blowing.png");
    brownArrowImg.load("://image/new/thin_wind_still.png");
    greenArrowUpImg.load("://image/new/thin_wind_blowing_up.png");
    brownArrowUpImg.load("://image/new/thin_wind_still_up.png");
    filterPumpOpenedBgImg.load("://image/new/filter_pump_opened.png");
    filterPumpClosedBgImg.load("://image/new/filter_pump_closed.png");
    filterPumpFaultBgImg.load("://image/new/filter_pump_fault.png");
    waterLevelLowBgImg.load("://image/new/thin_filter_bottom_low.png");
    waterLevelNormalBgImg.load("://image/new/thin_filter_bottom_normal.png");

    ui->frame_12->setVisible(false);
    ui->frame_4->setVisible(false);
    ui->frame_control_mode->setVisible(false);

    mixAirValve = new TankAirValve(this);
    mixAirValve->setDeviceIndex(0);
    mixAirValve->setTextIndexValue("-1");
    mixAirValve->setName(QStringLiteral("混料区管道阀"));
    ui->verticalLayout_mv->addWidget(mixAirValve);

    tankAirValve1 = new TankAirValve(this);
    tankAirValve2 = new TankAirValve(this);
    tankAirValve3 = new TankAirValve(this);
    tankAirValve4 = new TankAirValve(this);
    tankAirValve5 = new TankAirValve(this);
    tankAirValve6 = new TankAirValve(this);
    tankAirValve7 = new TankAirValve(this);
    tankAirValve8 = new TankAirValve(this);
    tankAirValve9 = new TankAirValve(this);
    tankAirValve10 = new TankAirValve(this);
    tankAirValve11 = new TankAirValve(this);
    tankAirValve12 = new TankAirValve(this);

    ui->verticalLayout_1->addWidget(tankAirValve1);
    ui->verticalLayout_2->addWidget(tankAirValve2);
    ui->verticalLayout_3->addWidget(tankAirValve3);
    ui->verticalLayout_4->addWidget(tankAirValve4);
    ui->verticalLayout_5->addWidget(tankAirValve5);
    ui->verticalLayout_6->addWidget(tankAirValve6);
    ui->verticalLayout_7->addWidget(tankAirValve7);
    ui->verticalLayout_8->addWidget(tankAirValve8);
    ui->verticalLayout_9->addWidget(tankAirValve9);
    ui->verticalLayout_10->addWidget(tankAirValve10);
    ui->verticalLayout_11->addWidget(tankAirValve11);
    ui->verticalLayout_12->addWidget(tankAirValve12);

    allGdValveParts.append(tankAirValve1);
    gdValveNumber++;
    allGdValveParts.append(tankAirValve2);
    gdValveNumber++;
    allGdValveParts.append(tankAirValve3);
    gdValveNumber++;
    allGdValveParts.append(tankAirValve4);
    gdValveNumber++;
    allGdValveParts.append(tankAirValve5);
    gdValveNumber++;
    allGdValveParts.append(tankAirValve6);
    gdValveNumber++;
    allGdValveParts.append(tankAirValve7);
    gdValveNumber++;
    allGdValveParts.append(tankAirValve8);
    gdValveNumber++;
    allGdValveParts.append(tankAirValve9);
    gdValveNumber++;
    allGdValveParts.append(tankAirValve10);
    gdValveNumber++;
    allGdValveParts.append(tankAirValve11);
    gdValveNumber++;
    allGdValveParts.append(tankAirValve12);
    gdValveNumber++;

    for (int i=0; i<gdValveNumber; i++)
    {
        allGdValveParts[i]->setPrefix("ValveGD");
        allGdValveParts[i]->setDeviceIndex(0);
        allGdValveParts[i]->setTextIndexValue(QString::number(i+1));
        allGdValveParts[i]->setName(QStringLiteral("管道阀"));
    }

    fjValve1 = new TankAirValve(this);
    fjValve1->setEnable(false);
    ui->fv_frame_1_layout->addWidget(fjValve1);
    allFjValveParts.insert(fjValveNumber++, fjValve1);
    fjValve2 = new TankAirValve(this);
    fjValve2->setEnable(false);
    ui->fv_frame_2_layout->addWidget(fjValve2);
    allFjValveParts.insert(fjValveNumber++, fjValve2);
    fjValve3 = new TankAirValve(this);
    fjValve3->setEnable(false);
    ui->fv_frame_3_layout->addWidget(fjValve3);
    allFjValveParts.insert(fjValveNumber++, fjValve3);
    fjValve4 = new TankAirValve(this);
    fjValve4->setEnable(false);
    ui->fv_frame_4_layout->addWidget(fjValve4);
    allFjValveParts.insert(fjValveNumber++, fjValve4);

    for (int i=0; i<fjValveNumber; i++)
    {
        allFjValveParts[i]->setPrefix("ValveFJ");
        allFjValveParts[i]->setDeviceIndex(0);
        allFjValveParts[i]->setTextIndexValue(QString::number(i+1));
        allFjValveParts[i]->setName(QStringLiteral("风机阀"));
    }


    fan1 = new SingleDeodorationFan(this);
    fan2 = new SingleDeodorationFan(this);
    ui->fan_frame_1_layout->addWidget(fan1);
    ui->fan_frame_2_layout->addWidget(fan2);
    allFans.insert(fanNumber++, fan1);
    allFans.insert(fanNumber++, fan2);

    for (int i=0; i<fanNumber; i++)
    {
        allFans[i]->setDeviceIndex(0);
        allFans[i]->setTextIndexValue(QString::number(i+1));
        allFans[i]->setName(QStringLiteral("引风机"));
    }

    for(int i=0; i < Global::deoDeviceGroupInfos.length();i++)
    {
        QString itemName = QString::number(i + 1) + QStringLiteral("号生物除臭滤池");
        ui->deviceIndexComboBox->addItem(itemName);
    }

    deoSettingDialog = new DeoSettingDialog(this);
    deoManualSettingDlg = new DeoManualSettingDialog(this);
    connect(this,SIGNAL(dataUpdate(QSet<int>,QMap<float,QString>)),deoManualSettingDlg,SLOT(updateFermentationData(QSet<int>, QMap<float,QString>)));

    blowTimer = new QTimer(this);
    connect(blowTimer, SIGNAL(timeout()), this, SLOT(switchBlowingImg()));

    myTimerThread = new MyTimerThread(1, this);
    connect(myTimerThread, SIGNAL(timeout()), this, SLOT(updateDeviceState()));

    getServerConnectStateTcpClient = new TcpClientSocket(this);
    connect(getServerConnectStateTcpClient, SIGNAL(updateConnectState(bool)), this, SLOT(localServerConnected(bool)));
    getAllDataTcpclient = new TcpClientSocket(this);
    connect(getAllDataTcpclient, SIGNAL(updateClients(QByteArray)),this,SLOT(showAllData(QByteArray)));
    actionTcpclient = new TcpClientSocket(this);    
}

DeoControlTabPageWidget::~DeoControlTabPageWidget()
{
    if(workerThread.isRunning())
    {
        workerThread.quit();
        workerThread.wait();
    }

    if(myTimerThread->isRunning())
    {
        myTimerThread->quit();
        myTimerThread->wait();
    }

    workerThread.deleteLater();

    myTimerThread->deleteLater();

    delete ui;
}

void DeoControlTabPageWidget::switchBlowingImg()
{
    if(blowing)
    {
        if(tankAirValve1->getDeviceOpenedState())
        {
            ui->airValveLabel_1->setPixmap(greenArrowImg);
        }
        else
        {
            ui->airValveLabel_1->setPixmap(brownArrowImg);
        }

        if(tankAirValve2->getDeviceOpenedState())
        {
            ui->airValveLabel_2->setPixmap(greenArrowImg);
        }
        else
        {
            ui->airValveLabel_2->setPixmap(brownArrowImg);
        }

        if(tankAirValve3->getDeviceOpenedState())
        {
            ui->airValveLabel_3->setPixmap(greenArrowImg);
        }
        else
        {
            ui->airValveLabel_3->setPixmap(brownArrowImg);
        }

        if(tankAirValve4->getDeviceOpenedState())
        {
            ui->airValveLabel_4->setPixmap(greenArrowImg);
        }
        else
        {
            ui->airValveLabel_4->setPixmap(brownArrowImg);
        }

        if(tankAirValve5->getDeviceOpenedState())
        {
            ui->airValveLabel_5->setPixmap(greenArrowImg);
        }
        else
        {
            ui->airValveLabel_5->setPixmap(brownArrowImg);
        }

        if(tankAirValve6->getDeviceOpenedState())
        {
            ui->airValveLabel_6->setPixmap(greenArrowImg);
        }
        else
        {
            ui->airValveLabel_6->setPixmap(brownArrowImg);
        }

        if(tankAirValve7->getDeviceOpenedState())
        {
            ui->airValveLabel_7->setPixmap(greenArrowImg);
        }
        else
        {
            ui->airValveLabel_7->setPixmap(brownArrowImg);
        }

        if(tankAirValve8->getDeviceOpenedState())
        {
            ui->airValveLabel_8->setPixmap(greenArrowImg);
        }
        else
        {
            ui->airValveLabel_8->setPixmap(brownArrowImg);
        }

        if(tankAirValve9->getDeviceOpenedState())
        {
            ui->airValveLabel_9->setPixmap(greenArrowImg);
        }
        else
        {
            ui->airValveLabel_9->setPixmap(brownArrowImg);
        }

        if(tankAirValve10->getDeviceOpenedState())
        {
            ui->airValveLabel_10->setPixmap(greenArrowImg);
        }
        else
        {
            ui->airValveLabel_10->setPixmap(brownArrowImg);
        }

        if(tankAirValve11->getDeviceOpenedState())
        {
            ui->airValveLabel_11->setPixmap(greenArrowImg);
        }
        else
        {
            ui->airValveLabel_11->setPixmap(brownArrowImg);
        }

        if(tankAirValve12->getDeviceOpenedState())
        {
            ui->airValveLabel_12->setPixmap(greenArrowImg);
        }
        else
        {
            ui->airValveLabel_12->setPixmap(brownArrowImg);
        }

        if(mixAirValve->getDeviceOpenedState())
        {
            ui->airValveLabel_mix->setPixmap(greenArrowImg);
        }
        else
        {
            ui->airValveLabel_mix->setPixmap(brownArrowImg);
        }
    }
    else
    {
        if(tankAirValve1->getDeviceOpenedState())
        {
            ui->airValveLabel_1->setPixmap(blankImg);
        }
        else
        {
            ui->airValveLabel_1->setPixmap(brownArrowImg);
        }

        if(tankAirValve2->getDeviceOpenedState())
        {
            ui->airValveLabel_2->setPixmap(blankImg);
        }
        else
        {
            ui->airValveLabel_2->setPixmap(brownArrowImg);
        }

        if(tankAirValve3->getDeviceOpenedState())
        {
            ui->airValveLabel_3->setPixmap(blankImg);
        }
        else
        {
            ui->airValveLabel_3->setPixmap(brownArrowImg);
        }

        if(tankAirValve4->getDeviceOpenedState())
        {
            ui->airValveLabel_4->setPixmap(blankImg);
        }
        else
        {
            ui->airValveLabel_4->setPixmap(brownArrowImg);
        }

        if(tankAirValve5->getDeviceOpenedState())
        {
            ui->airValveLabel_5->setPixmap(blankImg);
        }
        else
        {
            ui->airValveLabel_5->setPixmap(brownArrowImg);
        }

        if(tankAirValve6->getDeviceOpenedState())
        {
            ui->airValveLabel_6->setPixmap(blankImg);
        }
        else
        {
            ui->airValveLabel_6->setPixmap(brownArrowImg);
        }

        if(tankAirValve7->getDeviceOpenedState())
        {
            ui->airValveLabel_7->setPixmap(blankImg);
        }
        else
        {
            ui->airValveLabel_7->setPixmap(brownArrowImg);
        }

        if(tankAirValve8->getDeviceOpenedState())
        {
            ui->airValveLabel_8->setPixmap(blankImg);
        }
        else
        {
            ui->airValveLabel_8->setPixmap(brownArrowImg);
        }

        if(tankAirValve9->getDeviceOpenedState())
        {
            ui->airValveLabel_9->setPixmap(blankImg);
        }
        else
        {
            ui->airValveLabel_9->setPixmap(brownArrowImg);
        }

        if(tankAirValve10->getDeviceOpenedState())
        {
            ui->airValveLabel_10->setPixmap(blankImg);
        }
        else
        {
            ui->airValveLabel_10->setPixmap(brownArrowImg);
        }

        if(tankAirValve11->getDeviceOpenedState())
        {
            ui->airValveLabel_11->setPixmap(blankImg);
        }
        else
        {
            ui->airValveLabel_11->setPixmap(brownArrowImg);
        }

        if(tankAirValve12->getDeviceOpenedState())
        {
            ui->airValveLabel_12->setPixmap(blankImg);
        }
        else
        {
            ui->airValveLabel_12->setPixmap(brownArrowImg);
        }

        if(mixAirValve->getDeviceOpenedState())
        {
            ui->airValveLabel_mix->setPixmap(blankImg);
        }
        else
        {
            ui->airValveLabel_mix->setPixmap(brownArrowImg);
        }
    }

    blowing = !blowing;
}

void DeoControlTabPageWidget::showEvent(QShowEvent *)
{
    if(Global::ferGroupShow == 1)
    {
        ui->env_data_frame->setVisible(false);
        ui->mix_label_1->setVisible(false);
        ui->mix_label_2->setVisible(false);
        ui->mix_label_3->setVisible(false);
        ui->mix_valve_frame->setVisible(false);
        ui->airValveLabel_mix->setVisible(false);
    }
    else
    {
        ui->env_data_frame->setVisible(true);
        ui->mix_label_1->setVisible(true);
        ui->mix_label_2->setVisible(true);
        ui->mix_label_3->setVisible(true);
        ui->mix_valve_frame->setVisible(true);
        ui->airValveLabel_mix->setVisible(true);
    }

    if(!workerThread.isRunning())
    {
        workerThread.start();
    }

    if(!myTimerThread->isRunning())
    {
        myTimerThread->start();
    }

    blowTimer->start(1000);
    //testRunCode();
}

void DeoControlTabPageWidget::closeEvent(QCloseEvent *)
{
    ui->deviceIndexComboBox->clear();
    blowTimer->stop();
}

void DeoControlTabPageWidget::testRunCode()
{
    QMap<float,QString> plcDataMap;

    int deviceIndex = ui->deviceIndexComboBox->currentIndex();
    QFile dataFile("deodatafile" + QString::number(deviceIndex) + ".txt");
    if(!dataFile.open(QIODevice::ReadOnly))
    {

    }
    QTextStream in(&dataFile);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList fields = line.split(' ');
        float address = fields.takeFirst().toFloat();
        QString value = fields.takeFirst();
        plcDataMap.insert(address, value);
    }

    /*int deviceIndex = ui->deviceIndexComboBox->currentIndex();
    QFile dataFile("deodatafile" + QString::number(deviceIndex) + ".txt");
    if(!dataFile.open(QIODevice::WriteOnly))
    {

    }

    QTextStream out(&dataFile);
    for(int row = 0; row < addressArray.length(); row++)
    {
        out << addressArray[row] << " " << strArray[row] << endl;
    }*/

    parseDeodorationData(plcDataMap);
    parseRunCtrData(plcDataMap);
}

void DeoControlTabPageWidget::dispatchDeoData(QSet<int> changedDeviceIndex, QMap<float, QString> dataMap)
{
    uint startAddrss = Global::deoDeviceInfo.Runctr_Address;
    uint valueNumber = Global::deoDeviceInfo.Runctr_Num;
    QVector<bool> boolValues;
    for(uint i=0; i < valueNumber; i++)
    {
        uint step = i / 8;
        uint temp = i % 8;
        float index = float(temp) / 10;
        float dictAddress = index + startAddrss + step;
        QVariant tempValue = dataMap[dictAddress];
        boolValues.append(tempValue.toBool());

        if(!Global::currentDeodorationDataMap.contains(dictAddress))
        {
            Global::currentDeodorationDataMap.insert(dictAddress,dataMap[dictAddress]);
        }
        else
        {
            if(Global::currentDeodorationDataMap[dictAddress] != dataMap[dictAddress])
            {
                uint deviceIndex = i / Global::deoDeviceInfo.RunCtr_Block_Size;
                DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);

                QList<QStandardItem *> newItemList;
                QList<QStandardItem *> newSimpleItemList;
                Global::alertIndex += 1;
                QString simpleAlert;

                newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                newItemList.append(new QStandardItem(QString::number((deviceIndex + info.startIndex)+1)));
                if(tempValue.toBool())
                {
                    newItemList.append(new QStandardItem(Global::deoRunCtrDeviceNodes[i % Global::deoDeviceInfo.RunCtr_Block_Size].Alert1));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            Global::deoRunCtrDeviceNodes[i % Global::deoDeviceInfo.RunCtr_Block_Size].Alert1 + " " +
                            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                }
                else
                {
                    newItemList.append(new QStandardItem(Global::deoRunCtrDeviceNodes[i % Global::deoDeviceInfo.RunCtr_Block_Size].Alert0));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            Global::deoRunCtrDeviceNodes[i % Global::deoDeviceInfo.RunCtr_Block_Size].Alert0 + " " +
                            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                }
                QStandardItem *simpleAlertItem = new QStandardItem(simpleAlert);
                newSimpleItemList.append(simpleAlertItem);

                if(Identity::getInstance()->getUser() != Q_NULLPTR)
                {
                    newItemList.append(new QStandardItem(Identity::getInstance()->getUser()->getUsername()));
                }
                else
                {
                    newItemList.append(new QStandardItem(""));
                }

                UiGlobal::simpleAlertsModel->insertRow(0, newSimpleItemList);
                UiGlobal::alertsModel->appendRow(newItemList);
                Global::currentDeodorationDataMap[dictAddress] = dataMap[dictAddress];
            }
        }
    }

    parseDeodorationData(dataMap);
    parseRunCtrData(dataMap);

    emit dataUpdate(changedDeviceIndex, dataMap);
}

void DeoControlTabPageWidget::showAllData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode == 1)
    {
        emit serverDataReceived(data);
    }
}

void DeoControlTabPageWidget::parseDeodorationData(QMap<float,QString> dataMap)
{
    int deviceIndex = ui->deviceIndexComboBox->currentIndex();
    DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getDeodorationNodeInfoByName("FER_H2S");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->h2sLabel->setText(dataMap.value(address));
    deviceNode = Global::getDeodorationNodeInfoByName("FER_NH3");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->nh3Label->setText(dataMap.value(address));
    deviceNode = Global::getDeodorationNodeInfoByName("FER_TEMP");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->tempLabel->setText(dataMap.value(address));

    deviceNode = Global::getDeodorationNodeInfoByName("Temp_Read");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->waterTemptureLabel->setText(dataMap.value(address));
    deviceNode = Global::getDeodorationNodeInfoByName("PH_Read");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->waterPhLabel->setText(dataMap.value(address));
    deviceNode = Global::getDeodorationNodeInfoByName("Level_Switch_Read");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->waterLevelValueLabel->setText(dataMap.value(address));

    deviceNode = Global::getDeodorationNodeInfoByName("Total_Current");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->aivLabel->setText(dataMap.value(address));
}

void DeoControlTabPageWidget::parseRunCtrData(QMap<float,QString> dataMap)
{
    mixAirValve->setDeviceOpenedState(Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Mix_ValveGD_1_Opened_Signal",dataMap));
    mixAirValve->setDeviceClosedState(Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Mix_ValveGD_1_Closed_Signal",dataMap));
    mixAirValve->setDeviceFaultState(Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Mix_ValveGD_1_fault_Signal",dataMap));

    foreach (BasePartWidget * part, allGdValveParts) {
        part->setDeviceOpenedState(Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),part->getOpendSignalName(),dataMap));
        part->setDeviceClosedState(Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),part->getClosedSignalName(),dataMap));
        part->setDeviceFaultState(Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),part->getFaultSignalName(),dataMap));
    }

    foreach (BasePartWidget *part, allFjValveParts) {
        part->setDeviceOpenedState(Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),part->getOpendSignalName(),dataMap));
        part->setDeviceClosedState(Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),part->getClosedSignalName(),dataMap));
        part->setDeviceFaultState(Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),part->getFaultSignalName(),dataMap));
    }

    foreach (SingleDeodorationFan *fan, allFans) {
        fan->setFanRunState(Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),fan->getRunSignalName(),dataMap));
        fan->setFanReadyState(Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),fan->getReadySignalName(),dataMap));
        fan->setFanFaultState(Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),fan->getFaultSignalName(),dataMap));
    }

    if(!fan1->getFanRunState())
    {
        ui->startFanOneButton->setEnabled(true);
    }
    else
    {
        ui->startFanOneButton->setEnabled(false);
    }

    if(fan1->getFanRunState())
    {
        ui->stopFanOneButton->setEnabled(true);
    }
    else
    {
        ui->stopFanOneButton->setEnabled(false);
    }

    if(fan1->getFanFaultState())
    {
        ui->fan_one_state_label->setText(QStringLiteral("故障"));
        ui->fan_one_state_label->setStyleSheet("background-color: rgb(255, 255, 0)");
        ui->startFanOneButton->setEnabled(false);
        ui->stopFanOneButton->setEnabled(false);
    }

    if(!fan2->getFanRunState())
    {
        ui->startFanTwoButton->setEnabled(true);
    }
    else
    {
        ui->startFanTwoButton->setEnabled(false);
    }

    if(fan2->getFanRunState())
    {
        ui->stopFanTwoButton->setEnabled(true);
    }
    else
    {
        ui->stopFanTwoButton->setEnabled(false);
    }

    if(fan2->getFanFaultState())
    {
        ui->fan_two_state_label->setText(QStringLiteral("故障"));
        ui->fan_two_state_label->setStyleSheet("background-color: rgb(255, 255, 0)");
        ui->startFanTwoButton->setEnabled(false);
        ui->stopFanTwoButton->setEnabled(false);
    }

    /*bool checked = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"TimeControl_Chosed",dataMap);
    if(checked)
    {
        ui->timeControlRadioButton->setChecked(true);
    }
    checked = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"AerationControl_Chosed",dataMap);
    if(checked)
    {
        ui->aerationControlRadioButton->setChecked(true);
    }
    checked = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"AmmoniaControl_Chosed",dataMap);
    if(checked)
    {
        ui->ammoControlRadioButton->setChecked(true);
    }*/

    bool isRan = false;
    bool isFault = false;
    bool isAuto = false;

    /*isRan = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_1_Run_Signal",dataMap);
    isFault = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_1_False_Signal",dataMap);
    if(isFault)
    {
        ui->firstWashPumpStateLabel->setText(QStringLiteral("故障"));        
        ui->washPumpLabel_1->setPixmap(filterPumpFaultBgImg);
        ui->firstWashPumpStateLabel->setStyleSheet("background-color: rgb(255, 255, 0)");
    }
    else
    {
        if(isRan)
        {
            ui->washPumpLabel_1->setPixmap(filterPumpOpenedBgImg);
            ui->firstWashPumpStateLabel->setText(QStringLiteral("运行中"));
            ui->firstWashPumpStateLabel->setStyleSheet("background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->washPumpLabel_1->setPixmap(filterPumpClosedBgImg);
            ui->firstWashPumpStateLabel->setText(QStringLiteral("停止"));
            ui->firstWashPumpStateLabel->setStyleSheet("background-color: rgb(0, 170, 255)");
        }
    }


    isRan = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_2_Run_Signal",dataMap);
    isFault = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_2_False_Signal",dataMap);
    isRan = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_2_Run_Signal",dataMap);
    isFault = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_2_False_Signal",dataMap);
    if(isFault)
    {
        ui->secondWashPumpStateLabel->setText(QStringLiteral("故障"));
        ui->washPumpLabel_2->setPixmap(filterPumpFaultBgImg);
        ui->secondWashPumpStateLabel->setStyleSheet("background-color: rgb(255, 255, 0)");
    }
    else
    {
        if(isRan)
        {
            ui->washPumpLabel_2->setPixmap(filterPumpOpenedBgImg);
            ui->secondWashPumpStateLabel->setText(QStringLiteral("运行中"));
            ui->secondWashPumpStateLabel->setStyleSheet("background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->washPumpLabel_2->setPixmap(filterPumpClosedBgImg);
            ui->secondWashPumpStateLabel->setText(QStringLiteral("停止"));
            ui->secondWashPumpStateLabel->setStyleSheet("background-color: rgb(0, 170, 255)");
        }
    }*/

    isRan = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_PL_1_Run_Signal",dataMap);
    isFault = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_PL_1_False_Signal",dataMap);
    isAuto = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_PL_1_Auto",dataMap);
    if(isFault)
    {
        ui->sparyPumpLabel_1->setPixmap(filterPumpFaultBgImg);
    }
    else
    {
        if(isRan)
        {
            ui->sparyPumpLabel_1->setPixmap(filterPumpOpenedBgImg);
        }
        else
        {
            ui->sparyPumpLabel_1->setPixmap(filterPumpClosedBgImg);
        }
    }
    if(isAuto)
    {
        ui->Pump_PL_1_Auto_label->setText("自动");
    }
    else
    {
        ui->Pump_PL_1_Auto_label->setText("手动");
    }

    isRan = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_PL_2_Run_Signal",dataMap);
    isFault = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_PL_2_False_Signal",dataMap);
    isAuto = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_PL_2_Auto",dataMap);
    if(isFault)
    {
        ui->sparyPumpLabel_2->setPixmap(filterPumpFaultBgImg);
    }
    else
    {
        if(isRan)
        {
            ui->sparyPumpLabel_2->setPixmap(filterPumpOpenedBgImg);
        }
        else
        {
            ui->sparyPumpLabel_2->setPixmap(filterPumpClosedBgImg);
        }
    }
    if(isAuto)
    {
        ui->Pump_PL_2_Auto_label->setText("自动");
    }
    else
    {
        ui->Pump_PL_2_Auto_label->setText("手动");
    }

    isRan = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_1_Run_Signal",dataMap);
    isFault = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_1_False_Signal",dataMap);
    isAuto = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_1_Auto",dataMap);
    ui->washPumpLabel_2->setObjectName("washPumpLabel_1");
    if(isFault)
    {
        ui->washPumpLabel_1->setStyleSheet("QLabel#washPumpLabel_1{border-image:url(:/image/new/vertical_filter_pump_fault.png)}");
    }
    else
    {
        if(isRan)
        {
            ui->washPumpLabel_1->setStyleSheet("QLabel#washPumpLabel_1{border-image:url(:/image/new/vertical_filter_pump_opened.png)}");
        }
        else
        {
            ui->washPumpLabel_1->setStyleSheet("QLabel#washPumpLabel_1{border-image:url(:/image/new/vertical_filter_pump_closed.png)}");
        }
    }
    if(isAuto)
    {
        ui->Pump_1_Auto_label->setText("自动");
    }
    else
    {
        ui->Pump_1_Auto_label->setText("手动");
    }

    isRan = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_2_Run_Signal",dataMap);
    isFault = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_2_False_Signal",dataMap);
    isAuto = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Pump_2_Auto",dataMap);
    ui->washPumpLabel_2->setObjectName("washPumpLabel_2");
    if(isFault)
    {
        ui->washPumpLabel_2->setStyleSheet("QLabel#washPumpLabel_2{border-image:url(:/image/new/vertical_filter_pump_fault.png)}");
    }
    else
    {
        if(isRan)
        {
            ui->washPumpLabel_2->setStyleSheet("QLabel#washPumpLabel_2{border-image:url(:/image/new/vertical_filter_pump_opened.png)}");
        }
        else
        {
            ui->washPumpLabel_2->setStyleSheet("QLabel#washPumpLabel_2{border-image:url(:/image/new/vertical_filter_pump_closed.png)}");
        }
    }
    if(isAuto)
    {
        ui->Pump_2_Auto_label->setText("自动");
    }
    else
    {
        ui->Pump_2_Auto_label->setText("手动");
    }

    isRan = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"ValveBW_1_Run_Signal",dataMap);
    isFault = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"ValveBW_1_False_Signal",dataMap);
    isAuto = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"ValveBW_1_Auto",dataMap);
    ui->ValveBW_1_label->setObjectName("ValveBW_1_label");
    if(isRan)
    {
        ui->ValveBW_1_label->setStyleSheet("QLabel#ValveBW_1_label{border-image:url(:/image/new/water_valve_opened.png)}");
    }
    else
    {
        ui->ValveBW_1_label->setStyleSheet("QLabel#ValveBW_1_label{border-image:url(:/image/new/water_valve_closed.png)}");
    }
    if(isFault)
    {
        ui->ValveBW_1_label->setStyleSheet("QLabel#ValveBW_1_label{border-image:url(:/image/new/water_valve_fault.png)}");
    }
    if(isAuto)
    {
        ui->ValveBW_1_Auto_label->setText("自动");
    }
    else
    {
        ui->ValveBW_1_Auto_label->setText("手动");
    }

    isAuto = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Fan_1_Auto",dataMap);
    if(isAuto)
    {
        ui->Fan_1_Auto_label->setText("自动");
    }
    else
    {
        ui->Fan_1_Auto_label->setText("手动");
    }

    isAuto = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Fan_2_Auto",dataMap);
    if(isAuto)
    {
        ui->Fan_2_Auto_label->setText("自动");
    }
    else
    {
        ui->Fan_2_Auto_label->setText("手动");
    }

    isRan = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Heater_1_Run_Signal",dataMap);
    isFault = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Heater_1_False_Signal",dataMap);
    if(isFault)
    {
        ui->heater_1_state_label->setText(QStringLiteral("故障"));
        ui->heater_1_state_label->setStyleSheet("background-color: rgb(255, 255, 0)");
    }
    else
    {
        if(isRan)
        {
            ui->heater_1_state_label->setText(QStringLiteral("运行中"));
            ui->heater_1_state_label->setStyleSheet("background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->heater_1_state_label->setText(QStringLiteral("停止"));
            ui->heater_1_state_label->setStyleSheet("background-color: rgb(0, 170, 255)");
        }
    }

    isRan = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Heater_2_Run_Signal",dataMap);
    isFault = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Heater_2_False_Signal",dataMap);
    if(isFault)
    {
        ui->heater_2_state_label->setText(QStringLiteral("故障"));
        ui->heater_2_state_label->setStyleSheet("background-color: rgb(255, 255, 0)");
    }
    else
    {
        if(isRan)
        {
            ui->heater_2_state_label->setText(QStringLiteral("运行中"));
            ui->heater_2_state_label->setStyleSheet("background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->heater_2_state_label->setText(QStringLiteral("停止"));
            ui->heater_2_state_label->setStyleSheet("background-color: rgb(0, 170, 255)");
        }
    }

    bool isLevelLow = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Level_Switch_Low_Signal",dataMap);
    if(isLevelLow)
    {
        ui->waterLevelStateLabel->setText(QStringLiteral("液位低"));
        ui->water_level_state_label->setPixmap(waterLevelLowBgImg);
        ui->waterLevelStateLabel->setStyleSheet("background-color: rgb(255, 255, 0)");
    }

    isLevelLow = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Level_Switch_Middle_Signal",dataMap);
    if(isLevelLow)
    {
        ui->waterLevelStateLabel->setText(QStringLiteral("液位中"));
        ui->water_level_state_label->setPixmap(waterLevelNormalBgImg);
        ui->waterLevelStateLabel->setStyleSheet("background-color: rgb(0, 170, 0)");
    }

    isLevelLow = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Level_Switch_High_Signal",dataMap);
    if(isLevelLow)
    {
        ui->waterLevelStateLabel->setText(QStringLiteral("液位高"));
        ui->water_level_state_label->setPixmap(waterLevelNormalBgImg);
        ui->waterLevelStateLabel->setStyleSheet("background-color: rgb(0, 170, 0)");
    }
    /*else
    {
        ui->water_level_state_label->setPixmap(waterLevelNormalBgImg);
        ui->waterLevelStateLabel->setText(QStringLiteral("液位正常"));
        ui->waterLevelStateLabel->setStyleSheet("background-color: rgb(0, 170, 0)");
    }*/

    /*bool isTimeControl = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"TimeControl_Chosed",dataMap);

    fan1Choosed = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Fan_1_Choose",dataMap);
    fan2Choosed = Global::getDeoRunctrValueByName(ui->deviceIndexComboBox->currentIndex(),"Fan_2_Choose",dataMap);

    if(fan1Choosed)
    {
        ui->choseFanOneButton->setText(QStringLiteral("选中"));
    }
    else
    {
        ui->choseFanOneButton->setText(QStringLiteral("未选中"));
    }

    if(fan2Choosed)
    {
        ui->choseFanTwoButton->setText(QStringLiteral("选中"));
    }
    else
    {
        ui->choseFanTwoButton->setText(QStringLiteral("未选中"));
    }*/
}

void DeoControlTabPageWidget::updateDeviceState()
{
    //qDebug() << "Read_tank_data ***************************************************************** : " << readNumber++;
    getServerConnectStateTcpClient->sendTestConnectRequest();
}

void DeoControlTabPageWidget::localServerConnected(bool isConnected)
{
    emit serverConnectionChanged(isConnected);

    if(isConnected)
    {
       int deviceIndex = ui->deviceIndexComboBox->currentIndex();
       if(deviceIndex >= 0)
       {
           //DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
           //bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,r_AllCacheData,String,0,0,0,0,0,0};
           //getAllDataTcpclient->sendRequest(bpack);

           StreamPack bpack;
           bpack = {sizeof(StreamPack),2,(quint16)Global::ferGroupShow,r_AllCacheData,String,0,0,0,0,0,0};
           getAllDataTcpclient->sendRequest(bpack);
       }
    }
}

void DeoControlTabPageWidget::on_settingButton_clicked()
{
    deoSettingDialog->show();
}

void DeoControlTabPageWidget::on_deviceIndexComboBox_currentIndexChanged(int index)
{
    for (int i=0; i<gdValveNumber; i++)
    {
        allGdValveParts[i]->setDeviceIndex(index);
    }

    for (int i=0; i<fjValveNumber; i++)
    {
        allFjValveParts[i]->setDeviceIndex(index);
    }

    for (int i=0; i<fanNumber; i++)
    {
        allFans[i]->setDeviceIndex(index);
    }
}

void DeoControlTabPageWidget::on_startFanOneButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getDeodorationNodeInfoByName(fan1->getStartCtrName()).Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName(fan1->getStartCtrName()).Offset % 8;

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = true;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void DeoControlTabPageWidget::on_stopFanOneButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getDeodorationNodeInfoByName(fan1->getStopCtrName()).Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName(fan1->getStopCtrName()).Offset % 8;

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = true;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}


void DeoControlTabPageWidget::on_startFanTwoButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getDeodorationNodeInfoByName(fan2->getStartCtrName()).Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName(fan2->getStartCtrName()).Offset % 8;

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = true;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void DeoControlTabPageWidget::on_stopFanTwoButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getDeodorationNodeInfoByName(fan2->getStopCtrName()).Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName(fan2->getStopCtrName()).Offset % 8;

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        bool data = true;
        QVariant var_data = QVariant(data);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void DeoControlTabPageWidget::on_timeControlRadioButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        if(ui->timeControlRadioButton->isChecked())
        {
            QDateTime currentdt = QDateTime::currentDateTime();
            StreamPack bpack;
            uint stime =currentdt.toTime_t();
            uint etime =currentdt.toTime_t();
            ushort offset = Global::getDeodorationNodeInfoByName("TimeControl_Chosed").Offset / 8;
            ushort index = Global::getDeodorationNodeInfoByName("TimeControl_Chosed").Offset % 8;

            int deviceIndex = ui->deviceIndexComboBox->currentIndex();
            DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
            ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
            ushort address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
            bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
            bpack.bStartTime =stime;
            bpack.bEndTime =etime;

            bool data = true;
            QVariant var_data = QVariant(data);

            //actionTcpclient->abort();
            actionTcpclient->sendRequestWithResult(bpack,var_data,1);

            offset = Global::getDeodorationNodeInfoByName("AerationControl_Chosed").Offset / 8;
            index = Global::getDeodorationNodeInfoByName("AerationControl_Chosed").Offset % 8;

            address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
            bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
            bpack.bStartTime =stime;
            bpack.bEndTime =etime;

            data = false;
            var_data = QVariant(data);

            //actionTcpclient->abort();
            actionTcpclient->sendRequestWithResult(bpack,var_data,1);

            offset = Global::getDeodorationNodeInfoByName("AmmoniaControl_Chosed").Offset / 8;
            index = Global::getDeodorationNodeInfoByName("AmmoniaControl_Chosed").Offset % 8;

            address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
            bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
            bpack.bStartTime =stime;
            bpack.bEndTime =etime;

            data = false;
            var_data = QVariant(data);

            //actionTcpclient->abort();
            actionTcpclient->sendRequestWithResult(bpack,var_data,1);

            controlMode = TIME_CONTROL;
        }
    }
    else
    {
        if(controlMode != TIME_CONTROL)
        {
            if(controlMode == AERATION_CONTROL)
            {
                ui->aerationControlRadioButton->setChecked(true);
            }
            else if(controlMode == AMMO_CONTROL)
            {
                ui->ammoControlRadioButton->setChecked(true);
            }
        }
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void DeoControlTabPageWidget::on_aerationControlRadioButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        if(ui->aerationControlRadioButton->isChecked())
        {
            QDateTime currentdt = QDateTime::currentDateTime();
            StreamPack bpack;
            uint stime =currentdt.toTime_t();
            uint etime =currentdt.toTime_t();
            ushort offset = Global::getDeodorationNodeInfoByName("AerationControl_Chosed").Offset / 8;
            ushort index = Global::getDeodorationNodeInfoByName("AerationControl_Chosed").Offset % 8;

            int deviceIndex = ui->deviceIndexComboBox->currentIndex();
            DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
            ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
            ushort address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
            bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
            bpack.bStartTime =stime;
            bpack.bEndTime =etime;

            bool data = true;
            QVariant var_data = QVariant(data);

            //actionTcpclient->abort();
            actionTcpclient->sendRequestWithResult(bpack,var_data,1);

            offset = Global::getDeodorationNodeInfoByName("TimeControl_Chosed").Offset / 8;
            index = Global::getDeodorationNodeInfoByName("TimeControl_Chosed").Offset % 8;

            address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
            bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
            bpack.bStartTime =stime;
            bpack.bEndTime =etime;

            data = false;
            var_data = QVariant(data);

            //actionTcpclient->abort();
            actionTcpclient->sendRequestWithResult(bpack,var_data,1);

            offset = Global::getDeodorationNodeInfoByName("AmmoniaControl_Chosed").Offset / 8;
            index = Global::getDeodorationNodeInfoByName("AmmoniaControl_Chosed").Offset % 8;

            address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
            bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
            bpack.bStartTime =stime;
            bpack.bEndTime =etime;

            data = false;
            var_data = QVariant(data);

            //actionTcpclient->abort();
            actionTcpclient->sendRequestWithResult(bpack,var_data,1);

            controlMode = AERATION_CONTROL;
        }
    }
    else
    {
        if(controlMode != AERATION_CONTROL)
        {
            if(controlMode == TIME_CONTROL)
            {
                ui->timeControlRadioButton->setChecked(true);
            }
            else if(controlMode == AMMO_CONTROL)
            {
                ui->ammoControlRadioButton->setChecked(true);
            }
        }
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void DeoControlTabPageWidget::on_ammoControlRadioButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        if(ui->ammoControlRadioButton->isChecked())
        {
            QDateTime currentdt = QDateTime::currentDateTime();
            StreamPack bpack;
            uint stime =currentdt.toTime_t();
            uint etime =currentdt.toTime_t();
            ushort offset = Global::getDeodorationNodeInfoByName("AmmoniaControl_Chosed").Offset / 8;
            ushort index = Global::getDeodorationNodeInfoByName("AmmoniaControl_Chosed").Offset % 8;

            int deviceIndex = ui->deviceIndexComboBox->currentIndex();
            DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
            ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
            ushort address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
            bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
            bpack.bStartTime =stime;
            bpack.bEndTime =etime;

            bool data = true;
            QVariant var_data = QVariant(data);

            //actionTcpclient->abort();
            actionTcpclient->sendRequestWithResult(bpack,var_data,1);

            offset = Global::getDeodorationNodeInfoByName("TimeControl_Chosed").Offset / 8;
            index = Global::getDeodorationNodeInfoByName("TimeControl_Chosed").Offset % 8;

            address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
            bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
            bpack.bStartTime =stime;
            bpack.bEndTime =etime;

            data = false;
            var_data = QVariant(data);

            //actionTcpclient->abort();
            actionTcpclient->sendRequestWithResult(bpack,var_data,1);

            offset = Global::getDeodorationNodeInfoByName("AerationControl_Chosed").Offset / 8;
            index = Global::getDeodorationNodeInfoByName("AerationControl_Chosed").Offset % 8;

            address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
            bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
            bpack.bStartTime =stime;
            bpack.bEndTime =etime;

            data = false;
            var_data = QVariant(data);

            //actionTcpclient->abort();
            actionTcpclient->sendRequestWithResult(bpack,var_data,1);

            controlMode = AMMO_CONTROL;
        }
    }
    else
    {
        if(controlMode != AMMO_CONTROL)
        {
            if(controlMode == TIME_CONTROL)
            {
                ui->timeControlRadioButton->setChecked(true);
            }
            else if(controlMode == AERATION_CONTROL)
            {
                ui->ammoControlRadioButton->setChecked(true);
            }
        }
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void DeoControlTabPageWidget::on_choseFanOneButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getDeodorationNodeInfoByName("Fan_1_Choose").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Fan_1_Choose").Offset % 8;

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        QVariant var_data = QVariant(!fan1Choosed);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void DeoControlTabPageWidget::on_choseFanTwoButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getDeodorationNodeInfoByName("Fan_2_Choose").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Fan_2_Choose").Offset % 8;

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + (deviceIndex - info.startIndex) * runctrlByteSize + offset;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;

        QVariant var_data = QVariant(!fan2Choosed);

        actionTcpclient->abort();
        actionTcpclient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void DeoControlTabPageWidget::on_manualRunButton_clicked()
{
    deoManualSettingDlg->show();
}
