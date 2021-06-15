#ifndef TANKAHEAD_H
#define TANKAHEAD_H

#include <QWidget>

namespace Ui {
class TankAhead;
}

class TankAhead : public QWidget
{
    Q_OBJECT

public:
    explicit TankAhead(QWidget *parent = 0);
    ~TankAhead();

    void setIndex(int index);

private:
    Ui::TankAhead *ui;
};

#endif // TANKAHEAD_H
