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
    bool servo_1_fault, sb_1_fault, cooler_1_fault, cooler_2_fault;
    bool spool_fault, sweeping_em_fault, lifting_em_fault, roller_em_fault;
    bool walking_oc_fault, lifting_oc_fault, roller_oc_fault;
    bool os_level;
    bool stateFlag = false;
};

#endif // FPJRUNSTATUSDIALOG_H
