#include "fercaseform.h"
#include "ui_fercaseform.h"

FerCaseForm::FerCaseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FerCaseForm)
{
    ui->setupUi(this);
}

FerCaseForm::~FerCaseForm()
{
    delete ui;
}
