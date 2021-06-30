#ifndef YHCRUNSTATUSDIALOG_H
#define YHCRUNSTATUSDIALOG_H

#include <QDialog>

#include <QTimer>
#include <data.h>
#include <dataformat.h>
#include <global.h>

namespace Ui {
class YhcRunStatusDialog;
}

class YhcRunStatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit YhcRunStatusDialog(QWidget *parent = 0);
    ~YhcRunStatusDialog();

    int deviceIndex() const;
    void setDeviceIndex(int deviceIndex);

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void showStatus();
    void swithState();
    void on_closePushButton1_clicked();

    void on_closePushButton2_clicked();

private:
    Ui::YhcRunStatusDialog *ui;

    QTimer *timer;
    QTimer *switchStateTimer;
    int _deviceIndex = 0;
    bool YHC_SP_HS_PUMP_FAULT, YHC_HS_PUMP_FAULT, YHC_CF_FAULT, YHC_SP_FILTER_BLOCKING, YHC_FILTER_BLOCKING, YHC_OT_LEVEL, hs_level, filter_blocking;
    bool stateFlag = false;
};

#endif // YHCRUNSTATUSDIALOG_H
