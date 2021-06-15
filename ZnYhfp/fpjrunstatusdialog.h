#ifndef FPJRUNSTATUSDIALOG_H
#define FPJRUNSTATUSDIALOG_H

#include <QDialog>

#include <QTimer>
#include <data.h>
#include <dataformat.h>
#include <global.h>

namespace Ui {
class FpjRunStatusDialog;
}

class FpjRunStatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FpjRunStatusDialog(QWidget *parent = 0);
    ~FpjRunStatusDialog();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void showStatus();
    void swithState();

    void on_closePushButton1_clicked();

    void on_closePushButton2_clicked();

private:
    Ui::FpjRunStatusDialog *ui;

    QTimer *timer;
    QTimer *switchStateTimer;
    int _deviceIndex = 0;
    bool cooler_1_state, cooler_2_state;
    bool fpj_nub_pump_fault, sb_1_fault, cooler_1_fault, cooler_2_fault;
    bool spool_fault, fpj_ot_fault, sweeping_lifting_fault, roller_rt_fault;
    bool walking_fault, roller_lifting_fault, roller_oc_fault, sweeping_rt_fault;
    bool fpj_wt_level, fpj_pl_pump_fault;
    bool stateFlag = false;
};

#endif // FPJRUNSTATUSDIALOG_H
