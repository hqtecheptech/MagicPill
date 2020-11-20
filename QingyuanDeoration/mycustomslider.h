#ifndef MYCUSTOMERSLIDER_H
#define MYCUSTOMERSLIDER_H

#include <QSlider>
#include <QLabel>

class MyCustomSlider : public QSlider
{
public:
    MyCustomSlider(QWidget *parent=0);
    ~MyCustomSlider();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    QLabel *m_displayLabel;
};

#endif // MYCUSTOMERSLIDER_H
