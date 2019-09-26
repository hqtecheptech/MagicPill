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

    void netStatChecked(bool state);

    void getNetState();

    void on_speedDownButton_clicked();

    void on_speedUpButton_clicked();

    void on_historyButton_clicked();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

signals:
    void checkNetState(QString type);
    void pollingDatas();
    void histDataReady(HistData data);

public slots:
    void handleControllerResult();
    void handlePlcDataUpdate(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);
    void wirteTestData();
    void updateWatchs();

private:
    Ui::Yhcc *ui;

    HistoryDlg *hisDlg;
    QThread netManageThread;
    NetStateManageWorker* nsmWorker;
    QThread dbThread;
    DatabaseWorker* dbWorker;
    QTimer* checkNetStateTimer;
    QTimer* testTimer;
    QTimer* updateWatchsTimer;
    QTime st;
    int uca = 0;
    Syscontroller* controller;
    Plc_Db oldPlcDb;
    int deviceIndex = 0;
    bool started = false;

    void parseYhcData(QMap<float,QString> dataMap);
    void parseYhcRunCtrData(QMap<float,QString> dataMap);
};

#endif // YHCC_H
