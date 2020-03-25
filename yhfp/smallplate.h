#ifndef SMALLPLATE_H
#define SMALLPLATE_H

#include <QWidget>

namespace Ui {
class SmallPlate;
}

class SmallPlate : public QWidget
{
    Q_OBJECT

public:
    explicit SmallPlate(QWidget *parent = 0);
    ~SmallPlate();

    void setRange(int leftMin, int leftMax, int rightMin, int rightMax);

public slots:
    void updatePlates(int leftValue, int rightValue);

private:
    Ui::SmallPlate *ui;

    int _leftValue=0, _rightValue=0;
    int _leftMin=0, _leftMax=0, _rightMin=0, _rightMax=0;
};

#endif // SMALLPLATE_H
