#include "alerthistoryform.h"
#include "ui_alerthistoryform.h"

#include "uiglobal.h"

AlertHistoryForm::AlertHistoryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlertHistoryForm)
{
    ui->setupUi(this);

    alertsModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("时间"));
    alertsModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("设备号"));
    alertsModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("事件"));
    alertsModel->setHeaderData(3,Qt::Horizontal,QStringLiteral("用户"));
    alertsModel->removeRows(0,alertsModel->rowCount(QModelIndex()),QModelIndex());

    ui->hisTableView->setModel(alertsModel);
    ui->hisTableView->horizontalHeader()->setVisible(false);
    ui->hisTableView->verticalHeader()->setVisible(false);
    ui->hisTableView->setShowGrid(false);
    ui->hisTableView->setColumnWidth(0, 200);
    ui->hisTableView->setColumnWidth(1, 100);
    ui->hisTableView->setColumnWidth(2, 300);
    ui->hisTableView->setColumnWidth(3, 100);
}

AlertHistoryForm::~AlertHistoryForm()
{
    delete ui;
}

void AlertHistoryForm::updateAlerts(QString alert)
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
