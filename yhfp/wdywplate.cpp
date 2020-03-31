#include "wdywplate.h"
#include "ui_wdywplate.h"

WdywPlate::WdywPlate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WdywPlate)
{
    ui->setupUi(this);

    ui->frame->setLeftRange(0, 150);
    ui->frame->setRightRange(0, 150);
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
        ui->l_label->setText(QString::number(leftValue));
        ui->r_label->setText(QString::number(rightValue));
        _leftValue = leftValue;
        _rightValue = rightValue;
    }
}
