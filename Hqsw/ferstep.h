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

    void removeLastCase();
    void appendCase(FerCase *ferCase);

    QString checkStepValidation();

    int defaultParaFEQ() const;
    void setDefaultParaFEQ(int defaultParaFEQ);

signals:

public slots:

private:
    int _planNum = 0;
    int _airMode = 0;
    int _nextStepTimeMax = 0;
    int _nextStepTimeMin = 0;
    int _defaultParaAE = 0;
    int _defaultParaSTA = 0;
    int _defaultParaFEQ = 0;
    float _nextStepTemp = 0.0;
    float _hopeTemp = 0.0;
    QList<FerCase*> _ferCases;
};

#endif // FERSTEP_H
