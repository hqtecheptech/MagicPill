#include "alerthistorydialog.h"
#include "ui_alerthistorydialog.h"

AlertHistoryDialog::AlertHistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertHistoryDialog)
{
    ui->setupUi(this);
}

AlertHistoryDialog::~AlertHistoryDialog()
{
    delete ui;
}

void AlertHistoryDialog::on_closePushButton_clicked()
{
    close();
}
