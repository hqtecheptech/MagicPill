#include "alerthistoryform.h"
#include "ui_alerthistoryform.h"
#include "global.h"

AlertHistoryForm::AlertHistoryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlertHistoryForm)
{
    ui->setupUi(this);
    ui->tableView->setModel(simpleAlertsModel);
    ui->tableView->horizontalHeader()->setVisible(false);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setColumnWidth(0, 430);
}

AlertHistoryForm::~AlertHistoryForm()
{
    delete ui;
}

void AlertHistoryForm::addAlertTable(QString alert)
{
    if(simpleAlertsModel->rowCount() > 200)
    {
        simpleAlertsModel->removeRow(simpleAlertsModel->rowCount()-1);
    }
    QStandardItem * newSimpleItem =  new QStandardItem(alert);
    simpleAlertsModel->insertRow(0, newSimpleItem);
}
