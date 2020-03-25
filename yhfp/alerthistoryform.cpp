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
    ui->hisTableView->setColumnWidth(0, 200);
}

AlertHistoryForm::~AlertHistoryForm()
{
    delete ui;
}
