#ifndef BASETANKWIDGET_H
#define BASETANKWIDGET_H

#include <QWidget>

class BaseTankWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseTankWidget(QWidget *parent = 0);

    virtual void setTextIndexValue(const QString &strIndex);
    virtual void setDeviceIndex(int index);
    virtual void setName(int index);
    virtual void setTankIndex(int tankIndex);

    virtual QString getTextIndexValue();
    virtual int getDeviceIndex();
    virtual int getName();
    virtual int getTankIndex() const;

signals:

public slots:
};

#endif // BASETANKWIDGET_H
