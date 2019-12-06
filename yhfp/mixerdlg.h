#ifndef MIXERDLG_H
#define MIXERDLG_H

#include <QDialog>
#include "netstatemanageworker.h"
#include "syscontroller.h"
#include "data.h"

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

signals:
    void checkNetState(QString type);
    void pollingDatas();
    void histDataReady(HistData data);

public slots:
    void handleControllerResult();
    void handlePlcDataUpdate(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);
    void wirteTestData();
    void switchState();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private:
    Ui::MixerDlg *ui;

    QThread netManageThread;
    NetStateManageWorker* nsmWorker;
    QThread dbThread;
    QTimer* checkNetStateTimer;
    QTimer* testTimer;
    QTimer* switchStateTimer;
    QTimer* updateWatchsTimer;
    int uca = 0;
    Syscontroller* controller;
    Plc_Db oldPlcDb;
    int deviceIndex = 0;
    bool started = false;
    bool stateFlag = false;

    bool wnhjdjFault, wnzpdjFault, flzpdjFault, fllxdjFault, fhlxdjFault;
    bool wnjrFault, wnyxywLow, fljrFault, flyxywLow;

    void parseData(QMap<float,QString> dataMap);
    void parseRunCtrData(QMap<float,QString> dataMap);
};

#endif // MIXERDLG_H
