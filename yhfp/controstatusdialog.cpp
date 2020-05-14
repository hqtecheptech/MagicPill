#include "controstatusdialog.h"
#include "ui_controstatusdialog.h"

#include "global.h"
#include <QScrollBar>

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

    ui->tableView->horizontalHeader()
                            ->setStyleSheet(
                                "QHeaderView::section {"
                                "color: black;padding-left: 0px;border: 1px solid #6c6c6c;"
                                "font: 25 18pt;"
                                "height: 40px;}");

    ui->tableView->verticalScrollBar()
                ->setStyleSheet("QScrollBar:vertical"
                                    "{"
                                    "width:50px;"
                                    "background:rgba(255,255,255,80%);"
                                    "margin:0px,0px,0px,0px;"
                                    "padding-top:9px;"
                                    "padding-bottom:9px;"
                                    "}"
                                    "QScrollBar::handle:vertical"
                                    "{"
                                    "width:50px;"
                                    "background:rgba(0,0,0,25%);"
                                    " border-radius:4px;"
                                    "min-height:20;"
                                    "}"
                                    "QScrollBar::handle:vertical:hover"
                                    "{"
                                    "width:50px;"
                                    "background:rgba(0,0,0,50%);"
                                    " border-radius:4px;"
                                    "min-height:20;"
                                    "}"
                                    "QScrollBar::add-line:vertical"
                                    "{"
                                    "height:9px;width:8px;"
                                    "border-image:url(:/images/a/3.png);"
                                    "subcontrol-position:bottom;"
                                    "}"
                                    "QScrollBar::sub-line:vertical"
                                    "{"
                                    "height:9px;width:8px;"
                                    "border-image:url(:/images/a/1.png);"
                                    "subcontrol-position:top;"
                                    "}"
                                    "QScrollBar::add-line:vertical:hover"
                                    "{"
                                    "height:9px;width:8px;"
                                    "border-image:url(:/images/a/4.png);"
                                    "subcontrol-position:bottom;"
                                    "}"
                                    "QScrollBar::sub-line:vertical:hover"
                                    "{"
                                    "height:9px;width:8px;"
                                    "border-image:url(:/images/a/2.png);"
                                    "subcontrol-position:top;"
                                    "}"
                                    "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                    "{"
                                    "background:rgba(0,0,0,10%);"
                                    "border-radius:4px;"
                                    "}");
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
