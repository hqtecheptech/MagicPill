#ifndef MIXERDLG_H
#define MIXERDLG_H

#include <QDialog>
#include "netstatemanageworker.h"
#include "syscontroller.h"
#include "data.h"
#include "tcpclientsocket.h"
#include "parseserverdataworker.h"
#include "identity.h"
#include "mixsettingdialog.h"

namespace Ui {
class MixerDlg;
}

class MixerDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MixerDlg(QWidget *parent = 0);
    ~MixerDlg();

private slots:
    void netStatChecked(QString type, bool state);
    void getNetState();

    void on_exitButton_clicked();

    void on_settingButton_clicked();

signals:
    void checkNetState(QString type);
    void pollingDatas();
    void histDataReady(HistData data);
    void dataChanged();
    void serverConnectionChanged(bool isConnected);
    void dataUpdate(QSet<int>, QMap<float,QString>);
    void serverDataReceived(QByteArray);

public slots:
    void handleControllerResult();
    void handlePlcDataUpdate(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);
    void wirteTestData();
    void switchState();
    void showData(QByteArray);
    void localServerConnected(bool);
    void dispatchData(QSet<int>, QMap<float,QString>);
    void updateData(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);
    void readData();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private:
    Ui::MixerDlg *ui;

    QThread netManageThread;
    NetStateManageWorker* nsmWorker;
    QThread dbThread;
    ParseServerDataWorker *psWorker;
    QThread psThread;
    QTimer* checkNetStateTimer;
    QTimer* testTimer;
    QTimer* switchStateTimer;
    QTimer* updateWatchsTimer;
    QTimer* readDataTimer;
    MixSettingDialog *mixSettingDlg;
    int uca = 0;
    //Syscontroller* controller;
    Plc_Db oldPlcDb;
    int deviceIndex = 0;
    bool started = false;
    bool stateFlag = false;

    bool wnhjdjFault, wnzpdjFault, flzpdjFault, fllxdjFault, fhlxdjFault;
    bool wnjrFault, wnyxywLow, fljrFault, flyxywLow;
    bool hljFault, flczdFault, fhczdFault, shpd_1_Fault, shpd_2_Fault, shpd_3_Fault;

    TcpClientSocket* getServerConnectStateTcpClient;
    TcpClientSocket* getAllDataTcpClient;
    TcpClientSocket* actionTcpClient;
    bool isServerConnected = false;

    void parseData(QMap<float,QString> dataMap);
    void parseRunCtrData(QMap<float,QString> dataMap);
};

#endif // MIXERDLG_H
