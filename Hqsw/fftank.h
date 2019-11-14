#ifndef FFTANK_H
#define FFTANK_H

#include "dataformat.h"
#include "tcpclientsocket.h"
#include "fancontroldialog.h"
#include "basetankwidget.h"

#include <QWidget>
#include <QSplitter>
#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QPalette>
#include <mytimerthread.h>
#include <QMessageBox>
#include <QTimer>

namespace Ui {
class FFTank;
}

class FFTank : public BaseTankWidget
{
    Q_OBJECT

public:
    explicit FFTank(QWidget *parent = 0);
    ~FFTank();

    int getTankIndex() const;
    void setTankIndex(int value);

    int getLocation() const;
    void setLocation(int value);

public slots:
    void updateFermentationData(QSet<int>, QMap<float,QString> dataMap);

private slots:
    void updateDeviceState();
    void switchState();
    void on_fanButton_clicked();

signals:
    void dataChanged(QSet<int>, QMap<float,QString> dataMap);

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::FFTank *ui;

    QTimer *switchStateTimer;
    FanControlDialog *fanControlDialog;
    TcpClientSocket* tcpClient;
    MyTimerThread *myTimerThread;
    QPalette p;
    QPixmap tankRunBgImg;
    QPixmap tankRunQBgImg;
    QPixmap tankStopBgImg;
    QPixmap fanOpenBgImg;
    QPixmap fanCloseBgImg;
    QPixmap fanFaultBgImg;
    QPixmap tankRunFanOpenBgImg;
    QPixmap ferAutoBgImg;
    QPixmap ferCustomBgImg;
    QMessageBox msgBox;
    //bool isFanRemote = false;
    bool isFanOpen = false;
    //bool isFanAuto = false;
    bool isFerStart = false;
    //bool isAeration = false;
    bool isFerAuto = false;
    bool isFanFault = false;
    bool isSwitchFault = false;
    bool switchFlag = true;
    int tankIndex = 0;
    int location = 0;
    uint latestFerRunTime = 0;
    uint latestFerAeTime = 0;
    uint latestFerStaTime = 0;

    QString formatLongDateString(uint value);

    void parseFermentationData(QMap<float,QString> dataMap);
    void parseFerRunTimeData(QMap<float,QString> dataMap);
    void parseFerStepData(QMap<float,QString> dataMap);
    void parseFerRunCtrData(QMap<float,QString> dataMap);
    void parseFerStartEndTime(QMap<float,QString> dataMap);

    uint extraRuntimeAddition = 0;
};

#endif // FFTANK_H
