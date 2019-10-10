#include "loginhistoryform.h"
#include "ui_loginhistoryform.h"
#include "global.h"

loginHistoryForm::loginHistoryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginHistoryForm)
{
    ui->setupUi(this);
    ui->tableView->setModel(Global::loginHistoryModel);
    ui->tableView->horizontalHeader()->setVisible(false);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setColumnWidth(0, 430);
}

loginHistoryForm::~loginHistoryForm()
{
    delete ui;
}
