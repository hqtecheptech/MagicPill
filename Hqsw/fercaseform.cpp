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

FerCase FerCaseForm::fercase() const
{
    return _fercase;
}

void FerCaseForm::setFercase(const FerCase &fercase)
{
    _fercase = fercase;
}
