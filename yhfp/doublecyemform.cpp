#include "doublecyemform.h"
#include "ui_doublecyemform.h"

DoubleCyEmForm::DoubleCyEmForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoubleCyEmForm)
{
    ui->setupUi(this);
}

DoubleCyEmForm::~DoubleCyEmForm()
{
    delete ui;
}
