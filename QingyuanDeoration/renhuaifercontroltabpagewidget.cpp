#include "renhuaifercontroltabpagewidget.h"
#include "ui_renhuaifercontroltabpagewidget.h"

#include "global.h"
#include "tankstatebarchart.h"
#include "loginhistoryform.h"
#include "alerthistoryform.h"
#include "identity.h"
#include "renhuainew.h"

RenhuaiFerControlTabPageWidget::RenhuaiFerControlTabPageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenhuaiFerControlTabPageWidget)
{
    ui->setupUi(this);

    fanOpenBgImg.load("://image/old/VF_G.bmp");
    fanCloseBgImg.load("://image/old/VF_R.bmp");

    sfcDialog = new SharedFanControlDialog(this);

    for(int i=0; i<Global::ferDeviceInfo.Device_Number; i++)
    {
        UFFTank *tk = new UFFTank(this);
        tk->setTankIndex(i);
        tanks.append(tk);
    }

    fanButtons.append(ui->fanPushButton_1);
    fanButtons.append(ui->fanPushButton_2);
    fanButtons.append(ui->fanPushButton_3);
    fanButtons.append(ui->fanPushButton_4);

    ui->group_one_frame_horizontalLayout->addWidget(tanks[0]);
    ui->group_one_frame_horizontalLayout->addWidget(tanks[1]);
    ui->group_one_frame_horizontalLayout->addWidget(tanks[2]);

    ui->group_two_frame_horizontalLayout->addWidget(tanks[3]);
    ui->group_two_frame_horizontalLayout->addWidget(tanks[4]);
    ui->group_two_frame_horizontalLayout->addWidget(tanks[5]);

    ui->group_three_frame_horizontalLayout->addWidget(tanks[6]);
    ui->group_three_frame_horizontalLayout->addWidget(tanks[7]);

    ui->group_four_frame_horizontalLayout->addWidget(tanks[8]);
    ui->group_four_frame_horizontalLayout->addWidget(tanks[9]);

    TankStateBarChart *tankChart = new TankStateBarChart(this);
    ui->state_frame_horizontalLayout->addWidget(tankChart);

    loginHistoryForm *loginHistory = new loginHistoryForm(this);
    ui->loginHistoryFrame_horizontalLayout->addWidget(loginHistory);

    AlertHistoryForm *alertHistory = new AlertHistoryForm(this);
    ui->alert_history_horizontalLayout->addWidget(alertHistory);

    getServerConnectStateTcpClient = new TcpClientSocket(this);
    connect(getServerConnectStateTcpClient, SIGNAL(updateConnectState(bool)), this, SLOT(localServerConnected(bool)));
    getAllFerDataTcpClient = new TcpClientSocket(this);
    connect(getAllFerDataTcpClient, SIGNAL(updateClients(QByteArray)), this, SLOT(showFerData(QByteArray)));
    getAllFanDataTcpClient = new TcpClientSocket(this);
    connect(getAllFanDataTcpClient, SIGNAL(updateClients(QByteArray)), this, SLOT(showFanData(QByteArray)));

    myTimerThread = new MyTimerThread(1, this);
    connect(myTimerThread, SIGNAL(timeout()),this,SLOT(read_server_data()));
}

RenhuaiFerControlTabPageWidget::~RenhuaiFerControlTabPageWidget()
{
    delete ui;
}

void RenhuaiFerControlTabPageWidget::timerEvent(QTimerEvent *event)
{
    read_server_data();
}

void RenhuaiFerControlTabPageWidget::showEvent(QShowEvent *event)
{
    //testRunCode();
    myTimerThread->start();
}

void RenhuaiFerControlTabPageWidget::closeEvent(QCloseEvent *event)
{
    myTimerThread->terminate();
}

void RenhuaiFerControlTabPageWidget::showFerData(QByteArray data)
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

        for(uint i=0; i < tanks.length(); i++)
        {
            tanks[i]->setLocation(i);
        }

        emit dataUpdate(plcDataMap);

        parseFermentationData(plcDataMap);
        parseFerRunCtrData(plcDataMap);
        parseFerRunTimeData(plcDataMap);
        parseFerStepData(plcDataMap);
    }
}

