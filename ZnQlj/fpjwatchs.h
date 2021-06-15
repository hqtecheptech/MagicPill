#ifndef FPJWATCHS_H
#define FPJWATCHS_H

#include <QWidget>

namespace Ui {
class FpjWatchs;
}

class FpjWatchs : public QWidget
{
    Q_OBJECT

public:
    explicit FpjWatchs(QWidget *parent = 0);
    ~FpjWatchs();

    void setLeftTopPlateRange(int leftMin, int leftMax, int rightMin, int rightMax);
    void setRightTopPlateRange(int leftMin, int leftMax, int rightMin, int rightMax);
    void setLeftBottomPlateRange(int leftMin, int leftMax, int rightMin, int rightMax);
    void setRightBottomPlateRange(int leftMin, int leftMax, int rightMin, int rightMax);
    void setLeftCenterPlateRange(int leftMin, int leftMax, int rightMin, int rightMax);
    void setRightCenterPlateRange(int leftMin, int leftMax, int rightMin, int rightMax);

public slots:
    void updateLeftTopPlate(int leftValue, int rightValue);
    void updateRightTopPlate(int leftValue, int rightValue);
    void updateLeftBottomPlate(int leftValue, int rightValue);
    void updateRightBottomPlate(int leftValue, int rightValue);
    void updateLeftCenterPlate(int leftValue, int rightValue);
    void updateRightCenterPlate(int leftValue, int rightValue);

private:
    Ui::FpjWatchs *ui;
};

#endif // FPJWATCHS_H
