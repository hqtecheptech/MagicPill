#include "dydlplate.h"
#include "ui_dydlplate.h"

DydlPlate::DydlPlate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DydlPlate)
{
    ui->setupUi(this);

    ui->frame->setLeftRange(0, 500);
    ui->frame->setRightRange(0, 500);
    ui->frame->setValue(0, 0);
}

DydlPlate::~DydlPlate()
{
    delete ui;
}

void DydlPlate::updatePlates(int leftValue, int rightValue)
{
    if(_leftValue != leftValue || _rightValue != rightValue)
    {
        ui->frame->setValue(leftValue, rightValue);
        _leftValue = leftValue;
        _rightValue = rightValue;
    }
}
