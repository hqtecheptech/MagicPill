#ifndef DEODORATIONFAN_H
#define DEODORATIONFAN_H

#include <QWidget>
#include <QTimer>
#include <QPalette>

#include "tankairvalve.h"

namespace Ui {
class DeodorationFan;
}

class DeodorationFan : public QWidget
{
    Q_OBJECT

public:
    explicit DeodorationFan(QWidget *parent = 0);
    ~DeodorationFan();

    int getFanIndex() const;
    void setFanIndex(int value);

    bool getFaultState() const;
    void setFaultState(bool value);

    TankAirValve *valveFront;
    TankAirValve *valveBack;

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

public slots:
    void SetFrontValveState(bool);
    void SetBackValveState(bool);
    void SetFanState(bool);

private:
    Ui::DeodorationFan *ui;
    QTimer *fanRotationTimer;
    bool isFanRunning = false;
    bool faultState = false;
    bool faultFlag = false;
    bool fanAngleFlag = false;
    int fanIndex = 0;
    QPixmap fanNegBgImg;
    QPixmap fanPosBgImg;
    QPixmap fanOpenedBgImg;
    QPixmap fanClosedBgImg;
    QPixmap fanFaultBgImg;

private slots:
    void rotateFan();

};

#endif // DEODORATIONFAN_H
