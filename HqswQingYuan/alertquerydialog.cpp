#include "alertquerydialog.h"
#include "ui_alertquerydialog.h"
#include "global.h"

AlertQueryDialog::AlertQueryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertQueryDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("报警查询"));
}

AlertQueryDialog::~AlertQueryDialog()
{
    delete ui;
}

void AlertQueryDialog::showEvent(QShowEvent *)
{
    ui->alertTableView->setModel(alertsModel);
    ui->alertTableView->setColumnWidth(0, 200);
    ui->alertTableView->sortByColumn(0,Qt::DescendingOrder);
}

void AlertQueryDialog::updateAlerts(QString alert)
{
    if(alertsModel->rowCount() > 200)
    {
        alertsModel->removeRow(alertsModel->rowCount()-1);
    }

    QStringList strList = alert.split(";");

    QList<QStandardItem *> newItemList;
    newItemList.append(new QStandardItem(strList[0]));
    newItemList.append(new QStandardItem(strList[1]));
    newItemList.append(new QStandardItem(strList[2]));
    if(strList.length() == 4)
    {
        newItemList.append(new QStandardItem(strList[3]));
    }
    else
    {
        newItemList.append(new QStandardItem(" "));
    }

    alertsModel->insertRow(0, newItemList);
}

void AlertQueryDialog::on_pushButton_clicked()
{
    close();
}
