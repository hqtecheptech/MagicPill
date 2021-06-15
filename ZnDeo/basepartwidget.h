#ifndef BASEPARTWIDGET_H
#define BASEPARTWIDGET_H

#include <QWidget>

class BasePartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BasePartWidget(QWidget *parent = 0);

    virtual void setTextIndexValue(const QString &strIndex);
    virtual void setPrefix(const QString &prefix);
    virtual void setName(const QString &name);
    virtual void setDeviceIndex(int index);
    virtual void setDeviceOpenedState(bool value);
    virtual void setDeviceClosedState(bool value);
    virtual void setDeviceOpeningState(bool value);
    virtual void setDeviceClosingState(bool value);
    virtual void setDeviceFaultState(bool value);
    virtual void setEnable(bool value);
    virtual QString getOpendSignalName();
    virtual QString getClosedSignalName();
    virtual QString getFaultSignalName();

    virtual QString getTextIndexValue();
    virtual QString getPrefix();
    virtual QString getName();
    virtual int getDeviceIndex();
    virtual bool getDeviceOpenedState();
    virtual bool getDeviceClosedState();
    virtual bool getDeviceOpeningState();
    virtual bool getDeviceClosingState();
    virtual bool getDeviceFaultState();
    virtual bool getEnable();

signals:

public slots:
};

#endif // BASEPARTWIDGET_H
