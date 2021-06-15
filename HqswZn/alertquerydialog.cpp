#include "alertquerydialog.h"
#include "ui_alertquerydialog.h"
#include "global.h"

AlertQueryDialog::AlertQueryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertQueryDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("报警查询"));

    ui->pushButton->setStyleSheet("QPushButton {font: 11px;}");
}

AlertQueryDialog::~AlertQueryDialog()
{
    delete ui;
}

void AlertQueryDialog::showEvent(QShowEvent *)
{
    ui->alertTableView->setModel(UiGlobal::alertsModel);
    ui->alertTableView->setColumnWidth(0, 200);
    ui->alertTableView->sortByColumn(0,Qt::DescendingOrder);
}

void AlertQueryDialog::on_pushButton_clicked()
{
    close();
}
