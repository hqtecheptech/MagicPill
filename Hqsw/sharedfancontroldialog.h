#ifndef SHAREDFANCONTROLDIALOG_H
#define SHAREDFANCONTROLDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "dataformat.h"
#include "mytimerthread.h"
#include "tcpclientsocket.h"

namespace Ui {
class SharedFanControlDialog;
}

class SharedFanControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SharedFanControlDialog(QWidget *parent = 0);
    ~SharedFanControlDialog();

    int getFanIndex() const;
    void setFanIndex(int value);

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *);

private:
    Ui::SharedFanControlDialog *ui;

    int frequency = 0;
    int fanIndex = 0;
    MyTimerThread *myTimerThread;
    TcpClientSocket* getServerConnectStateTcpClient;
    TcpClientSocket* getFanGroupDataTcpClient;
    TcpClientSocket *actionTcpclient;
    TcpClientSocket *setFrequencyTcpClient;
    bool isServerConnected = false;
    bool fanMode = false;
    bool switchFlag = false;
    QPalette p;
    QPixmap icoGreen;
    QPixmap icoYellow;
    QPixmap icoRed;
    QPixmap fanAutoControlBg;
    QPixmap fanManualControlBg;
    QMessageBox msgBox;

    void parseHzSettingData(QMap<float,QString> map);
    void parseRunCtrData(QMap<float,QString> map);

public slots:
    void showFanGroupData(QByteArray);
    void localServerConnected(bool);

private slots:
    void read_fan_group_data();
    void on_fanOpenPushButton_pressed();
    void on_fanOpenPushButton_released();
    void on_switchFanControlModePushButton_clicked();
    void on_outputFreqSlider_valueChanged(int value);
    void on_fanIndexComboBox_currentIndexChanged(const QString &arg1);
    void on_fanStopPushButton_pressed();
    void on_fanStopPushButton_released();
    void on_backupFanOpenPushButton_pressed();
    void on_backupFanOpenPushButton_released();
    void on_backupFanStopPushButton_pressed();
    void on_backupFanStopPushButton_released();
    void on_fanIndexComboBox_currentIndexChanged(int index);
};

#endif // SHAREDFANCONTROLDIALOG_H
