#include "ferstep.h"

FerStep::FerStep(QObject *parent) : QObject(parent)
{

}

int FerStep::planNum() const
{
    return _planNum;
}

void FerStep::setPlanNum(int planNum)
{
    _planNum = planNum;
}

int FerStep::airMode() const
{
    return _airMode;
}

void FerStep::setAirMode(int airMode)
{
    _airMode = airMode;
}

int FerStep::nextStepTimeMax() const
{
    return _nextStepTimeMax;
}

void FerStep::setNextStepTimeMax(int nextStepTimeMax)
{
    _nextStepTimeMax = nextStepTimeMax;
}

int FerStep::nextStepTimeMin() const
{
    return _nextStepTimeMin;
}

void FerStep::setNextStepTimeMin(int nextStepTimeMin)
{
    _nextStepTimeMin = nextStepTimeMin;
}

int FerStep::defaultParaAE() const
{
    return _defaultParaAE;
}

void FerStep::setDefaultParaAE(int defaultParaAE)
{
    _defaultParaAE = defaultParaAE;
}

int FerStep::defaultParaSTA() const
{
    return _defaultParaSTA;
}

void FerStep::setDefaultParaSTA(int defaultParaSTA)
{
    _defaultParaSTA = defaultParaSTA;
}

float FerStep::nextStepTemp() const
{
    return _nextStepTemp;
}

void FerStep::setNextStepTemp(float nextStepTemp)
{
    _nextStepTemp = nextStepTemp;
}

float FerStep::hopeTemp() const
{
    return _hopeTemp;
}

void FerStep::setHopeTemp(float hopeTemp)
{
    _hopeTemp = hopeTemp;
}

QList<FerCase *> FerStep::ferCases() const
{
    return _ferCases;
}

void FerStep::setFerCases(const QList<FerCase *> &ferCases)
{
    _ferCases = ferCases;
}
