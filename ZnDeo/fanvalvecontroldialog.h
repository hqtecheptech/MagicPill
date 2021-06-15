#ifndef FANVALVECONTROLDIALOG_H
#define FANVALVECONTROLDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QMessageBox>

#include "dataformat.h"
#include "mytimerthread.h"
#include "tcpclientsocket.h"
#include "uiglobal.h"

namespace Ui {
class FanValveControlDialog;
}

class FanValveControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FanValveControlDialog(QWidget *parent = 0);
    ~FanValveControlDialog();

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *);

private:
    Ui::FanValveControlDialog *ui;

    MyTimerThread *myTimerThread;
    TcpClientSocket* getServerConnectStateTcpClient;
    TcpClientSocket* getValveGroupDataTcpClient;
    TcpClientSocket *actionTcpclient;
    bool isServerConnected = false;
    bool switchFlag = false;
    bool isValveControlAuto = false;
    bool isBackupValveControlAuto = false;
    QPalette p;
    QPixmap icoGreen;
    QPixmap icoYellow;
    QPixmap icoRed;
    QPixmap valveAutoControlBg;
    QPixmap valveManualControlBg;
    QMessageBox msgBox;

    void parseRunCtrData(QMap<float,QString> map);

public slots:
    void showValveGroupData(QByteArray);
    void localServerConnected(bool);

private slots:
    void read_valve_group_data();
    void on_switchValveControlModePushButton_clicked();
    void on_switchBackupValveControlModePushButton_clicked();
    void on_valveOpenPushButton_pressed();
    void on_valveStopPushButton_released();
    void on_valveOpenPushButton_released();
    void on_valveStopPushButton_pressed();
    void on_backupValveOpenPushButton_pressed();
    void on_backupValveOpenPushButton_released();
    void on_backupValveStopPushButton_pressed();
    void on_backupValveStopPushButton_released();
};

#endif // FANVALVECONTROLDIALOG_H
