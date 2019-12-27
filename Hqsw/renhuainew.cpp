﻿#include "renhuainew.h"
#include "ui_renhuainew.h"
#include "identity.h"
#include "realtimedialog.h"
#include "global.h"

RenhuaiNew::RenhuaiNew(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RenhuaiNew)
{
    ui->setupUi(this);

    uint valueNumber = Global::ferDeviceInfo.Enviroment_Num;
    for(uint i=0; i < valueNumber; i++)
    {
        envParamsArray.append(new EnvironmentParams());
    }

    ui->uploadtime_label->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->serverIP_label->setText(Global::serverInfo.IP.split('.').at(3));
    ui->hostname_label->setText(QHostInfo::localHostName());

    ui->fanValveControlButton->setVisible(false);
    ui->HistoryQueryButton->setVisible(false);
    ui->realTimeChartButton->setVisible(false);
    ui->ReportButton->setVisible(false);

    ui->title_frame->setObjectName("titleframe");
    ui->title_frame->setStyleSheet("QFrame#titleframe{border-image:url(:/image/new/title_banner.jpg)}");

    ui->left_banner_frame->setObjectName("leftbannerframe");
    ui->left_banner_frame->setStyleSheet("QFrame#leftbannerframe{border-image:url(:/image/new/left_banner.bmp)}");

    tabWidget = new RenhuaiTabWidget(this);
    ui->main_content_frame_horizontalLayout->addWidget(tabWidget);

    ui->switchUserPushButton->setEnabled(false);

    connect(Identity::getInstance(),SIGNAL(logout()),this,SLOT(updateLoginState()));
    loginDialog = new LoginDialog(this);
    connect(loginDialog, SIGNAL(LogingStateChanged()),this,SLOT(updateLoginState()));
    connect(tabWidget,SIGNAL(dataChanged()),this,SLOT(updateDataChanged()));
    connect(tabWidget,SIGNAL(serverConnectionChanged(bool)),this,SLOT(updateServerConnectionChanged(bool)));
    connect(tabWidget,SIGNAL(ferDataUpdate(QSet<int>, QMap<float,QString>)),SLOT(updateFerData(QSet<int>, QMap<float,QString>)));
    connect(tabWidget,SIGNAL(deoDataUpdate(QSet<int>, QMap<float,QString>)),this,SLOT(updateDeoData(QSet<int>, QMap<float,QString>)));

    //fanValveControlDialog = new FanValveControlDialog(this);
    sfcontrolDialog = new SharedFanControlDialog(this);
    fanControlDialog = new FanControlDialog(this);
    connect(this,SIGNAL(ferDataChanged(QSet<int>, QMap<float,QString>)),fanControlDialog,SLOT(updateFermentationData(QSet<int>, QMap<float,QString>)));
    //sensorControlDialog = new SensorControlDialog(this);
    //connect(this,SIGNAL(ferDataChanged(QSet<int>, QMap<float,QString>)),sensorControlDialog,SLOT(updateFermentationData(QSet<int>, QMap<float,QString>)));

    historyDialog = new HistoryDialog(this);
    reportDialog = new ReportDialog(this);
    //settingDialog = new SettingDialog(this);
    ferconfigDialog = new FerConfigDialog(this);
    alertQueryDialog = new AlertQueryDialog(this);
    fercontrolDialog = new FerControlDialog(this);
    connect(this,SIGNAL(ferDataChanged(QSet<int>, QMap<float,QString>)),fercontrolDialog,SLOT(updateFermentationData(QSet<int>, QMap<float,QString>)));

    m_nTimerId = startTimer(1000);
}

RenhuaiNew::~RenhuaiNew()
{
    if ( m_nTimerId != 0 )
            killTimer(m_nTimerId);
    delete ui;
}

