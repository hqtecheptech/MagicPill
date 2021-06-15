#include "tankahead.h"
#include "ui_tankahead.h"

TankAhead::TankAhead(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TankAhead)
{
    ui->setupUi(this);
}

TankAhead::~TankAhead()
{
    delete ui;
}

void TankAhead::setIndex(int index)
{
    ui->indexLabel->setText(QString::number(index));
}
