#include "deocontroldialog.h"
#include "ui_deocontroldialog.h"
#include "global.h"
#include "identity.h"

DeoControlDialog::DeoControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeoControlDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("除臭控制"));

    brownArrowImg.load("://image/flow_brown.png");
    tankAirValve1 = new TankAirValve(this);
    tankAirValve2 = new TankAirValve(this);
    tankAirValve3 = new TankAirValve(this);
    tankAirValve4 = new TankAirValve(this);
    tankAirValve5 = new TankAirValve(this);
    tankAirValve6 = new TankAirValve(this);
    tankAirValve1->setTextIndexValue("1");
    tankAirValve2->setTextIndexValue("2");
    tankAirValve3->setTextIndexValue("3");
    tankAirValve4->setTextIndexValue("4");
    tankAirValve5->setTextIndexValue("5");
    tankAirValve6->setTextIndexValue("6");
    pipeValve1 = new PiPeValve(this);
    pipeValve2 = new PiPeValve(this);
    pipeValve1->setTextIndexValue("1");
    pipeValve2->setTextIndexValue("2");
    fan1 = new DeodorationFan(this);
    fan2 = new DeodorationFan(this);
    backupFan = new DeodorationFan(this);
    fan1->setFanIndex(1);
    fan2->setFanIndex(3);
    backupFan->setFanIndex(2);
    fan1->valveFront->setTextIndexValue("1-1");
    fan1->valveBack->setTextIndexValue("1-2");
    fan2->valveFront->setTextIndexValue("3-1");
    fan2->valveBack->setTextIndexValue("3-2");
    backupFan->valveFront->setTextIndexValue("2-1");
    backupFan->valveBack->setTextIndexValue("2-2");
    deoSettingDialog = new DeoSettingDialog(this);
    ui->verticalLayout_1->addWidget(tankAirValve1);
    ui->verticalLayout_2->addWidget(tankAirValve2);
    ui->verticalLayout_3->addWidget(tankAirValve3);
    ui->verticalLayout_4->addWidget(tankAirValve4);
    ui->verticalLayout_5->addWidget(tankAirValve5);
    ui->verticalLayout_6->addWidget(tankAirValve6);
    ui->pipe_valve_verticalLayout_1->addWidget(pipeValve1);
    ui->pipe_valve_verticalLayout_2->addWidget(pipeValve2);
    ui->fan_verticalLayout_1->addWidget(fan1);
    ui->fan_verticalLayout_2->addWidget(fan2);
    ui->fan_verticalLayout_bk->addWidget(backupFan);

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

DeoControlDialog::~DeoControlDialog()
{
    delete ui;
}

void DeoControlDialog::switchBlowingImg()
{
    if(blowing == 0)
    {
        ui->airValveLabel_1->setPixmap(blankImg);
        blowing = 1;
    }
    else
    {
        ui->airValveLabel_1->setPixmap(brownArrowImg);
        blowing = 0;
    }
}

void DeoControlDialog::showEvent(QShowEvent *)
{
    for(int i=0; i < Global::deoDeviceGroupInfos.length();i++)
    {
        QString itemName = QString::number(i + 1) + QStringLiteral("号生物除臭滤池");
        ui->deviceIndexComboBox->addItem(itemName);
    }

    myTimerThread->start();
    blowTimer->start(1000);

    //testRunCode();
}

void DeoControlDialog::closeEvent(QCloseEvent *)
{
    ui->deviceIndexComboBox->clear();
    blowTimer->stop();
}

