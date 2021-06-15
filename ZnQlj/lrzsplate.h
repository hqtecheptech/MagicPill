#ifndef LRZSPLATE_H
#define LRZSPLATE_H

#include <QWidget>

namespace Ui {
class lrzsPlate;
}

class lrzsPlate : public QWidget
{
    Q_OBJECT

public:
    explicit lrzsPlate(QWidget *parent = 0);
    ~lrzsPlate();

private:
    Ui::lrzsPlate *ui;
};

#endif // LRZSPLATE_H
