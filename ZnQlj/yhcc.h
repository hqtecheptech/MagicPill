#ifndef YHCC_H
#define YHCC_H

#include <QDialog>
#include <QTimer>
#include <QTime>

#include "syscontroller.h"
#include "data.h"
#include "databaseworker.h"
#include "dataformat.h"
#include "historydlg.h"
#include "netstatemanageworker.h"
#include "tcpclientsocket.h"
#include "parseserverdataworker.h"
#include "global.h"
#include "uiglobal.h"
#include "identity.h"
#include "alerthistorydialog.h"
#include "yhcrunstatusdialog.h"
#include "fpjrunstatusdialog.h"

namespace Ui {
class Yhcc;
}

class Yhcc : public QDialog
{
    Q_OBJECT

public:
    explicit Yhcc(QWidget *parent = 0);
    ~Yhcc();

private slots:
    void on_yhcExitButton_clicked();

    void on_speedDownButton_pressed();

    void on_speedDownButton_released();

    void on_speedUpButton_pressed();

    void on_speedUpButton_released();

    void on_yhDownButton_pressed();

    void on_yhDownButton_released();

    void on_yhUpButton_pressed();

    void on_yhUpButton_released();

    void netStatChecked(QString type, bool state);

    void getNetState();

    void on_speedDownButton_clicked();

    void on_speedUpButton_clicked();

    void on_historyButton_clicked();

    void updateCharts();

    void on_alertButton_clicked();

    void on_assistButton_clicked();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

signals:
    void checkNetState(QString type);
    void pollingDatas();
    void histDataReady(HistData data);    
    void dataChanged();
    void serverConnectionChanged(bool isConnected);
    void dataUpdate(QSet<int>, QMap<float,QString>);
    void serverDataReceived(QByteArray);
    void simpleAlertReady(QString);
    void alertReady(QString);

public slots:
    void handleControllerResult();
    void handlePlcDataUpdate(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);
    void wirteTestData();
    void updateWatchs(QMap<float,QString> dataMap);
    void showData(QByteArray);
    void localServerConnected(bool);
    void dispatchData(QSet<int>, QMap<float,QString>);
    void updateData(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);
    void readData();

private:
    Ui::Yhcc *ui;

    //YhcRunStatusDialog *runStatusDlg;
    FpjRunStatusDialog *runStatusDlg;
    HistoryDlg *hisDlg;
    QThread netManageThread;
    NetStateManageWorker* nsmWorker;
    QThread dbThread;
    DatabaseWorker* dbWorker;
    QThread psThread;
    ParseServerDataWorker *psWorker;
    QTimer* checkNetStateTimer;
    QTimer* testTimer;
    QTimer* updateChartsTimer;
    QTimer* readDataTimer;
    QTime st;
    int uca = 0;
    Plc_Db oldPlcDb;
    int _deviceIndex = 0;

    AlertHistoryDialog *alertHisDlg;

    TcpClientSocket* getServerConnectStateTcpClient;
    TcpClientSocket* getAllDataTcpClient;
    TcpClientSocket* actionTcpClient;
    bool isServerConnected = false;

    float currentSpeed = 0.0;

    void parseYhcData(QMap<float,QString> dataMap);
    void parseYhcRunCtrData(QMap<float,QString> dataMap);
};

#endif // YHCC_H
