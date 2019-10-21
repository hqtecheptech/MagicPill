#include "fercase.h"

FerCase::FerCase()
{

}

float FerCase::highTempture() const
{
    return _highTempture;
}

void FerCase::setHighTempture(float highTempture)
{
    _highTempture = highTempture;
}

float FerCase::lowTempture() const
{
    return _lowTempture;
}

void FerCase::setLowTempture(float lowTempture)
{
    _lowTempture = lowTempture;
}

int FerCase::ae() const
{
    return _ae;
}

void FerCase::setAe(int ae)
{
    _ae = ae;
}

int FerCase::sta() const
{
    return _sta;
}

void FerCase::setSta(int sta)
{
    _sta = sta;
}

QString FerCase::checkCaseValidation(int caseSeq)
{
    if(_lowTempture >= _highTempture)
    {
        return "高温必须大于低温";
    }

    if(_ae <= 0)
    {
        return "曝气时长必须大于0";
    }

    if(_sta <=0)
    {
       return "间隔时长必须大于0";
    }

    Global::ferConfigStrContent.append("case_" + QString::number(caseSeq+1) + " "
            + QString::number(_lowTempture) + " < x < " + QString::number(_highTempture) + " "
            + QString::number(_ae) + " " + QString::number(_sta));

    return "OK";
}
