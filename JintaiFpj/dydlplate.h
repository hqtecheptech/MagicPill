#ifndef DYDLPLATE_H
#define DYDLPLATE_H

#include <QWidget>

namespace Ui {
class DydlPlate;
}

class DydlPlate : public QWidget
{
    Q_OBJECT

public:
    explicit DydlPlate(QWidget *parent = 0);
    ~DydlPlate();

public slots:
    void updatePlates(int leftValue, int rightValue);

private:
    Ui::DydlPlate *ui;

    int _leftValue=0, _rightValue=0;
};

#endif // DYDLPLATE_H
