#ifndef TANKAIRVALVE_H
#define TANKAIRVALVE_H

#include <QWidget>
#include <QMessageBox>

#include <mytimerthread.h>
#include "tcpclientsocket.h"
#include "basepartwidget.h"

namespace Ui {
class TankAirValve;
}

class TankAirValve : public BasePartWidget
{
    Q_OBJECT

public:
    explicit TankAirValve(QWidget *parent = 0);
    ~TankAirValve();

    bool getDeviceOpenedState() const;
    void setDeviceOpenedState(bool value);

    bool getDeviceClosedState() const;
    void setDeviceClosedState(bool value);

    QString getTextIndexValue() const;
    void setTextIndexValue(const QString &value);

    QString getPrefix() const;
    void setPrefix(const QString &value);

    int getDeviceIndex() const;
    void setDeviceIndex(int value);

    bool getDeviceOpeningState() const;
    void setDeviceOpeningState(bool value);

    bool getDeviceClosingState() const;
    void setDeviceClosingState(bool value);

    bool getDeviceFaultState() const;
    void setDeviceFaultState(bool value);

    bool getEnable() const;
    void setEnable(bool value);

    QString getName();
    void setName(const QString &name);

    QString getOpendSignalName();
    QString getClosedSignalName();
    QString getFaultSignalName();

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::TankAirValve *ui;
    MyTimerThread *myTimerThread;
    TcpClientSocket *actionTcpclient;
    bool isValveOpen = false;
    bool isValveClose = false;
    bool isValveOpening = false;
    bool isValveClosing = false;
    bool isValveFault = false;
    bool enable = true;
    bool switchFlag = false;
    QString textIndexValue;
    QString valveName;
    QPalette p;
    QPixmap valveOpenBgImg;
    QPixmap valveCloseBgImg;
    QPixmap valveFaultBgImg;
    QPixmap valveSwitchingBgImg;
    int valveIndex = 0;
    int deviceIndex = 0;
    QString prefix;
    QMessageBox msgBox;
    QString openValveControlPointName = "";

private slots:
    void updateValveState();
    void on_valvePushButton_pressed();
    void on_valvePushButton_released();
};

#endif // TANKAIRVALVE_H
