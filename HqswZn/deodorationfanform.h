#ifndef DEODORATIONFANFORM_H
#define DEODORATIONFANFORM_H

#include <QWidget>
#include <QPixmap>
#include <QSize>

#include "rotatefan.h"
#include "tankairvalveform.h"

namespace Ui {
class DeodorationFanForm;
}

class DeodorationFanForm : public QWidget
{
    Q_OBJECT

public:
    explicit DeodorationFanForm(QWidget *parent = 0);
    ~DeodorationFanForm();

    TankAirValveForm *valveFront;
    TankAirValveForm *valveBack;

private:
    Ui::DeodorationFanForm *ui;

    QPixmap LixinfanBgImg;
    RotateFan *rotateFan;
};

#endif // DEODORATIONFANFORM_H
