#include "basepartwidget.h"

BasePartWidget::BasePartWidget(QWidget *parent) : QWidget(parent)
{

}

void BasePartWidget::setTextIndexValue(const QString &strIndex)
{

}

void BasePartWidget::setPrefix(const QString &prefix)
{

}

void BasePartWidget::setName(const QString &name)
{
    "base part";
}

void BasePartWidget::setDeviceIndex(int index)
{

}

void BasePartWidget::setDeviceOpenedState(bool value)
{

}

void BasePartWidget::setDeviceClosedState(bool value)
{

}

void BasePartWidget::setDeviceOpeningState(bool value)
{

}

void BasePartWidget::setDeviceClosingState(bool value)
{

}

void BasePartWidget::setDeviceFaultState(bool value)
{

}

void BasePartWidget::setEnable(bool value)
{

}

QString BasePartWidget::getOpendSignalName()
{
    return "opened";
}

QString BasePartWidget::getClosedSignalName()
{
    return "closed";
}

QString BasePartWidget::getFaultSignalName()
{
    return "fault";
}

QString BasePartWidget::getTextIndexValue()
{
    return "";
}

QString BasePartWidget::getPrefix()
{
    return "";
}

QString BasePartWidget::getName()
{
    return "";
}

int BasePartWidget::getDeviceIndex()
{
    return -1;
}

bool BasePartWidget::getDeviceOpenedState()
{
    return false;
}

bool BasePartWidget::getDeviceClosedState()
{
    return false;
}

bool BasePartWidget::getDeviceOpeningState()
{
    return false;
}

bool BasePartWidget::getDeviceClosingState()
{
    return false;
}

bool BasePartWidget::getDeviceFaultState()
{
    return false;
}

bool BasePartWidget::getEnable()
{
    return false;
}
