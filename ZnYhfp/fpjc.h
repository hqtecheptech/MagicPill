#ifndef FPJC_H
#define FPJC_H

#include <QDialog>
#include <QTimer>
#include <QTime>
#include <QVector>
#include <QThread>
#include "data.h"
#include "dataformat.h"
#include "syscontroller.h"
#include "netstatemanageworker.h"
#include "alerthistorydialog.h"
#include "historydlg.h"
#include "databaseworker.h"
#include "fpjrunstatusdialog.h"

#include "data.h"
#include "global.h"

namespace Ui {
class fpjc;
}

class Fpjc : public QDialog
{
    Q_OBJECT

public:
    explicit Fpjc(QWidget *parent = 0);
    ~Fpjc();

signals:
    void histDataReady(HistData data);

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:

    void on_exitButton_clicked();
    void updateWatch();
    void updateChart();
    void checkWifiConnection();

    void on_leftFpButton_clicked();

    void on_leftLdButton_clicked();

    void on_rightFpButton_clicked();

    void on_rightLdButton_clicked();

    void on_alertButton_clicked();

    void on_historyButton_clicked();

    void on_assistButton_clicked();

private:
    Ui::fpjc *ui;

    QThread dbThread;
    DatabaseWorker* dbWorker;
    QThread netManageThread;
    NetStateManageWorker* nsmWorker;
    QTimer* checkNetStateTimer;
    QTimer* testTimer;
    QTimer* updateWatchTimer;
    QTimer* updateChartTimer;
    QTime st;
    int uca = 0;
    Syscontroller* controller;
    Plc_Db oldPlcDb;
    int deviceIndex = 0;
    QList<int> lrpArr;
    QList<int> lrsArr;
    QList<int> lwpArr;
    QList<int> lspArr;
    QList<int> rrpArr;
    QList<int> rrsArr;
    QList<int> rwpArr;
    QList<int> rspArr;
    bool leftChartType = false;
    bool rightChartType = false;
    AlertHistoryDialog *alertHisDlg;
    HistoryDlg *hisDlg;
    FpjRunStatusDialog *runStatusDlg;

    int _deviceIndex = 0;
};

#endif // FPJC_H
