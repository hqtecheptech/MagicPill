#include "controlhistory.h"
#include "ui_controlhistory.h"
#include "global.h"

ControlHistory::ControlHistory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlHistory)
{
    ui->setupUi(this);
    ui->hisTableView->setModel(Global::simpleAlertsModel);
    ui->hisTableView->horizontalHeader()->setVisible(false);
    ui->hisTableView->verticalHeader()->setVisible(false);
    ui->hisTableView->setShowGrid(false);
    ui->hisTableView->setColumnWidth(0, 685);
}

ControlHistory::~ControlHistory()
{
    delete ui;
}
