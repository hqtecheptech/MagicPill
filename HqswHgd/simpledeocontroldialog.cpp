#include "simpledeocontroldialog.h"
#include "ui_simpledeocontroldialog.h"

SimpleDeoControlDialog::SimpleDeoControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimpleDeoControlDialog)
{
    ui->setupUi(this);
}

SimpleDeoControlDialog::~SimpleDeoControlDialog()
{
    delete ui;
}
