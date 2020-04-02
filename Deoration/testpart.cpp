#include "testpart.h"
#include "ui_testpart.h"

TestPart::TestPart(QWidget *parent) :
    BasePartWidget(parent),
    ui(new Ui::TestPart)
{
    ui->setupUi(this);
}

TestPart::~TestPart()
{
    delete ui;
}

void TestPart::setTextIndexValue(QString strIndex)
{

}

void TestPart::setPrefix(QString prefix)
{

}

void TestPart::setName(QString name)
{
    ui->label->setText(name);
}

void TestPart::setDeviceIndex(int index)
{

}

void TestPart::setDeviceOpenedState(bool value)
{

}

void TestPart::setDeviceClosedState(bool value)
{

}

void TestPart::setDeviceOpeningState(bool value)
{

}

void TestPart::setDeviceClosingState(bool value)
{

}

void TestPart::setDeviceFaultState(bool value)
{

}

void TestPart::setEnable(bool value)
{

}

QString TestPart::getOpendSignalName()
{
    return "inherit opened";
}

QString TestPart::getClosedSignalName()
{
    return "inherit closed";
}
