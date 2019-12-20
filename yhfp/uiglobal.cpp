#include "uiglobal.h"

UiGlobal::UiGlobal()
{
    alertsModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("时间"));
    alertsModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("设备号"));
    alertsModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("事件"));
    alertsModel->setHeaderData(3,Qt::Horizontal,QStringLiteral("用户"));
    alertsModel->removeRows(0,alertsModel->rowCount(QModelIndex()),QModelIndex());
}

QStandardItemModel* Global::alertsModel = new QStandardItemModel(0,4);

QStandardItemModel* Global::simpleAlertsModel = new QStandardItemModel(0,1);

QStandardItemModel* Global::loginHistoryModel = new QStandardItemModel(0,1);
