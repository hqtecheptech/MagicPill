#include "plateframe.h"
#include <QRadialGradient>
#include <QRectF>
#include <QPainterPath>
#include <QDebug>

PlateFrame::PlateFrame(QWidget *parent)
    :QFrame(parent)
{

}

void PlateFrame::setStartAngel(int angel)
{
    _startAngel = angel;
}

void PlateFrame::setValue(int leftValue, int rightValue)
{
    _leftValue = leftValue;
    _rightValue = rightValue;
    update();
}

void PlateFrame::setAlertValue(int alertValue)
{
    _alertValue = alertValue;
}

void PlateFrame::setLeftRange(int min, int max)
{
    _leftMin = min;
    _leftMax = max;
    if(min == 0)
    {
        _leftStartAngel = 0;
        _leftRangeNegAngel = 0;
        _leftRangePosAngel = 100;
    }
    else if(min < 0)
    {
        _leftStartAngel = qRound(((qreal)min / (qreal)(min - max)) * (qreal)100);
        _leftRangeNegAngel = -_leftStartAngel;
        _leftRangePosAngel = 100 - _leftStartAngel;
    }
}

void PlateFrame::setRightRange(int min, int max)
{
    _rightMin = min;
    _rightMax = max;
    if(min == 0)
    {
        _rightStartAngel = 0;
        _rightRangeNegAngel = 0;
        _rightRangePosAngel = 100;
    }
    else if(min < 0)
    {
        _rightStartAngel = qRound(((qreal)min / (qreal)(min - max)) * (qreal)100);
        _rightRangeNegAngel = -_rightStartAngel;
        _rightRangePosAngel = 100 - _rightStartAngel;
    }
}

void PlateFrame::paintEvent(QPaintEvent *event)
{
    int side = qMin(width(), height());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 260.0, side / 260.0);

    QColor outCircleColor(70, 190, 255, 200);

    painter.setPen(QPen(Qt::white, 2));
    painter.setBrush(outCircleColor);

    //painter.rotate(-90.0);
    //painter.drawText(QRectF(-15, 30, 30, 10), Qt::AlignHCenter,QString("r-90"));

    // adjusted case by case
    int radius = 109;
    int arcOuterHeight = 12;
    int arcInnerHeight = 10;
    int startAngel = 40;

    int rightAngelLength = calcRightAngelLength();
    qDebug() << "_rightStartAngel: " << _rightStartAngel;
    qDebug() << "_rightRangePosAngel: " << _rightRangePosAngel;
    qDebug() << "_rightRangeNegAngel: " << _rightRangeNegAngel;
    qDebug() << "rightAngelLength: " << rightAngelLength;
    //gradientArc(&painter, radius, startAngel - 90,  150, arcOuterHeight, QColor(255, 255, 255, 0), QColor(255, 255, 255, 0));
    //gradientArc(&painter, radius-1, startAngel - 90,  100, arcInnerHeight, QColor(200, 100, 0, 150), QColor(255, 128, 0, 150), false);
    gradientArc(&painter, radius-1, startAngel + _rightStartAngel - 90,  rightAngelLength, arcInnerHeight, QColor(200, 100, 0, 150), QColor(255, 128, 0, 150), false);

    int leftAngelLength = calcLeftAngelLength();
    qDebug() << "_leftStartAngel: " << _leftStartAngel;
    qDebug() << "_leftRangePosAngel: " << _leftRangePosAngel;
    qDebug() << "_leftRangeNegAngel: " << _leftRangeNegAngel;
    qDebug() << "leftAngelLength: " << leftAngelLength;
    //gradientArc(&painter, radius, -startAngel - 90, -150, arcOuterHeight, QColor(255, 255, 255, 0), QColor(255, 255, 255, 0));
    //gradientArc(&painter, radius-1, -startAngel - 90, -100, arcInnerHeight, QColor(0, 200, 20, 150), QColor(0, 255, 64, 150), false);
    gradientArc(&painter, radius-1, -startAngel - _leftStartAngel - 90, -leftAngelLength, arcInnerHeight, QColor(0, 200, 20, 150), QColor(0, 255, 64, 150), false);
}

void PlateFrame::gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QColor startColor, QColor endColor, bool hasPen)
{
    // 渐变色
    QRadialGradient gradient(0, 0, radius);
    gradient.setColorAt(0, startColor);
    gradient.setColorAt(1.0, endColor);
    painter->setBrush(gradient);

    // << 1（左移1位）相当于radius*2 即：150*2=300
    //QRectF(-150, -150, 300, 300)
    QRectF rect(-radius, -radius, radius << 1, radius << 1);
    QPainterPath path;
    path.arcTo(rect, startAngle, angleLength);

    // QRectF(-120, -120, 240, 240)
    QPainterPath subPath;
    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));

    // path为扇形 subPath为椭圆
    path -= subPath;

    //painter->setPen(outlineColor);

    if(hasPen)
    {
        QPen pen(Qt::black, 0.5);
        painter->setPen(pen);
    }
    else
    {
        painter->setPen(Qt::NoPen);
    }

    painter->drawPath(path);
}

int PlateFrame::calcLeftAngelLength()
{
    int len = 0;
    if(_leftMin < 0)
    {
        if(_leftValue < 0)
        {
            len = qRound(((qreal)_leftValue / (qreal)_leftMin) * (qreal)_leftRangeNegAngel);
        }
        else
        {
            len = qRound(((qreal)_leftValue / (qreal)_leftMax) * (qreal)_leftRangePosAngel);
        }
    }
    else
    {
        len = qRound(((qreal)_leftValue / (qreal)_leftMax) * (qreal)100);
    }

    return len;
}

int PlateFrame::calcRightAngelLength()
{
    int len = 0;
    if(_rightMin < 0)
    {
        if(_rightValue < 0)
        {
            len = qRound(((qreal)_rightValue / (qreal)_rightMin) * (qreal)_rightRangeNegAngel);
        }
        else
        {
            len = qRound(((qreal)_rightValue / (qreal)_rightMax) * (qreal)_rightRangePosAngel);
        }
    }
    else
    {
        len = qRound(((qreal)_rightValue / (qreal)_rightMax) * (qreal)100);
    }

    return len;
}
