#ifndef FERCASE_H
#define FERCASE_H

#include <QString>
#include <global.h>

class FerCase
{
public:
    explicit FerCase();

    float highTempture() const;
    void setHighTempture(float highTempture);

    float lowTempture() const;
    void setLowTempture(float lowTempture);

    int ae() const;
    void setAe(int ae);

    int sta() const;
    void setSta(int sta);

    QString checkCaseValidation(int caseSeq);

    int freq() const;
    void setFreq(int freq);

private:
    float _lowTempture = 0.0;
    float _highTempture = 0.0;
    int _ae = 0;
    int _sta = 0;
    int _freq = 0;
};

#endif // FERCASE_H
