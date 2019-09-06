#include "fpjwatchs.h"
#include "ui_fpjwatchs.h"

FpjWatchs::FpjWatchs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FpjWatchs)
{
    ui->setupUi(this);
}

FpjWatchs::~FpjWatchs()
{
    delete ui;
}
