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

    void on_startSecondFanButton_clicked();

    void on_startFirstPumpButton_clicked();

    void on_startSecondPumpButton_clicked();

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

    void on_startFirstFanButton_pressed();

    void on_startFirstFanButton_released();

    void on_stopFirstFanButton_pressed();

    void on_stopFirstFanButton_released();

    void on_startFirstPumpButton_pressed();

    void on_startFirstPumpButton_released();

    void on_stopFirstPumpButton_pressed();

    void on_stopFirstPumpButton_released();

    void on_startSecondPumpButton_pressed();

    void on_startSecondPumpButton_released();

    void on_startFirstPlPumpButton_pressed();

    void on_startFirstPlPumpButton_released();

    void on_stopFirstPumpPlButton_pressed();

    void on_stopFirstPumpPlButton_released();

    void on_startSecondPlPumpButton_pressed();

    void on_startSecondPlPumpButton_released();

    void on_stopSecondPumpPLButton_pressed();

    void on_stopSecondPumpPLButton_released();

    void on_startFirstBwValveButton_pressed();

    void on_startFirstBwValveButton_released();

    void on_stopFirstBwValveButton_pressed();

    void on_stopFirstBwValveButton_released();

    void on_startSecondBwValveButton_pressed();

    void on_startSecondBwValveButton_released();

    void on_stopSecondBwValveButton_pressed();

    void on_stopSecondBwValveButton_released();

    void on_switchPumpPlSbPushButton_1_clicked();

    void on_switchPumpPlSbPushButton_2_clicked();

    void on_switchPumpPlModePushButton_1_clicked();

    void on_switchPumpPlModePushButton_2_clicked();

    void on_switchFanSbPushButton_clicked();

    void on_switchPumpModePushButton_1_clicked();

    void on_switchPumpModePushButton_2_clicked();

    void on_switchPumpSbPushButton_1_clicked();

    void on_switchPumpSbPushButton_2_clicked();

    void on_switchBwValveModePushButton_1_clicked();

    void on_switchBwValveModePushButton_2_clicked();

    void on_stopSecondPumpButton_released();

    void on_stopSecondPumpButton_pressed();

private:
    Ui::DeoManualSettingDialog *ui;

    bool fan_1_started, fan_2_started, fan_3_started, pump_1_started, pump_2_started, pump_3_started, pump_4_started;
    bool pump_pl_1_started, pump_pl_2_started, pump_pl_3_started, pump_pl_4_started, pump_pl_5_started, pump_pl_6_started;
    bool valve_bw_1_started, valve_bw_2_started, valve_bw_3_started, valve_bw_4_started;
    bool pump_am_1_started, pump_am_2_started;
    bool heater_1_started, heater_2_started;
    bool fanMode, fanSbMode, pump_1_mode, pump_2_mode, pump_1_sb_mode, pump_2_sb_mode;
    bool pump_pl_1_mode, pump_pl_2_mode, pump_pl_1_sb_mode, pump_pl_2_sb_mode;
    bool bw_valve_1_mode, bw_valve_2_mode, bw_valve_3_mode;
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