void RenhuaiNew::updateFerData(QSet<int> changedDeviceSet, QMap<float,QString> dataMap)
{
    emit ferDataChanged(changedDeviceSet, dataMap);

    for(int i=0; i<7; i++)
    {
        DeviceNode node = Global::getFermenationNodeInfoByName("FER_ET_R");
        uint address = node.Offset + i * 4;
        envParamsArray[i]->setTemperature(dataMap[address].toFloat());

        node = Global::getFermenationNodeInfoByName("FER_HM_R");
        address = node.Offset + i * 4;
        envParamsArray[i]->setHumidity(dataMap[address].toFloat());

        node = Global::getFermenationNodeInfoByName("ES_H2S_R");
        address = node.Offset + i * 4;
        envParamsArray[i]->setH2s(dataMap[address].toFloat());

        node = Global::getFermenationNodeInfoByName("ES_NH3_R");
        address = node.Offset + i * 4;
        envParamsArray[i]->setNh3(dataMap[address].toFloat());
    }

    QString strValue = "T:" + QString::number(envParamsArray.at(0)->getTemperature(), 'f', 2) + "  ";
            strValue.append("W:" + QString::number(envParamsArray.at(0)->getHumidity(), 'f', 2) + " %  ");
            strValue.append("NH3:" + QString::number(envParamsArray.at(0)->getNh3(), 'f', 2) + " ppm  ");
            strValue.append("H2S:" + QString::number(envParamsArray.at(0)->getH2s(), 'f', 2) + " ppm");
    ui->tab_banner_label->setText(strValue);
}

void RenhuaiNew::updateDeoData(QSet<int> changedDeviceSet, QMap<float,QString> dataMap)
{
    emit deoDataChanged(changedDeviceSet, dataMap);
}

void RenhuaiNew::updateServerConnectionChanged(bool isServerConnected)
{
    if(isServerConnected)
    {
        ui->systemstatus_label->setText(QStringLiteral("正常"));
    }
    else
    {
        ui->systemstatus_label->setText(QStringLiteral("中断"));
    }
}

void RenhuaiNew::updateDataChanged()
{
    ui->uploadtime_label->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void RenhuaiNew::timerEvent( QTimerEvent *event )
{
    ui->time_label->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    ui->date_label->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    int weekday = QDateTime::currentDateTime().date().dayOfWeek();
    switch (weekday) {
    case 1:
        ui->week_label->setText(QStringLiteral("星期一"));
        break;
    case 2:
        ui->week_label->setText(QStringLiteral("星期二"));
        break;
    case 3:
        ui->week_label->setText(QStringLiteral("星期三"));
        break;
    case 4:
        ui->week_label->setText(QStringLiteral("星期四"));
        break;
    case 5:
        ui->week_label->setText(QStringLiteral("星期五"));
        break;
    case 6:
        ui->week_label->setText(QStringLiteral("星期六"));
        break;
    case 7:
        ui->week_label->setText(QStringLiteral("星期日"));
        break;
    default:
        break;
    }
}

void RenhuaiNew::on_loginButton_clicked()
{
    loginDialog->show();
}

void RenhuaiNew::updateLoginState()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        ui->currentUserLabel->setText(user->getUsername());
        ui->logoutPushButton->setText(QStringLiteral("登出"));
        ui->switchUserPushButton->setEnabled(true);
    }
    else
    {
        ui->currentUserLabel->setText("");
        ui->logoutPushButton->setText(QStringLiteral("登入"));
        ui->switchUserPushButton->setEnabled(false);
    }
}

void RenhuaiNew::on_ferControlButton_clicked()
{
    fercontrolDialog->show();
}

void RenhuaiNew::on_SettingButton_clicked()
{
    //settingDialog->show();
    ferconfigDialog->show();
}

void RenhuaiNew::on_logoutPushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        Identity::getInstance()->reset();
        updateLoginState();
    }
    else
    {
        loginDialog->show();
    }
}

void RenhuaiNew::on_switchUserPushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        loginDialog->show();
    }
}

void RenhuaiNew::on_fanControlButton_clicked()
{
    //sfcontrolDialog->show();
    fanControlDialog->show();
}

void RenhuaiNew::on_fanValveControlButton_clicked()
{
    //fanValveControlDialog->show();
    //sensorControlDialog->show();
}

void RenhuaiNew::on_HistoryQueryButton_clicked()
{
    historyDialog->show();
}

void RenhuaiNew::on_realTimeChartButton_clicked()
{
    RealTimeDialog *realTimeDialog= new RealTimeDialog(this);
    realTimeDialog->show();
}

void RenhuaiNew::on_ReportButton_clicked()
{
    reportDialog->show();
}

void RenhuaiNew::on_AlertQueryButton_clicked()
{
    alertQueryDialog->show();
}

void RenhuaiNew::on_exitButton_clicked()
{
    reportDialog->close();
    historyDialog->close();
    alertQueryDialog->close();
    sfcontrolDialog->close();
    fanControlDialog->close();
    //settingDialog->close();
    ferconfigDialog->close();
    fercontrolDialog->close();

    QApplication::quit();
}
