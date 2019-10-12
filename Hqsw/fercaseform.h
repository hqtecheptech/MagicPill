#ifndef FERCASEFORM_H
#define FERCASEFORM_H

#include <QWidget>
#include "fercase.h"

namespace Ui {
class FerCaseForm;
}

class FerCaseForm : public QWidget
{
    Q_OBJECT

public:
    explicit FerCaseForm(QWidget *parent = 0);
    ~FerCaseForm();

private:
    Ui::FerCaseForm *ui;
    FerCase fercase;
};

#endif // FERCASEFORM_H
