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

private:
    Ui::YlzsPlate *ui;
};

#endif // YLZSPLATE_H
