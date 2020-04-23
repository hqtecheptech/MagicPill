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

private:
    Ui::DeoManualSettingDialog *ui;

    bool fan_1_started, fan_2_started, pump_1_started, pump_2_started;
    bool pump_pl_1_started, pump_pl_2_started;
    bool valve_bw_started, heater_1_started, heater_2_started;
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
