#include "alerthistoryform.h"
#include "ui_alerthistoryform.h"
#include "global.h"

AlertHistoryForm::AlertHistoryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlertHistoryForm)
{
    ui->setupUi(this);
    ui->tableView->setModel(UiGlobal::simpleAlertsModel);
    ui->tableView->horizontalHeader()->setVisible(false);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setColumnWidth(0, 430);
}

AlertHistoryForm::~AlertHistoryForm()
{
    delete ui;
}
