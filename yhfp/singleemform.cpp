#include "singleemform.h"
#include "ui_singleemform.h"

SingleEmForm::SingleEmForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleEmForm)
{
    ui->setupUi(this);
}

SingleEmForm::~SingleEmForm()
{
    delete ui;
}
