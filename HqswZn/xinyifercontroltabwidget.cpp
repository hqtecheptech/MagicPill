#include "xinyifercontroltabwidget.h"
#include "ui_xinyifercontroltabwidget.h"

#include "global.h"
#include "tankstatebarchart.h"
#include "loginhistoryform.h"
#include "alerthistoryform.h"
#include "identity.h"
#include "renhuainew.h"
#include "tankahead.h"

XinyiFerControlTabWidget::XinyiFerControlTabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::XinyiFerControlTabWidget)
{
    ui->setupUi(this);

    worker = new ParseServerDataWorker;
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &XinyiFerControlTabWidget::serverDataReceived, worker, &ParseServerDataWorker::parseFerServerData);
    connect(worker, &ParseServerDataWorker::resultReady, this, &XinyiFerControlTabWidget::dispatchFerData, Qt::QueuedConnection);

    fanOpenBgImg.load("://image/old/VF_G.bmp");
    fanCloseBgImg.load("://image/old/VF_R.bmp");

    initUI();

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
    //getAllFanDataTcpClient = new TcpClientSocket(this);
    //connect(getAllFanDataTcpClient, SIGNAL(updateClients(QByteArray)), this, SLOT(showFanData(QByteArray)));

    myTimerThread = new MyTimerThread(2, this);
    connect(myTimerThread, SIGNAL(timeout()),this,SLOT(read_server_data()));
}

XinyiFerControlTabWidget::~XinyiFerControlTabWidget()
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

void XinyiFerControlTabWidget::testRunCode()
{
    QMap<float,QString> plcDataMap;

    QFile dataFile("ferdatafile0.txt");
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

    QSet<int> changedDeviceSet;
    for(int i=0; i < 18; i++)
    {
        changedDeviceSet.insert(i);
    }

    dispatchFerData(changedDeviceSet, plcDataMap);
}

