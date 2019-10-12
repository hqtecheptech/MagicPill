#include "ferconfig.h"

FerConfig::FerConfig(QObject *parent) : QObject(parent)
{

}

QList<FerStep *> FerConfig::getFerSteps() const
{
    return ferSteps;
}

void FerConfig::setFerSteps(const QList<FerStep *> &value)
{
    ferSteps = value;
}