void RenhuaiFerControlTabPageWidget::showFanData(QByteArray data)
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

        parseFanRunCtrData(plcDataMap);
    }
}

void RenhuaiFerControlTabPageWidget::parseFermentationData(QMap<float,QString> dataMap)
{
    uint startAddrss = Global::ferDeviceInfo.Fermentation_Address;
    uint valueNumber = Global::ferDeviceInfo.Fermentation_Num;
    QVector<float> realValues;
    for(uint i=0; i < valueNumber; i++)
    {
        float address = float(startAddrss + 4 * i);
        realValues.append(dataMap.value(address).toFloat());

        if(!Global::currentFermenationDataMap.contains(address))
        {
            Global::currentFermenationDataMap.insert(address,dataMap.value(address));
        }
        else
        {
            Global::currentFermenationDataMap[address] = dataMap.value(address);
        }
    }

    for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
    {
        tanks[j]->setupFermentationMapper(realValues, j);
    }
}

void RenhuaiFerControlTabPageWidget::parseFerRunTimeData(QMap<float,QString> dataMap)
{
    uint startAddrss = Global::ferDeviceInfo.Runtime_Address;
    uint valueNumber = Global::ferDeviceInfo.Runtime_Num;
    QVector<uint> uintValues;
    for(uint i=0; i < valueNumber; i++)
    {
        float address = float(startAddrss + 4 * i);
        uintValues.append(dataMap.value(address).toUInt());

        if(!Global::currentFermenationDataMap.contains(address))
        {
            Global::currentFermenationDataMap.insert(address,dataMap.value(address));
        }
        else
        {
            Global::currentFermenationDataMap[address] = dataMap.value(address);
        }
    }
    for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
    {
        tanks[j]->setupStepRunningTimeMapper(uintValues, j);
    }
}

void RenhuaiFerControlTabPageWidget::parseFerStepData(QMap<float,QString> dataMap)
{
    uint startAddrss = Global::ferDeviceInfo.Tank_State_Address;
    uint valueNumber = Global::ferDeviceInfo.Tank_State_Num;
    QVector<ushort> ushortValues;
    for(uint i=0; i < valueNumber; i++)
    {
        float address = float(startAddrss + 2 * i);
        ushortValues.append(dataMap.value(address).toUShort());

        if(!Global::currentFermenationDataMap.contains(address))
        {
            Global::currentFermenationDataMap.insert(address,dataMap.value(address));
        }
        else
        {
            Global::currentFermenationDataMap[address] = dataMap.value(address);
        }
    }

    for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
    {
        tanks[j]->setupStepMapper(ushortValues, j);
    }
}

