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

void FpjWatchs::setLeftTopPlateRange(int leftMin, int leftMax, int rightMin, int rightMax)
{
    ui->leftTopWidget->setRange(leftMin, leftMax, rightMin, rightMax);
}

void FpjWatchs::setRightTopPlateRange(int leftMin, int leftMax, int rightMin, int rightMax)
{
    ui->rightTopWidget->setRange(leftMin, leftMax, rightMin, rightMax);
}

void FpjWatchs::setLeftBottomPlateRange(int leftMin, int leftMax, int rightMin, int rightMax)
{
    ui->leftBottomWidget->setRange(leftMin, leftMax, rightMin, rightMax);
}

void FpjWatchs::setRightBottomPlateRange(int leftMin, int leftMax, int rightMin, int rightMax)
{
    ui->rightBottomWidget->setRange(leftMin, leftMax, rightMin, rightMax);
}

void FpjWatchs::setLeftCenterPlateRange(int leftMin, int leftMax, int rightMin, int rightMax)
{
    ui->leftCenterWidget->setRange(leftMin, leftMax, rightMin, rightMax);
}

void FpjWatchs::setRightCenterPlateRange(int leftMin, int leftMax, int rightMin, int rightMax)
{
    //ui->rightCenterWidget->setRange(leftMin, leftMax, rightMin, rightMax);
}

void FpjWatchs::updateLeftTopPlate(int leftValue, int rightValue)
{
    //ui->leftTopWidget->updatePlates(leftValue, rightValue);
}

void FpjWatchs::updateRightTopPlate(int leftValue, int rightValue)
{
    ui->rightTopWidget->updatePlates(leftValue, rightValue);
}

void FpjWatchs::updateLeftBottomPlate(int leftValue, int rightValue)
{
    ui->leftBottomWidget->updatePlates(leftValue, rightValue);
}

void FpjWatchs::updateRightBottomPlate(int leftValue, int rightValue)
{
    ui->rightBottomWidget->updatePlates(leftValue, rightValue);
}

void FpjWatchs::updateLeftCenterPlate(int leftValue, int rightValue)
{
    ui->leftCenterWidget->updatePlates(leftValue, rightValue);
}

void FpjWatchs::updateRightCenterPlate(int leftValue, int rightValue)
{
    //ui->rightCenterWidget->updatePlates(leftValue, rightValue);
}
