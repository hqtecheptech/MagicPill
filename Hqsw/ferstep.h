#ifndef FERSTEP_H
#define FERSTEP_H

#include <QObject>
#include <QList>
#include "fercase.h"

class FerStep : public QObject
{
    Q_OBJECT
public:
    explicit FerStep(QObject *parent = 0);

    int planNum() const;
    void setPlanNum(int planNum);

    int airMode() const;
    void setAirMode(int airMode);

    int nextStepTimeMax() const;
    void setNextStepTimeMax(int nextStepTimeMax);

    int nextStepTimeMin() const;
    void setNextStepTimeMin(int nextStepTimeMin);

    int defaultParaAE() const;
    void setDefaultParaAE(int defaultParaAE);

    int defaultParaSTA() const;
    void setDefaultParaSTA(int defaultParaSTA);

    float nextStepTemp() const;
    void setNextStepTemp(float nextStepTemp);

    float hopeTemp() const;
    void setHopeTemp(float hopeTemp);

    QList<FerCase *> ferCases() const;
    void setFerCases(const QList<FerCase *> &ferCases);

signals:

public slots:

private:
    int _planNum = 3;
    int _airMode = 0;
    int _nextStepTimeMax = 24;
    int _nextStepTimeMin = 2;
    int _defaultParaAE = 300;
    int _defaultParaSTA = 1800;
    float _nextStepTemp = 32.6;
    float _hopeTemp = 32.6;
    QList<FerCase*> _ferCases;
};

#endif // FERSTEP_H
