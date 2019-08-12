#include "yhcwatchs.h"
#include "ui_yhcwatchs.h"

yhcWatchs::yhcWatchs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::yhcWatchs)
{
    ui->setupUi(this);
}

yhcWatchs::~yhcWatchs()
{
    delete ui;
}

void yhcWatchs::updateDydl(int leftValue, int rightValue)
{
    ui->dydlplateWidget->updatePlates(leftValue, rightValue);
}

void yhcWatchs::updateWdyw(int leftValue, int rightValue)
{
    ui->wdywplateWidget->updatePlates(leftValue, rightValue);
}
