#include "ferfanvalvecontroldialog.h"
#include "ui_ferfanvalvecontroldialog.h"

FerFanValveControlDialog::FerFanValveControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FerFanValveControlDialog)
{
    ui->setupUi(this);
}

FerFanValveControlDialog::~FerFanValveControlDialog()
{
    delete ui;
}