void RenhuaiFerControlTabPageWidget::parseFerRunCtrData(QMap<float,QString> dataMap)
{
    uint startAddrss = Global::ferDeviceInfo.Runctr_Address;
    uint valueNumber = Global::ferDeviceInfo.Runctr_Num;
    QVector<bool> boolValues;
    for(uint i=0; i < valueNumber; i++)
    {
        uint step = i / 8;
        uint temp = i % 8;
        float index = float(temp) / 10;
        float dictAddress = index + startAddrss + step;
        QVariant tempValue = dataMap[dictAddress];
        boolValues.append(tempValue.toBool());

        if(!Global::currentFermenationDataMap.contains(dictAddress))
        {
            Global::currentFermenationDataMap.insert(dictAddress,dataMap[dictAddress]);
        }
        else
        {
            if(Global::currentFermenationDataMap[dictAddress] != dataMap[dictAddress])
            {
                uint tankIndex = i / Global::ferDeviceInfo.RunCtr_Block_Size;
                QList<QStandardItem *> newItemList;
                QList<QStandardItem *> newSimpleItemList;
                Global::alertIndex += 1;
                QString simpleAlert;

                newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                newItemList.append(new QStandardItem(QString::number(tankIndex+1)));
                if(tempValue.toBool())
                {
                    newItemList.append(new QStandardItem(Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert1));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            QString::number(tankIndex+1) + "#" +
                            Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert1 + " " +
                            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                }
                else
                {
                    newItemList.append(new QStandardItem(Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert0));
                    simpleAlert = QString::number(Global::alertIndex) + ": " +
                            QString::number(tankIndex+1) + "#" +
                            Global::ferRunCtrDeviceNodes[i % Global::ferDeviceInfo.RunCtr_Block_Size].Alert0 + " " +
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
                UiGlobal::alertsModel->insertRow(0, newItemList);
                Global::currentFermenationDataMap[dictAddress] = dataMap[dictAddress];

                emit(dataChanged());
            }
        }
    }

    for(int j=0; j<Global::ferDeviceInfo.Device_Number; j++)
    {
        tanks[j]->setupRunCtrMapper(boolValues, j);
    }
}

void RenhuaiFerControlTabPageWidget::parseFanRunCtrData(QMap<float,QString> dataMap)
{
    uint startAddrss = Global::fanDeviceInfo.Runctr_Address;
    uint valueNumber = Global::fanDeviceInfo.Runctr_Num;
    QVector<bool> boolValues;
    for(uint i=0; i < valueNumber; i++)
    {
        uint step = i / 8;
        uint temp = i % 8;
        float index = float(temp) / 10;
        float dictAddress = index + startAddrss + step;
        QVariant tempValue = dataMap[dictAddress];
        boolValues.append(tempValue.toBool());
    }

    for(int i=0; i < Global::fanDeviceInfo.Device_Number; i++)
    {
        DeviceGroupInfo info = Global::getFanDeviceGroupInfo(i);

        bool isMainFanRun = false;
        bool isBackupFanInUse = false;
        bool isBackupFanRun = false;
        bool isFault = false;
        int runctrBlockSize = (info.offset + i - info.startIndex)*Global::fanDeviceInfo.RunCtr_Block_Size;
        // The fifth fan is the backup fan, so offset + 4 is the backup fan.
        int backupFanRunctrBlockSize = (info.offset + 4 - info.startIndex)*Global::fanDeviceInfo.RunCtr_Block_Size;
        isMainFanRun = boolValues[runctrBlockSize + Global::getFanNodeInfoByName("FAN_Run_SIGNAL").Offset];
        isBackupFanInUse = boolValues[runctrBlockSize + Global::getFanNodeInfoByName("FAN_SPARE_INUSE").Offset];
        isBackupFanRun = boolValues[backupFanRunctrBlockSize + Global::getFanNodeInfoByName("FAN_Run_SIGNAL").Offset];
        isFault = boolValues[backupFanRunctrBlockSize + Global::getFanNodeInfoByName("FAN_Fault_SIGNAL").Offset];

        bool isRun = false;
        if(isBackupFanInUse)
        {
            isRun = isBackupFanRun;
        }
        else
        {
            isRun = isMainFanRun;
        }

        if(i==0)
        {
            ui->fanPushButton_1->setObjectName("pushbutton1");
            if(isRun)
            {
                ui->fanPushButton_1->setStyleSheet("QPushButton#pushbutton1{border-image:url(:/image/new/green_fan_button.png)}");
            }
            else
            {
                ui->fanPushButton_1->setStyleSheet("QPushButton#pushbutton1{border-image:url(:/image/new/red_fan_button.png)}");
            }
            if(isFault)
            {
                if(fanStateSwitchFlag)
                {
                    ui->fanPushButton_1->setStyleSheet("QPushButton#pushbutton1{border-image:url(:/image/new/yellow_fan_button.png)}");
                }
                else
                {
                    ui->fanPushButton_1->setStyleSheet("QPushButton#pushbutton1{border-image:url(:/image/new/green_fan_button.png)}");
                }
            }
        }
        if(i==1)
        {
            ui->fanPushButton_2->setObjectName("pushbutton2");
            if(isRun)
            {
                ui->fanPushButton_2->setStyleSheet("QPushButton#pushbutton2{border-image:url(:/image/new/green_fan_button.png)}");
            }
            else
            {
                ui->fanPushButton_2->setStyleSheet("QPushButton#pushbutton2{border-image:url(:/image/new/red_fan_button.png)}");
            }
            if(isFault)
            {
                if(fanStateSwitchFlag)
                {
                    ui->fanPushButton_2->setStyleSheet("QPushButton#pushbutton1{border-image:url(:/image/new/yellow_fan_button.png)}");
                }
                else
                {
                    ui->fanPushButton_2->setStyleSheet("QPushButton#pushbutton1{border-image:url(:/image/new/green_fan_button.png)}");
                }
            }
        }
        if(i==2)
        {
            ui->fanPushButton_3->setObjectName("pushbutton3");
            if(isRun)
            {
                ui->fanPushButton_3->setStyleSheet("QPushButton#pushbutton3{border-image:url(:/image/new/green_fan_button.png)}");
            }
            else
            {
                ui->fanPushButton_3->setStyleSheet("QPushButton#pushbutton3{border-image:url(:/image/new/red_fan_button.png)}");
            }
            if(isFault)
            {
                if(fanStateSwitchFlag)
                {
                    ui->fanPushButton_3->setStyleSheet("QPushButton#pushbutton1{border-image:url(:/image/new/yellow_fan_button.png)}");
                }
                else
                {
                    ui->fanPushButton_3->setStyleSheet("QPushButton#pushbutton1{border-image:url(:/image/new/green_fan_button.png)}");
                }
            }
        }
        if(i==3)
        {
            ui->fanPushButton_4->setObjectName("pushbutton4");
            if(isRun)
            {
                ui->fanPushButton_4->setStyleSheet("QPushButton#pushbutton4{border-image:url(:/image/new/green_fan_button.png)}");
            }
            else
            {
                ui->fanPushButton_4->setStyleSheet("QPushButton#pushbutton4{border-image:url(:/image/new/red_fan_button.png)}");
            }
            if(isFault)
            {
                if(fanStateSwitchFlag)
                {
                    ui->fanPushButton_4->setStyleSheet("QPushButton#pushbutton1{border-image:url(:/image/new/yellow_fan_button.png)}");
                }
                else
                {
                    ui->fanPushButton_4->setStyleSheet("QPushButton#pushbutton1{border-image:url(:/image/new/green_fan_button.png)}");
                }
            }
        }

        fanStateSwitchFlag = !fanStateSwitchFlag;
    }
}

void RenhuaiFerControlTabPageWidget::localServerConnected(bool isConnected)
{
    readStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if(isServerConnected != isConnected)
    {
        isServerConnected = isConnected;
        emit serverConnectionChanged(isConnected);
    }

    if(isConnected)
    {
        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,r_AllCacheData,String,0,0,0,0,0,0};
        getAllFerDataTcpClient->sendRequest(bpack);

        bpack = {sizeof(StreamPack),4,0,r_AllCacheData,String,0,0,0,0,0,0};
        getAllFanDataTcpClient->sendRequest(bpack);
    }
}

