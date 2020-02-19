#include "doubleemform.h"
#include "ui_doubleemform.h"

DoubleEmForm::DoubleEmForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoubleEmForm)
{
    ui->setupUi(this);
}

DoubleEmForm::~DoubleEmForm()
{
    delete ui;
}
