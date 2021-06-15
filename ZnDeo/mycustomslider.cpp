#include "mycustomslider.h"

#include <QPalette>

MyCustomSlider::MyCustomSlider(QWidget *parent) : QSlider(parent)
{
    m_displayLabel = new QLabel(this);
    m_displayLabel->setAlignment(Qt::AlignVCenter);
    m_displayLabel->setFixedSize(QSize(16,14));
    m_displayLabel->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background,Qt::white);
    m_displayLabel->setPalette(palette);
    m_displayLabel->setAlignment(Qt::AlignCenter);
    m_displayLabel->setVisible(false);
    m_displayLabel->move(0,-2);
}

MyCustomSlider::~MyCustomSlider()
{

}

void MyCustomSlider::mousePressEvent(QMouseEvent *event)
{
    if(!m_displayLabel->isVisible())
    {
        m_displayLabel->setVisible(true);
        m_displayLabel->setText(QString::number(this->value()));
    }
    QSlider::mousePressEvent(event);
}

void MyCustomSlider::mouseReleaseEvent(QMouseEvent *event)
{
    /*if(m_displayLabel->isVisible())
    {
        m_displayLabel->setVisible(false);
    }*/
    m_displayLabel->setText(QString::number(this->value()));
    m_displayLabel->move((this->width()-m_displayLabel->width())*this->value()/(this->maximum()-this->minimum()),-2);
    QSlider::mouseReleaseEvent(event);
}

void MyCustomSlider::mouseMoveEvent(QMouseEvent *event)
{
    m_displayLabel->setText(QString::number(this->value()));
    m_displayLabel->move((this->width()-m_displayLabel->width())*this->value()/(this->maximum()-this->minimum()),-2);
    QSlider::mouseMoveEvent(event);
}
