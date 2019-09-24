#ifndef PLATEFRAME_H
#define PLATEFRAME_H

#include <QPainter>
#include <QFrame>
#include <QWidget>

#include "data.h"

class PlateFrame : public QFrame
{
public:
    explicit PlateFrame(QWidget *parent = 0);

    void setStartAngel(int angel);
    void setValue(int leftValue, int rightValue);
    void setAlertValue(int alertValue);
    void setLeftRange(int min, int max);
    void setRightRange(int min, int max);
    void setPlateStyle(QSizeF size, int startAngel, int radius, int arcInnerHeight, int arcOuterHeight, WatchDirection direction);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void gradientArc(QPainter *painter, int _radius, int startAngle, int angleLength, int arcHeight, QColor startColor, QColor endColor, bool hasPen = true);
    int calcLeftAngelLength();
    int calcRightAngelLength();

    QSizeF _size = QSizeF(260.0, 260.0);
    WatchDirection _direction = Vertical;
    int _radius = 109;
    int _arcOuterHeight = 12;
    int _arcInnerHeight = 10;
    int _startAngel = 40;
    int _alertValue, _leftValue, _rightValue;
    int _leftMin, _leftMax, _rightMin, _rightMax;
    int _leftRangePosAngel = 100, _leftRangeNegAngel = 0, _leftStartAngel = 0;
    int _rightRangePosAngel = 100, _rightRangeNegAngel = 0, _rightStartAngel = 0;
};

#endif // PLATEFRAME_H
