#ifndef ENVIRONMENTPARAMS_H
#define ENVIRONMENTPARAMS_H


class EnvironmentParams
{
public:
    EnvironmentParams();

    float getTemperature() const;
    void setTemperature(float value);

    float getHumidity() const;
    void setHumidity(float value);

    float getH2s() const;
    void setH2s(float value);

    float getNh3() const;
    void setNh3(float value);

    float getCurrA() const;
    void setCurrA(float value);

private:
    float temperature;
    float humidity;
    float h2s;
    float nh3;
    float currA;
};

#endif // ENVIRONMENTPARAMS_H
