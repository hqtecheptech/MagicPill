#include "environmentparams.h"

EnvironmentParams::EnvironmentParams()
{

}

float EnvironmentParams::getTemperature() const
{
    return temperature;
}

void EnvironmentParams::setTemperature(float value)
{
    temperature = value;
}

float EnvironmentParams::getHumidity() const
{
    return humidity;
}

void EnvironmentParams::setHumidity(float value)
{
    humidity = value;
}

float EnvironmentParams::getH2s() const
{
    return h2s;
}

void EnvironmentParams::setH2s(float value)
{
    h2s = value;
}

float EnvironmentParams::getNh3() const
{
    return nh3;
}

void EnvironmentParams::setNh3(float value)
{
    nh3 = value;
}

float EnvironmentParams::getCurrA() const
{
    return currA;
}

void EnvironmentParams::setCurrA(float value)
{
    currA = value;
}
