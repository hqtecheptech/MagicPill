#ifndef DEOMANUALSETTINGDIALOG_H
#define DEOMANUALSETTINGDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QPixmap>

#include "dataformat.h"
#include "tcpclientsocket.h"
#include "mytimerthread.h"

namespace Ui {
class DeoManualSettingDialog;
}

class DeoManualSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeoManualSettingDialog(QWidget *parent = 0);
    ~DeoManualSettingDialog();

public slots:
    void updateFermentationData(QSet<int> changedDataSet, QMap<float,QString> dataMap);

private slots:
    void on_startFirstFanButton_clicked();

    void on_startSecondFanButton_clicked();

    void on_startFirstPumpButton_clicked();

    void on_startSecondPumpButton_clicked();

    void on_startFirstPlPumpButton_clicked();

    void on_startSecondPlPumpButton_clicked();

    void on_startFirstHeaterButton_clicked();

    void on_startSecondHeaterButton_clicked();

    void on_startBwValveButton_clicked();

    void on_startBwValveButton_2_clicked();

    void on_switchFanModePushButton_clicked();

    void on_switchWaterTankModePushButton_clicked();

    void on_startThirdFanButton_clicked();

    void on_startThirdPumpButton_clicked();

    void on_startForthPumpButton_clicked();

    void on_startSecondBwValveButton_clicked();

    void on_startFirstBwValveButton_clicked();

    void on_startThirdBwValveButton_clicked();

    void on_startForthBwValveButton_clicked();

    void on_startThirdPlPumpButton_clicked();

    void on_startForthPlPumpButton_clicked();

    void on_startFifthPlPumpButton_clicked();

    void on_startSixthPlPumpButton_clicked();

    void on_startFirstAmPumpButton_clicked();

    void on_startSecondAmPumpButton_clicked();

private:
    Ui::DeoManualSettingDialog *ui;

    bool fan_1_started, fan_2_started, fan_3_started, pump_1_started, pump_2_started, pump_3_started, pump_4_started;
    bool pump_pl_1_started, pump_pl_2_started, pump_pl_3_started, pump_pl_4_started, pump_pl_5_started, pump_pl_6_started;
    bool valve_bw_1_started, valve_bw_2_started, valve_bw_3_started, valve_bw_4_started;
    bool pump_am_1_started, pump_am_2_started;
    bool heater_1_started, heater_2_started;
    bool fanMode = false;
    bool wtMode = false;
    TcpClientSocket *tcpClient;
    QMessageBox *msgBox;
    int deviceIndex = 0;

    QPixmap fanAutoControlBg;
    QPixmap fanManualControlBg;

    void parseDeoData(QMap<float,QString> dataMap);
    void parseDeoRunCtrData(QMap<float,QString> dataMap);
};

#endif // DEOMANUALSETTINGDIALOG_H
