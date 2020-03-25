#ifndef YLZSPLATE_H
#define YLZSPLATE_H

#include <QWidget>

namespace Ui {
class YlzsPlate;
}

class YlzsPlate : public QWidget
{
    Q_OBJECT

public:
    explicit YlzsPlate(QWidget *parent = 0);
    ~YlzsPlate();

public slots:
    void updatePlates(int leftValue, int rightValue);
    void setRange(int leftMin, int leftMax, int rightMin, int rightMax);

private:
    Ui::YlzsPlate *ui;

    int _leftValue=0, _rightValue=0;
    int _leftMin=0, _leftMax=0, _rightMin=0, _rightMax=0;
};

#endif // YLZSPLATE_H
