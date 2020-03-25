#include "ylzsplate.h"
#include "ui_ylzsplate.h"

YlzsPlate::YlzsPlate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::YlzsPlate)
{
    ui->setupUi(this);

    _leftMin = 0;
    _leftMax = 300;
    _rightMin = 0;
    _rightMax = 100;
    ui->frame->setLeftRange(0, _leftMax);
    ui->frame->setRightRange(0, _rightMax);
    ui->frame->setValue(0, 0);
}

YlzsPlate::~YlzsPlate()
{
    delete ui;
}

void YlzsPlate::updatePlates(int leftValue, int rightValue)
{
    if(_leftValue != leftValue || _rightValue != rightValue)
    {
        ui->frame->setValue(leftValue, rightValue);
        _leftValue = leftValue;
        _rightValue = rightValue;
    }
}

void YlzsPlate::setRange(int leftMin, int leftMax, int rightMin, int rightMax)
{
    _leftMin = leftMin;
    _leftMax = leftMax;
    _rightMin = rightMin;
    _rightMax = rightMax;
    ui->frame->setLeftRange(_leftMin, _leftMax);
    ui->frame->setRightRange(_rightMin, _rightMax);
    ui->frame->setValue(0, 0);
}
