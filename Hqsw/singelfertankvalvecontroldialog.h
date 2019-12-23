#ifndef SINGELFERTANKVALVECONTROLDIALOG_H
#define SINGELFERTANKVALVECONTROLDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QMessageBox>

#include "dataformat.h"
#include "mytimerthread.h"
#include "tcpclientsocket.h"
#include "uiglobal.h"

namespace Ui {
class SingelFerTankValveControlDialog;
}

class SingelFerTankValveControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SingelFerTankValveControlDialog(QWidget *parent = 0);
    ~SingelFerTankValveControlDialog();

    int getValveIndex() const;
    void setValveIndex(int value);

private:
    Ui::SingelFerTankValveControlDialog *ui;

    int valveIndex = 0;
    MyTimerThread *myTimerThread;
    TcpClientSocket* getServerConnectStateTcpClient;
    TcpClientSocket* getValveDataTcpClient;
    TcpClientSocket *actionTcpclient;
    bool isServerConnected = false;
    bool switchFlag = false;
    bool isAuto =false;
    QPalette p;
    QPixmap icoGreen;
    QPixmap icoYellow;
    QPixmap icoRed;
    QPixmap valveAutoControlBg;
    QPixmap valveManualControlBg;
    QMessageBox msgBox;

    int start;
    int stop;

    void parseRunCtrData(QMap<float,QString> map);

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *);

public slots:
    void showValveData(QByteArray);
    void localServerConnected(bool);

private slots:
    void read_valve_data();

    void on_valveOpenPushButton_pressed();
    void on_valveStopPushButton_pressed();
    void on_valveOpenPushButton_released();
    void on_valveStopPushButton_released();
    void on_switchValveControlModePushButton_clicked();
};

#endif // SINGELFERTANKVALVECONTROLDIALOG_H
