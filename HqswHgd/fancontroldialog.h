#ifndef FANCONTROLDIALOG_H
#define FANCONTROLDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDataWidgetMapper>
#include <QMap>

#include "dataformat.h"
#include "requestworkthread.h"
#include "mytimerthread.h"
#include "tcpclientsocket.h"

namespace Ui {
class FanControlDialog;
}

class FanControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FanControlDialog(QWidget *parent = 0);
    ~FanControlDialog();

    int getTankIndex() const;
    void setTankIndex(int value);

    void setCombBoxVisible(bool value);

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *);

public slots:
    void updateFermentationData(QSet<int>, QMap<float,QString>);

private slots:
    void on_fanOpenPushButton_clicked();
    void on_fanStopPushButton_clicked();
    void on_switchFanModePushButton_clicked();
    void fanIndexChanged(int index);
    void localServerConnected(bool);
    void showSwtichFanMode(QByteArray);
    void showFanOpen(QByteArray);
    void showFanClose(QByteArray);
    void showChangeOutputFrequency(QByteArray);
    void getTestResult(int);

    void on_fanIndexComboBox_currentIndexChanged(int index);

    void on_ValveClosePushButton_2_clicked();

    void on_ValveOpenPushButton_clicked();

    void on_ValveClosePushButton_clicked();

    void on_modifyOpenTimeButton_clicked();

    void on_modifyCloseTimeButton_clicked();

    void on_modifyFrePushButton_clicked();

private:
    Ui::FanControlDialog *ui;

    MyTimerThread *myTimerThread;
    int tankIndex = 0;
    int frequency = 0;
    int frequencySetting = 0;
    //bool isFanRemote = false;
    bool isFanRun = false;
    bool fanMode = false;
    bool isFault = false;
    bool isSwitchFault = false;
    bool isValveOpened = false;
    bool isSettig = false;
    RequestWorkThread *work;
    TcpClientSocket *tcpClient;
    TcpClientSocket *actionTcpClient;
    QPalette p;
    QPixmap icoGreen;
    QPixmap icoYellow;
    QPixmap icoRed;
    QPixmap fanAutoControlBg;
    QPixmap fanManualControlBg;
    QMessageBox *msgBox;
    QString oldValveOpen = "";
    QString oldValveClose = "";

    void parseFermentationData(QMap<float,QString> dataMap);
    void parseFerRunTimeData(QMap<float,QString> dataMap);
    void parseFerStepData(QMap<float,QString> dataMap);
    void parseFerRunCtrData(QMap<float,QString> dataMap);
};

#endif // FANCONTROLDIALOG_H
