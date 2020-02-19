#ifndef SINGLEEMFORM_H
#define SINGLEEMFORM_H

#include <QWidget>

namespace Ui {
class SingleEmForm;
}

class SingleEmForm : public QWidget
{
    Q_OBJECT

public:
    explicit SingleEmForm(QWidget *parent = 0);
    ~SingleEmForm();

private:
    Ui::SingleEmForm *ui;
};

#endif // SINGLEEMFORM_H
