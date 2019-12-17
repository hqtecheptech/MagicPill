#ifndef TANKAIRVALVEFORM_H
#define TANKAIRVALVEFORM_H

#include <QWidget>
#include <QPixmap>

namespace Ui {
class TankAirValveForm;
}

class TankAirValveForm : public QWidget
{
    Q_OBJECT

public:
    explicit TankAirValveForm(QWidget *parent = 0);
    ~TankAirValveForm();

private:
    Ui::TankAirValveForm *ui;

    QPixmap valveOpenBgImg;
    QPixmap valveCloseBgImg;
    QPixmap hPipeBgImg;
};

#endif // TANKAIRVALVEFORM_H
