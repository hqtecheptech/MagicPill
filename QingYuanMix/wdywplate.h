#ifndef WDYWPLATE_H
#define WDYWPLATE_H

#include <QWidget>

namespace Ui {
class WdywPlate;
}

class WdywPlate : public QWidget
{
    Q_OBJECT

public:
    explicit WdywPlate(QWidget *parent = 0);
    ~WdywPlate();

public slots:
    void updatePlates(int leftValue, int rightValue);

private:
    Ui::WdywPlate *ui;

    int _leftValue=0, _rightValue=0;
};

#endif // WDYWPLATE_H