void XinyiFerControlTabWidget::initUI()
{
    tankLayoutWidgets.clear();
    tanks.clear();

    QFrame *allTankFrame = new QFrame;
    tankLayoutWidgets.append(allTankFrame);
    allTankLayout = new QVBoxLayout;
    allTankLayout->setSpacing(0);

    QSet<int> changeSet;
    /*int tankRows = 0;*/
    QHBoxLayout *tankRowLayout = new QHBoxLayout;
    tankRowLayout->setContentsMargins(50,0,0,0);
    tankRowLayout->setSpacing(50);
    int mid = 7;
    /*if((Global::ferDeviceInfo.Device_Number % 2) == 0)
    {
        mid = Global::ferDeviceInfo.Device_Number / 2;
    }
    else
    {
        mid = (Global::ferDeviceInfo.Device_Number + 1) / 2;
    }*/

    if(mid >= Global::ferDeviceInfo.Device_Number)
    {
        mid = Global::ferDeviceInfo.Device_Number;
    }

    //int taIndex = 0;
    //TankAhead* tkAhead = new TankAhead(this);
    //tkAhead->setMinimumSize(200, 269);
    //tkAhead->setIndex(taIndex+1);
    //tankRowLayout->addWidget(tkAhead);
    for(int i=0; i<mid; i++)
    {
        changeSet.insert(i);
        //BaseTankWidget *tk;
        /*if(Global::ferDeviceInfo.SharedFan)
        {
            tk = new UFFTank;
            tk->setMinimumSize(150, 327);
            connect(this, SIGNAL(dataUpdate(QSet<int>, QMap<float,QString>)),tk,SLOT(updateFermentationData(QSet<int>, QMap<float,QString>)));
        }
        else
        {*/
            FFTank *tk = new FFTank(this);
            tk->setMinimumSize(130, 269);
            connect(this, SIGNAL(dataUpdate(QSet<int>, QMap<float,QString>)),tk,SLOT(updateFermentationData(QSet<int>, QMap<float,QString>)));
        /*}*/


        tanks.append(tk);
        tankLayoutWidgets.append(tk);

        // For displaying with right size, widgets must set the minimum size before being added into scroll area.
        tk->setTankIndex(i);
        tankRowLayout->addWidget(tk);
        tankRowLayout->setAlignment(tk, Qt::AlignLeft);
    }
    tankRowLayout->addStretch();
    allTankLayout->addLayout(tankRowLayout);

    if(mid < Global::ferDeviceInfo.Device_Number)
    {
        QHBoxLayout *tankRowLayout1 = new QHBoxLayout;
        tankRowLayout1->setContentsMargins(50,0,0,0);
        //taIndex++;
        //TankAhead* tkAhead = new TankAhead(this);
        //tkAhead->setMinimumSize(200, 269);
        //tkAhead->setIndex(taIndex+1);
        //tankRowLayout1->addWidget(tkAhead);
        tankRowLayout1->setSpacing(50);
        for(int i=mid; i<Global::ferDeviceInfo.Device_Number; i++)
        {
            changeSet.insert(i);
            //BaseTankWidget *tk;
            /*if(Global::ferDeviceInfo.SharedFan)
            {
                tk = new UFFTank;
                tk->setMinimumSize(150, 327);
                connect(this, SIGNAL(dataUpdate(QSet<int>, QMap<float,QString>)),tk,SLOT(updateFermentationData(QSet<int>, QMap<float,QString>)));
            }
            else
            {*/
                FFTank *tk = new FFTank(this);
                tk->setMinimumSize(130, 269);
                connect(this, SIGNAL(dataUpdate(QSet<int>, QMap<float,QString>)),tk,SLOT(updateFermentationData(QSet<int>, QMap<float,QString>)));
            /*}*/


            tanks.append(tk);
            tankLayoutWidgets.append(tk);
            // For displaying with right size, widgets must set the minimum size before being added into scroll area.
            tk->setTankIndex(i);
            tankRowLayout1->addWidget(tk);
            tankRowLayout1->setAlignment(tk, Qt::AlignLeft);
        }
        tankRowLayout1->addStretch();
        allTankLayout->addLayout(tankRowLayout1);
    }

    allTankFrame->setLayout(allTankLayout);
    // Scroll Area must set widget;
    ui->tankScrollArea->setWidget(allTankFrame);

    if(Global::currentFermenationDataMap.count() > 0)
    {
        dispatchFerData(changeSet, Global::currentFermenationDataMap);
    }
}

void XinyiFerControlTabWidget::timerEvent(QTimerEvent *event)
{
    read_server_data();
}

void XinyiFerControlTabWidget::showEvent(QShowEvent *event)
{
    //testRunCode();
    if(!workerThread.isRunning())
    {
        workerThread.start();
    }
    if(!myTimerThread->isRunning())
    {
        myTimerThread->start();
    }
}

void XinyiFerControlTabWidget::closeEvent(QCloseEvent *event)
{

}

void XinyiFerControlTabWidget::showFerData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {        
        emit serverDataReceived(data);
    }
}

void XinyiFerControlTabWidget::localServerConnected(bool isConnected)
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

        //bpack = {sizeof(StreamPack),4,0,r_AllCacheData,String,0,0,0,0,0,0};
        //getAllFanDataTcpClient->sendRequest(bpack);
    }
}

void XinyiFerControlTabWidget::dispatchFerData(QSet<int> changedDeviceSet, QMap<float, QString> dataMap)
{
    emit dataUpdate(changedDeviceSet,dataMap);
    qDebug() << "Dispatch server data";

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
                DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);

                QList<QStandardItem *> newItemList;
                QList<QStandardItem *> newSimpleItemList;
                Global::alertIndex += 1;
                QString simpleAlert;

                newItemList.append(new QStandardItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
                newItemList.append(new QStandardItem(QString::number((tankIndex + info.startIndex)+1)));
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
            }
        }
    }
}

void XinyiFerControlTabWidget::read_server_data()
{
    //qDebug() << "Read_tank_data ***************************************************************** : " << readNumber++;
    getServerConnectStateTcpClient->sendTestConnectRequest();
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
