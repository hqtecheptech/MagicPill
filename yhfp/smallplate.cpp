#include "smallplate.h"
#include "ui_smallplate.h"

#include "data.h"

SmallPlate::SmallPlate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmallPlate)
{
    ui->setupUi(this);

    //ui->frame->setLeftRange(-30, 120);
    //ui->frame->setRightRange(0, 1500);
    //ui->frame->setValue(0, 0);
    //ui->frame->setPlateStyle(QSizeF(200.0, 200.0), 40, 90, 8, 10, Horizontal);

    //updatePlates(120, 1500);
    ui->frame->setLeftRangePosAngel(126);
    ui->frame->setRightRangePosAngel(126);
}

SmallPlate::~SmallPlate()
{
    delete ui;
}

void SmallPlate::setRange(int leftMin, int leftMax, int rightMin, int rightMax)
{
    _leftMin = leftMin;
    _leftMax = leftMax;
    _rightMin = rightMin;
    _rightMax = rightMax;
    ui->frame->setLeftRange(_leftMin, _leftMax);
    ui->frame->setRightRange(_rightMin, _rightMax);
    ui->frame->setValue(0, 0);
    ui->frame->setPlateStyle(QSizeF(200.0, 200.0), 27, 90, 8, 10, Horizontal);
}

void SmallPlate::updatePlates(int leftValue, int rightValue)
{
    if(_leftValue != leftValue || _rightValue != rightValue)
    {
        ui->frame->setValue(leftValue, rightValue);
        ui->l_label->setText(QString::number(leftValue));
        ui->r_label->setText(QString::number(rightValue));
        _leftValue = leftValue;
        _rightValue = rightValue;
    }
}
