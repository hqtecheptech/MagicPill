#ifndef FERCASE_H
#define FERCASE_H

#include <QObject>

class FerCase : public QObject
{
    Q_OBJECT
public:
    explicit FerCase(QObject *parent = 0);

    float highTempture() const;
    void setHighTempture(float highTempture);

    float lowTempture() const;
    void setLowTempture(float lowTempture);

    int ae() const;
    void setAe(int ae);

    int sta() const;
    void setSta(int sta);

signals:

public slots:

private:
    float _lowTempture;
    float _highTempture;
    int _ae;
    int _sta;
};

#endif // FERCASE_H
