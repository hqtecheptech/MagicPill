#include "wdywplate.h"
#include "ui_wdywplate.h"

WdywPlate::WdywPlate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WdywPlate)
{
    ui->setupUi(this);

    ui->frame->setLeftRange(-30, 120);
    ui->frame->setRightRange(0, 1500);
    ui->frame->setValue(0, 0);
}

WdywPlate::~WdywPlate()
{
    delete ui;
}

void WdywPlate::updatePlates(int leftValue, int rightValue)
{
    if(_leftValue != leftValue || _rightValue != rightValue)
    {
        ui->frame->setValue(leftValue, rightValue);
        _leftValue = leftValue;
        _rightValue = rightValue;
    }
}