void DeoControlDialog::testRunCode()
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

    /*QFile dataFile("deodatafile" + QString::number(deviceIndex) + ".txt");
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

void DeoControlDialog::showAllData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QString msg;
    if(bDevice.bErrorCode==1)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice), data.length() - sizeof(bDevice) - 4 * bDevice.bDataLength);
        QString strValues(byteValues);
        QStringList strValueList = strValues.split(",");
        QVector<QString> strArray = strValueList.toVector();

        byteValues = data.mid(data.length() - 4 * bDevice.bDataLength, 4 * bDevice.bDataLength);
        QVector<float> addressArray;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            float temp = 0;
            memcpy(&temp,value,4);
            addressArray.append(temp);
            msg.append(QString::number(temp));
            msg.append(",");
        }

        QMap<float,QString> plcDataMap;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            plcDataMap.insert(addressArray[i],strArray[i]);
        }

        parseDeodorationData(plcDataMap);
        parseRunCtrData(plcDataMap);
    }
}

void DeoControlDialog::parseDeodorationData(QMap<float,QString> dataMap)
{
    uint startAddrss = Global::deoDeviceInfo.Deodoration_Address;
    uint valueNumber = Global::deoDeviceInfo.Deodoration_Num;
    QVector<int> values;
    for(uint i=0; i < valueNumber; i++)
    {
        float address = float(startAddrss + 4 * i);
        values.append(dataMap.value(address).toFloat());

        if(!Global::currentDeodorationDataMap.contains(address))
        {
            Global::currentDeodorationDataMap.insert(address,dataMap.value(address));
        }
        else
        {
            Global::currentDeodorationDataMap[address] = dataMap.value(address);
        }
    }

    int deviceIndex = ui->deviceIndexComboBox->currentIndex();
    DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getDeodorationNodeInfoByName("Level_Switch_Read");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->waterLeverValueLabel->setText(dataMap.value(address));
    deviceNode = Global::getDeodorationNodeInfoByName("Temp_Read");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * 4;
    ui->waterTemptureLabel->setText(dataMap.value(address));
}

void DeoControlDialog::parseRunCtrData(QMap<float,QString> dataMap)
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
                QList<QStandardItem *> newItemList;
                newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                newItemList.append(new QStandardItem(QString::number(deviceIndex+1)));

                QList<QStandardItem *> newSimpleAlertItemList;
                Global::alertIndex += 1;
                QString simpleAlert;

                if(tempValue.toBool())
                {
                    newItemList.append(new QStandardItem(Global::deoRunCtrDeviceNodes[i % Global::deoDeviceInfo.RunCtr_Block_Size].Alert1));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            QString::number(deviceIndex+1) + "#" +
                            Global::deoRunCtrDeviceNodes[i % Global::deoDeviceInfo.RunCtr_Block_Size].Alert1 +
                            " " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                }
                else
                {
                    newItemList.append(new QStandardItem(Global::deoRunCtrDeviceNodes[i % Global::deoDeviceInfo.RunCtr_Block_Size].Alert0));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            QString::number(deviceIndex+1) + "#" +
                            Global::deoRunCtrDeviceNodes[i % Global::deoDeviceInfo.RunCtr_Block_Size].Alert1 +
                            " " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                }
                QStandardItem *simpleAlertItem = new QStandardItem(simpleAlert);
                newSimpleAlertItemList.append(simpleAlertItem);

                if(Identity::getInstance()->getUser() != Q_NULLPTR)
                {
                    newItemList.append(new QStandardItem(Identity::getInstance()->getUser()->getUsername()));
                }
                else
                {
                    newItemList.append(new QStandardItem(""));
                }

                UiGlobal::simpleAlertsModel->insertRow(0, newSimpleAlertItemList);
                UiGlobal::alertsModel->insertRow(0, newItemList);
                Global::currentFermenationDataMap[dictAddress] = dataMap[dictAddress];
            }
        }
    }

    int deviceIndex = ui->deviceIndexComboBox->currentIndex();
    DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
    int runctrBlockSize = (info.offset + deviceIndex - info.startIndex) * Global::ferDeviceInfo.RunCtr_Block_Size;

    pipeValve1->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveGD_1_Opened_Signal").Offset]);
    pipeValve1->setDeviceClosedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveGD_1_Closed_Signal").Offset]);
    pipeValve2->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveGD_2_Opened_Signal").Offset]);
    pipeValve2->setDeviceClosedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveGD_2_Closed_Signal").Offset]);

    tankAirValve1->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveFJ_1_Opened_Signal").Offset]);
    tankAirValve1->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveFJ_1_Closed_Signal").Offset]);
    tankAirValve2->setDeviceClosedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveFJ_2_Opened_Signal").Offset]);
    tankAirValve2->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveFJ_2_Closed_Signal").Offset]);
    tankAirValve3->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveFJ_3_Opened_Signal").Offset]);
    tankAirValve3->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveFJ_3_Closed_Signal").Offset]);
    tankAirValve4->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveFJ_4_Opened_Signal").Offset]);
    tankAirValve4->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveFJ_4_Closed_Signal").Offset]);
    tankAirValve5->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveFJ_5_Opened_Signal").Offset]);
    tankAirValve5->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveFJ_5_Closed_Signal").Offset]);
    tankAirValve6->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveFJ_6_Opened_Signal").Offset]);
    tankAirValve6->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("ValveFJ_6_Closed_Signal").Offset]);

    fan1->valveFront->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Valve_1-1_Opened_Signal").Offset]);
    fan1->valveFront->setDeviceClosedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Valve_1-1_Closed_Signal").Offset]);
    fan1->valveBack->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Valve_1-2_Opened_Signal").Offset]);
    fan1->valveBack->setDeviceClosedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Valve_1-2_Closed_Signal").Offset]);
    backupFan->valveFront->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Valve_2-1_Opened_Signal").Offset]);
    backupFan->valveFront->setDeviceClosedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Valve_2-1_Closed_Signal").Offset]);
    backupFan->valveBack->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Valve_2-2_Opened_Signal").Offset]);
    backupFan->valveBack->setDeviceClosedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Valve_2-2_Closed_Signal").Offset]);
    fan2->valveFront->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Valve_3-1_Opened_Signal").Offset]);
    fan2->valveFront->setDeviceClosedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Valve_3-1_Closed_Signal").Offset]);
    fan2->valveBack->setDeviceOpenedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Valve_3-2_Opened_Signal").Offset]);
    fan2->valveBack->setDeviceClosedState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Valve_3-2_Closed_Signal").Offset]);

    fan1->SetFanState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Fan_1_Run_Signal").Offset]);
    backupFan->SetFanState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Fan_2_Run_Signal").Offset]);
    fan2->SetFanState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Fan_3_Run_Signal").Offset]);
    fan1->setFaultState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Fan_1_False_Signal").Offset]);
    backupFan->setFaultState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Fan_2_False_Signal").Offset]);
    fan2->setFaultState(boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Fan_3_False_Signal").Offset]);

    bool isRan = false;
    bool isFault = false;
    isRan = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Pump_1_Run_Signal").Offset];
    isFault = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Pump_1_False_Signal").Offset];
    if(isFault)
    {
        ui->firstWashPumpStateLabel->setText(QStringLiteral("故障"));
        if(faultFlag)
        {
            ui->firstWashPumpStateLabel->setStyleSheet("background-color: rgb(255, 0, 0)");
        }
        else
        {
            ui->firstWashPumpStateLabel->setStyleSheet("background-color: rgb(255, 255, 255)");
        }
    }
    else
    {
        if(isRan)
        {
            ui->firstWashPumpStateLabel->setText(QStringLiteral("运行中"));
            ui->firstWashPumpStateLabel->setStyleSheet("background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->firstWashPumpStateLabel->setText(QStringLiteral("停止"));
            ui->firstWashPumpStateLabel->setStyleSheet("background-color: rgb(0, 170, 255)");
        }
    }

    isRan = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Pump_2_Run_Signal").Offset];
    isFault = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Pump_2_False_Signal").Offset];
    if(isFault)
    {
        ui->secondWashPumpStateLabel->setText(QStringLiteral("故障"));
        if(faultFlag)
        {
            ui->secondWashPumpStateLabel->setStyleSheet("background-color: rgb(255, 0, 0)");
        }
        else
        {
            ui->secondWashPumpStateLabel->setStyleSheet("background-color: rgb(255, 255, 255)");
        }
    }
    else
    {
        if(isRan)
        {
            ui->secondWashPumpStateLabel->setText(QStringLiteral("运行中"));
            ui->secondWashPumpStateLabel->setStyleSheet("background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->secondWashPumpStateLabel->setText(QStringLiteral("停止"));
            ui->secondWashPumpStateLabel->setStyleSheet("background-color: rgb(0, 170, 255)");
        }
    }

    isRan = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Pump_PL_Run_Signal").Offset];
    isFault = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Pump_PL_False_Signal").Offset];
    if(isFault)
    {
        ui->spary_pump_label->setText(QStringLiteral("故障"));
        if(faultFlag)
        {
            ui->spary_pump_label->setStyleSheet("background-color: rgb(255, 0, 0)");
        }
        else
        {
            ui->spary_pump_label->setStyleSheet("background-color: rgb(255, 255, 255)");
        }
        ui->startSparyPumpButton->setEnabled(false);
        ui->stopSparyPumpButton->setEnabled(false);
    }
    else
    {
        if(isRan)
        {
            ui->spary_pump_label->setText(QStringLiteral("运行中"));
            ui->spary_pump_label->setStyleSheet("background-color: rgb(0, 170, 0)");
            ui->startSparyPumpButton->setEnabled(false);
            ui->stopSparyPumpButton->setEnabled(true);
        }
        else
        {
            ui->spary_pump_label->setText(QStringLiteral("停止"));
            ui->spary_pump_label->setStyleSheet("background-color: rgb(0, 170, 255)");
            ui->startSparyPumpButton->setEnabled(true);
            ui->stopSparyPumpButton->setEnabled(false);
        }
    }

    isRan = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Heater_Run_Signal").Offset];
    isFault = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Heater_False_Signal").Offset];
    if(isFault)
    {
        ui->heater_state_label->setText(QStringLiteral("故障"));
        if(faultFlag)
        {
            ui->heater_state_label->setStyleSheet("background-color: rgb(255, 0, 0)");
        }
        else
        {
            ui->heater_state_label->setStyleSheet("background-color: rgb(255, 255, 255)");
        }
    }
    else
    {
        if(isRan)
        {
            ui->heater_state_label->setText(QStringLiteral("运行中"));
            ui->heater_state_label->setStyleSheet("background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->heater_state_label->setText(QStringLiteral("停止"));
            ui->heater_state_label->setStyleSheet("background-color: rgb(0, 170, 255)");
        }
    }

    bool isLevelLow = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Level_Switch_Low_Signal").Offset];
    if(isLevelLow)
    {
        ui->waterLevelStateLabel->setText(QStringLiteral("液位低"));
        if(faultFlag)
        {
            ui->waterLevelStateLabel->setStyleSheet("background-color: rgb(255, 0, 0)");
        }
        else
        {
            ui->waterLevelStateLabel->setStyleSheet("background-color: rgb(255, 255, 255)");
        }
    }
    else
    {
        ui->waterLevelStateLabel->setText(QStringLiteral("液位正常"));
        ui->waterLevelStateLabel->setStyleSheet("background-color: rgb(0, 170, 0)");
    }

    isRan = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("HMI_Fanline_1_Run").Offset];
    if(isRan)
    {
        ui->lineOneStateLabel->setText(QStringLiteral("运行中"));
        ui->lineOneStateLabel->setStyleSheet("background-color: rgb(0, 170, 0)");
        ui->startLineOneButton->setEnabled(false);
        ui->stopLineOneButton->setEnabled(true);
    }
    else
    {
        ui->lineOneStateLabel->setText(QStringLiteral("停止"));
        ui->lineOneStateLabel->setStyleSheet("background-color: rgb(0, 170, 255)");
        ui->startLineOneButton->setEnabled(true);
        ui->stopLineOneButton->setEnabled(false);
    }

    isRan = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("HMI_Fanline_2_Run").Offset];
    if(isRan)
    {
        ui->lineTwoStateLabel->setText(QStringLiteral("运行中"));
        ui->lineTwoStateLabel->setStyleSheet("background-color: rgb(0, 170, 0)");
        ui->startLineTwoButton->setEnabled(false);
        ui->stopLineTwoButton->setEnabled(true);
    }
    else
    {
        ui->lineTwoStateLabel->setText(QStringLiteral("停止"));
        ui->lineTwoStateLabel->setStyleSheet("background-color: rgb(0, 170, 255)");
        ui->startLineTwoButton->setEnabled(true);
        ui->stopLineTwoButton->setEnabled(false);
    }

    bool isTimeControl = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("TimeControl_Chosed").Offset];
    bool isRemoteContrl = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Remote_Chosed").Offset];
    if(isTimeControl & !isRemoteContrl)
    {
        ui->operationStateLabel->setText(QStringLiteral("时钟"));
        ui->operationStateLabel->setStyleSheet("background-color: rgb(0, 170, 0)");
    }
    else if(!isTimeControl & isRemoteContrl)
    {
        ui->operationStateLabel->setText(QStringLiteral("远程"));
        ui->operationStateLabel->setStyleSheet("background-color: rgb(0, 170, 255)");
    }
    else
    {
        ui->operationStateLabel->setText(QStringLiteral("切换中"));
    }

    isRan = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Backup_2_1_State").Offset];
    if(isRan)
    {
        ui->backupForFirstStateLabel->setText(QStringLiteral("已投入"));
        ui->backupForFirstStateLabel->setStyleSheet("background-color: rgb(0, 170, 0)");
    }
    else
    {
        ui->backupForFirstStateLabel->setText(QStringLiteral("未投入"));
        ui->backupForFirstStateLabel->setStyleSheet("background-color: rgb(0, 170, 255)");
    }

    isRan = boolValues[runctrBlockSize + Global::getDeodorationNodeInfoByName("Backup_2_3_State").Offset];
    if(isRan)
    {
        ui->backupForThirdStateLabel->setText(QStringLiteral("已投入"));
        ui->backupForThirdStateLabel->setStyleSheet("background-color: rgb(0, 170, 0)");
    }
    else
    {
        ui->backupForThirdStateLabel->setText(QStringLiteral("未投入"));
        ui->backupForThirdStateLabel->setStyleSheet("background-color: rgb(0, 170, 255)");
    }

    faultFlag = !faultFlag;
}

void DeoControlDialog::updateDeviceState()
{
    //qDebug() << "Read_tank_data ***************************************************************** : " << readNumber++;
    getServerConnectStateTcpClient->sendTestConnectRequest();
}

void DeoControlDialog::localServerConnected(bool isConnected)
{
    isServerConnected = isConnected;
    if(isConnected)
    {
       StreamPack bpack;
       int deviceIndex = ui->deviceIndexComboBox->currentIndex();
       if(deviceIndex >= 0)
       {
           DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
           bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,r_AllCacheData,String,0,0,0,0,0,0};
           getAllDataTcpclient->sendRequest(bpack);
       }
    }
}

void DeoControlDialog::on_pushButton_clicked()
{
    //ui->startLineOneButton->setEnabled(false);
}

void DeoControlDialog::on_switchOperationModeButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getDeodorationNodeInfoByName("Computer_TimeControl_Choose").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Computer_TimeControl_Choose").Offset % 8;

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + info.offset * runctrlByteSize + offset;
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

void DeoControlDialog::on_startLineOneButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getDeodorationNodeInfoByName("Computer_Fanline_1_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Computer_Fanline_1_Start_Signal").Offset % 8;

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + info.offset * runctrlByteSize + offset;
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

void DeoControlDialog::on_stopLineOneButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getDeodorationNodeInfoByName("Computer_Fanline_1_Stop_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Computer_Fanline_1_Stop_Signal").Offset % 8;

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + info.offset * runctrlByteSize + offset;
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

void DeoControlDialog::on_startLineTwoButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getDeodorationNodeInfoByName("Computer_Fanline_2_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Computer_Fanline_2_Start_Signal").Offset % 8;

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + info.offset * runctrlByteSize + offset;
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

void DeoControlDialog::on_stopLineTwoButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getDeodorationNodeInfoByName("Computer_Fanline_2_Stop_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Computer_Fanline_2_Stop_Signal").Offset % 8;

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + info.offset * runctrlByteSize + offset;
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

void DeoControlDialog::on_startSparyPumpButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getDeodorationNodeInfoByName("Computer_Pump_PL_Start_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Computer_Pump_PL_Start_Signal").Offset % 8;

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + info.offset * runctrlByteSize + offset;
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

void DeoControlDialog::on_stopSparyPumpButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getDeodorationNodeInfoByName("Computer_Pump_PL_Stop_Signal").Offset / 8;
        ushort index = Global::getDeodorationNodeInfoByName("Computer_Pump_PL_Stop_Signal").Offset % 8;

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);
        ushort runctrlByteSize = Global::deoDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::deoDeviceInfo.Runctr_Address + info.offset * runctrlByteSize + offset;
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

void DeoControlDialog::on_settingButton_clicked()
{
    deoSettingDialog->show();
}

void DeoControlDialog::on_deviceIndexComboBox_currentIndexChanged(int index)
{

}
