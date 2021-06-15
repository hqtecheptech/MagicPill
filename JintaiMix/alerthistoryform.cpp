#include "alerthistoryform.h"
#include "ui_alerthistoryform.h"

#include "uiglobal.h"

AlertHistoryForm::AlertHistoryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlertHistoryForm)
{
    ui->setupUi(this);

    ui->hisTableView->setModel(UiGlobal::alertsModel);
    ui->hisTableView->horizontalHeader()->setVisible(false);
    ui->hisTableView->verticalHeader()->setVisible(false);
    ui->hisTableView->setShowGrid(false);
    ui->hisTableView->setColumnWidth(0, 180);
    ui->hisTableView->setColumnWidth(1, 20);
    ui->hisTableView->setColumnWidth(2, 300);
    ui->hisTableView->setColumnWidth(3, 50);
}

AlertHistoryForm::~AlertHistoryForm()
{
    delete ui;
}
