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
