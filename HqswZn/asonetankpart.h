#ifndef ASONETANKPART_H
#define ASONETANKPART_H

#include <QWidget>
#include <QPointF>
#include <QPainterPath>
#include <QMessageBox>

#include "basetankwidget.h"
#include "tcpclientsocket.h"
#include "mytimerthread.h"

namespace Ui {
class AsOneTankPart;
}

class AsOneTankPart : public BaseTankWidget
{
    Q_OBJECT

public:
    explicit AsOneTankPart(QWidget *parent = 0);
    ~AsOneTankPart();

    int getTankIndex() const;
    void setTankIndex(int value);

    int getLocation() const;
    void setLocation(int value);

public slots:
    void updateFermentationData(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);

private slots:
    void updateDeviceState();
    void switchState();

signals:
    void dataChanged(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    Ui::AsOneTankPart *ui;

    QTimer *switchStateTimer;
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
    bool isFanRemote = false;
    bool isFanOpen = false;
    bool isFanAuto = false;
    bool isFerStart = false;
    bool isAeration = false;
    bool isFerAuto = false;
    bool isFanFault = false;
    bool switchFlag = true;
    int tankIndex = 0;
    int location = 0;
    float H1, H2, old_h1, old_h2;
    QPainterPath* path;

    void changePath(QList<QPointF> points);
    void parseFermentationData(QMap<float,QString> dataMap);
    void parseFerRunTimeData(QMap<float,QString> dataMap);
    void parseFerStepData(QMap<float,QString> dataMap);
    void parseFerRunCtrData(QMap<float,QString> dataMap);
    void parseFerStartEndTime(QMap<float,QString> dataMap);
    void parseFerStuffHeight(QMap<float,QString> dataMap);

    QString formatLongDateString(uint value);

    uint extraRuntimeAddition = 0;
};

#endif // ASONETANKPART_H
