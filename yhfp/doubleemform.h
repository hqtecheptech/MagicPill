#ifndef DOUBLEEMFORM_H
#define DOUBLEEMFORM_H

#include <QWidget>

namespace Ui {
class DoubleEmForm;
}

class DoubleEmForm : public QWidget
{
    Q_OBJECT

public:
    explicit DoubleEmForm(QWidget *parent = 0);
    ~DoubleEmForm();

private:
    Ui::DoubleEmForm *ui;
};

#endif // DOUBLEEMFORM_H
