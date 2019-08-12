#include "lrzsplate.h"
#include "ui_lrzsplate.h"

lrzsPlate::lrzsPlate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lrzsPlate)
{
    ui->setupUi(this);

    ui->frame->setLeftRange(0, 25);
    ui->frame->setRightRange(0, 25);
    ui->frame->setValue(20, 20);
}

lrzsPlate::~lrzsPlate()
{
    delete ui;
}
