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
    bool FPJ_FB_FAULT, sb_1_fault, FPJ_HEATER_FAULT, FPJ_UFB_FAULT;
    bool FPJ_HS_CF_FAULT, FPJ_OT_LEVEL, FPJ_WALKING_HS_PUMP_FAULT, FPJ_UPLOAD_HS_PUMP_FAULT;
    bool FPJ_WALKING_SD_EM_FAULT, FPJ_CP_HS_PUMP_FAULT, roller_oc_fault, FPJ_SPOOL_FAULT, FPJ_WHFB_FAULT;
    bool fpj_wt_level, FPJ_CPFB_FAULT;
    bool stateFlag = false;
};

#endif // FPJRUNSTATUSDIALOG_H
