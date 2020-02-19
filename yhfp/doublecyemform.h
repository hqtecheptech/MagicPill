#ifndef DOUBLECYEMFORM_H
#define DOUBLECYEMFORM_H

#include <QWidget>

namespace Ui {
class DoubleCyEmForm;
}

class DoubleCyEmForm : public QWidget
{
    Q_OBJECT

public:
    explicit DoubleCyEmForm(QWidget *parent = 0);
    ~DoubleCyEmForm();

private:
    Ui::DoubleCyEmForm *ui;
};

#endif // DOUBLECYEMFORM_H
