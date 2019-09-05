#include "smallplate.h"
#include "ui_smallplate.h"

#include "data.h"

SmallPlate::SmallPlate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmallPlate)
{
    ui->setupUi(this);

    ui->frame->setLeftRange(-30, 120);
    ui->frame->setRightRange(0, 1500);
    ui->frame->setValue(0, 0);
    ui->frame->setPlateStyle(QSizeF(200.0, 200.0), 40, 90, 8, 10, Horizontal);

    updatePlates(120, 1500);
}

SmallPlate::~SmallPlate()
{
    delete ui;
}

void SmallPlate::updatePlates(int leftValue, int rightValue)
{
    if(_leftValue != leftValue || _rightValue != rightValue)
    {
        ui->frame->setValue(leftValue, rightValue);
        _leftValue = leftValue;
        _rightValue = rightValue;
    }
}
