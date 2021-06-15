#ifndef TESTPART_H
#define TESTPART_H

#include <QWidget>

#include "basepartwidget.h"

namespace Ui {
class TestPart;
}

class TestPart : public BasePartWidget
{
    Q_OBJECT

public:
    explicit TestPart(QWidget *parent = 0);
    ~TestPart();

public:
    void setTextIndexValue(QString strIndex);
    void setPrefix(QString prefix);
    void setName(QString name);
    void setDeviceIndex(int index);
    void setDeviceOpenedState(bool value);
    void setDeviceClosedState(bool value);
    void setDeviceOpeningState(bool value);
    void setDeviceClosingState(bool value);
    void setDeviceFaultState(bool value);
    void setEnable(bool value);
    QString getOpendSignalName();
    QString getClosedSignalName();

private:
    Ui::TestPart *ui;
};

#endif // TESTPART_H
