#include "ylzsplate.h"
#include "ui_ylzsplate.h"

YlzsPlate::YlzsPlate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::YlzsPlate)
{
    ui->setupUi(this);

    ui->frame->setLeftRange(0, 25);
    ui->frame->setRightRange(0, 250);
    ui->frame->setValue(15, 100);
}

YlzsPlate::~YlzsPlate()
{
    delete ui;
}
