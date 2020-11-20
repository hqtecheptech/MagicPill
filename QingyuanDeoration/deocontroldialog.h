#ifndef DEOCONTROLDIALOG_H
#define DEOCONTROLDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "deodorationfan.h"
#include "pipevalve.h"
#include "tankairvalve.h"
#include "mytimerthread.h"
#include "tcpclientsocket.h"
#include "deosettingdialog.h"
#include "uiglobal.h"

namespace Ui {
class DeoControlDialog;
}

class DeoControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeoControlDialog(QWidget *parent = 0);
    ~DeoControlDialog();

private slots:
    void on_pushButton_clicked();
    void testRunCode();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void updateDeviceState();
    void localServerConnected(bool);
    void showAllData(QByteArray);
    void switchBlowingImg();

    void on_switchOperationModeButton_clicked();

    void on_startLineOneButton_clicked();

    void on_stopLineOneButton_clicked();

    void on_startLineTwoButton_clicked();

    void on_stopLineTwoButton_clicked();

    void on_startSparyPumpButton_clicked();

    void on_stopSparyPumpButton_clicked();

    void on_settingButton_clicked();

    void on_deviceIndexComboBox_currentIndexChanged(int index);

private:
    Ui::DeoControlDialog *ui;
    TankAirValve *tankAirValve1;
    TankAirValve *tankAirValve2;
    TankAirValve *tankAirValve3;
    TankAirValve *tankAirValve4;
    TankAirValve *tankAirValve5;
    TankAirValve *tankAirValve6;
    PiPeValve *pipeValve1;
    PiPeValve *pipeValve2;
    DeodorationFan *fan1;
    DeodorationFan *fan2;
    DeodorationFan *backupFan;
    DeoSettingDialog *deoSettingDialog;
    MyTimerThread *myTimerThread;
    TcpClientSocket *getAllDataTcpclient;
    TcpClientSocket *getServerConnectStateTcpClient;
    TcpClientSocket *actionTcpclient;
    bool isServerConnected = false;
    bool faultFlag = false;
    QMap<float,QString> currentDeodorationDataMap;
    QPixmap brownArrowImg;
    QPixmap blankImg;
    int blowing = 0;
    QTimer *blowTimer;
    void parseDeodorationData(QMap<float,QString> dataMap);
    void parseRunCtrData(QMap<float,QString> dataMap);
    QMessageBox msgBox;
};

#endif // DEOCONTROLDIALOG_H
