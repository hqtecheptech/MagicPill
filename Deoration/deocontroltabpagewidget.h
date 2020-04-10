#ifndef DEOCONTROLTABPAGEWIDGET_H
#define DEOCONTROLTABPAGEWIDGET_H

#define TIME_CONTROL        0
#define AERATION_CONTROL    1
#define AMMO_CONTROL        2

#include <QWidget>
#include <QMessageBox>

#include "deodorationfan.h"
#include "pipevalve.h"
#include "tankairvalve.h"
#include "mytimerthread.h"
#include "tcpclientsocket.h"
#include "deosettingdialog.h"
#include "qthread.h"
#include "parseserverdataworker.h"
#include "singledeodorationfan.h"
#include "uiglobal.h"
#include "deomanualsettingdialog.h"

namespace Ui {
class DeoControlTabPageWidget;
}

class DeoControlTabPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeoControlTabPageWidget(QWidget *parent = 0);
    ~DeoControlTabPageWidget();

private slots:
    void testRunCode();

public slots:
    void dispatchDeoData(QSet<int>, QMap<float,QString>);

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void updateDeviceState();
    void localServerConnected(bool);
    void showAllData(QByteArray);
    void switchBlowingImg();

    void on_settingButton_clicked();

    void on_deviceIndexComboBox_currentIndexChanged(int index);

    void on_startFanOneButton_clicked();

    void on_stopFanOneButton_clicked();

    void on_startFanTwoButton_clicked();

    void on_stopFanTwoButton_clicked();

    void on_timeControlRadioButton_clicked();

    void on_aerationControlRadioButton_clicked();

    void on_ammoControlRadioButton_clicked();

    void on_choseFanOneButton_clicked();

    void on_choseFanTwoButton_clicked();

    void on_manualRunButton_clicked();

private:
    Ui::DeoControlTabPageWidget *ui;

    QThread workerThread;
    ParseServerDataWorker *worker;

    QVector<BasePartWidget *> allGdValveParts;
    QVector<BasePartWidget *> allFjValveParts;
    QVector<SingleDeodorationFan *> allFans;

    TankAirValve *mixAirValve;

    TankAirValve *tankAirValve1;
    TankAirValve *tankAirValve2;
    TankAirValve *tankAirValve3;
    TankAirValve *tankAirValve4;
    TankAirValve *tankAirValve5;
    TankAirValve *tankAirValve6;
    TankAirValve *tankAirValve7;
    TankAirValve *tankAirValve8;
    TankAirValve *tankAirValve9;
    TankAirValve *tankAirValve10;
    TankAirValve *tankAirValve11;
    TankAirValve *tankAirValve12;

    TankAirValve *fjValve1;
    TankAirValve *fjValve2;
    TankAirValve *fjValve3;
    TankAirValve *fjValve4;

    SingleDeodorationFan *fan1;
    SingleDeodorationFan *fan2;

    DeoManualSettingDialog *deoManualSettingDlg;
    DeoSettingDialog *deoSettingDialog;
    MyTimerThread *myTimerThread;
    TcpClientSocket *getAllDataTcpclient;
    TcpClientSocket *getServerConnectStateTcpClient;
    TcpClientSocket *actionTcpclient;
    QMap<float,QString> currentDeodorationDataMap;
    QPixmap brownArrowImg;
    QPixmap greenArrowImg;
    QPixmap brownArrowUpImg;
    QPixmap greenArrowUpImg;
    QPixmap blankImg;
    QPixmap filterPumpOpenedBgImg;
    QPixmap filterPumpClosedBgImg;
    QPixmap filterPumpFaultBgImg;
    QPixmap waterLevelLowBgImg;
    QPixmap waterLevelNormalBgImg;
    bool blowing = false;
    QTimer *blowTimer;
    void parseDeodorationData(QMap<float,QString> dataMap);
    void parseRunCtrData(QMap<float,QString> dataMap);
    QMessageBox msgBox;

    int gdValveNumber = 0;
    int fjValveNumber = 0;
    int fanNumber = 0;
    int controlMode = TIME_CONTROL;

    bool fan1Choosed = false;
    bool fan2Choosed = false;

signals:
    void dataChanged();
    void serverConnectionChanged(bool isConnected);
    void dataUpdate(QSet<int>, QMap<float,QString>);
    void serverDataReceived(QByteArray dataArray);

};

#endif // DEOCONTROLTABPAGEWIDGET_H
