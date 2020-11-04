#ifndef SINGLEDEODORATIONFAN_H
#define SINGLEDEODORATIONFAN_H

#include <QWidget>
#include <QMessageBox>

#include "mytimerthread.h"
#include "tcpclientsocket.h"

namespace Ui {
class SingleDeodorationFan;
}

class SingleDeodorationFan : public QWidget
{
    Q_OBJECT

public:
    explicit SingleDeodorationFan(QWidget *parent = 0);
    ~SingleDeodorationFan();

    QString getName() const;
    void setName(const QString &value);

    QString getPrefix() const;
    void setPrefix(const QString &value);

    QString getTextIndexValue() const;
    void setTextIndexValue(const QString &value);

    bool getFanRunState() const;
    void setFanRunState(bool value);

    bool getFanFaultState() const;
    void setFanFaultState(bool value);

    int getDeviceIndex() const;
    void setDeviceIndex(int value);

    bool getFanReadyState() const;
    void setFanReadyState(bool value);

    QString getRunSignalName();
    QString getFaultSignalName();
    QString getReadySignalName();

    QString getStartCtrName();
    QString getStopCtrName();

private:
    Ui::SingleDeodorationFan *ui;

    MyTimerThread *myTimerThread;
    TcpClientSocket *actionTcpclient;

    bool isFanRun = false;
    bool isFanOpen = false;
    bool isFanClose = false;
    bool isFanOpening = false;
    bool isFanClosing = false;
    bool isFanFault = false;
    bool isFanReady = false;
    bool enable = true;
    bool switchFlag = false;

    QString name;
    QString prefix;
    QString textIndexValue;

    int deviceIndex;

    QMessageBox msgBox;
};

#endif // SINGLEDEODORATIONFAN_H
