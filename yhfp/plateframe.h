#ifndef PLATEFRAME_H
#define PLATEFRAME_H

#include <QPainter>
#include <QFrame>
#include <QWidget>

class PlateFrame : public QFrame
{
public:
    explicit PlateFrame(QWidget *parent = 0);

    void setStartAngel(int angel);
    void setValue(int leftValue, int rightValue);
    void setAlertValue(int alertValue);
    void setLeftRange(int min, int max);
    void setRightRange(int min, int max);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QColor startColor, QColor endColor, bool hasPen = true);
    int calcLeftAngelLength();
    int calcRightAngelLength();

    int _startAngel=0;
    int _alertValue, _leftValue, _rightValue;
    int _leftMin, _leftMax, _rightMin, _rightMax;
    int _leftRangePosAngel = 100, _leftRangeNegAngel = 0, _leftStartAngel = 0;
    int _rightRangePosAngel = 100, _rightRangeNegAngel = 0, _rightStartAngel = 0;
};

#endif // PLATEFRAME_H