void RenhuaiFerControlTabPageWidget::read_server_data()
{
    //qDebug() << "Read_tank_data ***************************************************************** : " << readNumber++;
    getServerConnectStateTcpClient->sendTestConnectRequest();
}

QVector<UFFTank *> RenhuaiFerControlTabPageWidget::getTanks() const
{
    return tanks;
}

/*@brief 重载自定义对象的输入*/
inline QDataStream &operator<<(QDataStream &out,const StreamPack &per)
{
    out<<per.bStreamLength<<per.bDeviceId<<per.bCommandType;
    out<<per.bDataType<<per.bAddress<<per.bIndex<<per.bDataLength<<per.bErrorCode;
    out<<per.bStartTime<<per.bEndTime;
    return out;
}
/*@brief 重载自定义对象的输出 */
inline QDataStream &operator>>(QDataStream &in,StreamPack &per)
{
    in>>per.bStreamLength>>per.bDeviceId>>per.bCommandType;
    in>>per.bDataType>>per.bAddress>>per.bIndex>>per.bDataLength>>per.bErrorCode;
    in>>per.bStartTime>>per.bEndTime;
    return in;
}

void RenhuaiFerControlTabPageWidget::on_fanPushButton_1_clicked()
{
    sfcDialog->setFanIndex(0);
    sfcDialog->show();
}

void RenhuaiFerControlTabPageWidget::on_fanPushButton_2_clicked()
{
    sfcDialog->setFanIndex(1);
    sfcDialog->show();
}

void RenhuaiFerControlTabPageWidget::on_fanPushButton_3_clicked()
{
    sfcDialog->setFanIndex(2);
    sfcDialog->show();
}

void RenhuaiFerControlTabPageWidget::on_fanPushButton_4_clicked()
{
    sfcDialog->setFanIndex(3);
    sfcDialog->show();
}
