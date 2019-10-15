#ifndef FERCASE_H
#define FERCASE_H

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

private:
    float _lowTempture = -20.6;
    float _highTempture = 36.6;
    int _ae = 180;
    int _sta = 1200;
};

#endif // FERCASE_H
