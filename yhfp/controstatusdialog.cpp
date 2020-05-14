#include "controstatusdialog.h"
#include "ui_controstatusdialog.h"

#include "global.h"

ControStatusDialog::ControStatusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControStatusDialog)
{
    ui->setupUi(this);

    controlStatusModel = new QStandardItemModel(Global::mixDeviceInfo.Runctr_Num,3);
    controlStatusModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("序号"));
    controlStatusModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("控制点名称"));
    controlStatusModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("当前值"));
    ui->tableView->setModel(controlStatusModel);
    ui->tableView->setColumnWidth(0, 50);
    ui->tableView->setColumnWidth(1, 200);
    ui->tableView->setColumnWidth(2, 50);
}

ControStatusDialog::~ControStatusDialog()
{
    delete ui;
}

void ControStatusDialog::showEvent(QShowEvent *event)
{

}

void ControStatusDialog::closeEvent(QCloseEvent *event)
{

}

void ControStatusDialog::handlePlcDataUpdate(QSet<int> changedDeviceSet, QMap<float, QString> dataMap)
{
    controlStatusModel->removeRows(0,controlStatusModel->rowCount(QModelIndex()),QModelIndex());

    uint row = 0;

    bool bValue = false;
    int iValue = 0;
    QVector<DeviceNode> nodes = Global::mixDeviceNodes;
    foreach (DeviceNode node, nodes) {
        if(node.DataType == "x0")
        {
            controlStatusModel->insertRows(row,1,QModelIndex());
            controlStatusModel->setData(controlStatusModel->index(row,0,QModelIndex()),node.Offset);
            controlStatusModel->setData(controlStatusModel->index(row,1,QModelIndex()),node.Cname);
            bValue = Global::getMixRunctrValueByName(0, node.Name, dataMap);
            if(bValue)
            {
                iValue = 1;
            }
            else
            {
                iValue = 0;
            }
            controlStatusModel->setData(controlStatusModel->index(row,2,QModelIndex()),iValue);

            row++;
        }
    }
}
