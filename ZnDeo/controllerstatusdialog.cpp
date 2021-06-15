#include "controllerstatusdialog.h"
#include "ui_controllerstatusdialog.h"

#include "global.h"
#include "QScrollBar"

ControllerStatusDialog::ControllerStatusDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ControllerStatusDialog)
{
    ui->setupUi(this);

    controlStatusModel = new QStandardItemModel(Global::deoDeviceInfo.Runctr_Num,3);
    controlStatusModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("序号"));
    controlStatusModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("名称"));
    controlStatusModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("当前值"));
    ui->tableView->setModel(controlStatusModel);
    ui->tableView->setColumnWidth(0, 50);
    ui->tableView->setColumnWidth(1, 350);
    ui->tableView->setColumnWidth(2, 150);

    for(int i=0; i<Global::deoDeviceInfo.Device_Number;i++)
    {
        ui->comboBox->addItem(QStringLiteral("%1#发酵槽").arg(QString::number(i+1)));
    }

    ui->comboBox->setVisible(false);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePlcData()));

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

ControllerStatusDialog::~ControllerStatusDialog()
{
    delete ui;
}

void ControllerStatusDialog::showEvent(QShowEvent *event)
{
    if(!timer->isActive())
    {
        timer->start(500);
    }
}

void ControllerStatusDialog::closeEvent(QCloseEvent *event)
{
    if(timer->isActive())
    {
        timer->stop();
    }
}

void ControllerStatusDialog::on_comboBox_currentIndexChanged(int index)
{

}

void ControllerStatusDialog::updatePlcData()
{
    controlStatusModel->removeRows(0,controlStatusModel->rowCount(QModelIndex()),QModelIndex());

    uint row = 0;

    bool bValue = false;
    int iValue = 0;
    QVector<DeviceNode> nodes = Global::deoDeviceNodes;
    foreach (DeviceNode node, nodes) {
        if(node.DataType == "x0" || node.DataType == "dw")
        {
            controlStatusModel->insertRows(row,1,QModelIndex());
            controlStatusModel->setData(controlStatusModel->index(row,0,QModelIndex()),node.Offset);
            controlStatusModel->setData(controlStatusModel->index(row,1,QModelIndex()),node.Cname);
            if(node.DataType == "x0")
            {
                bValue = Global::getDeoRunctrValueByName(ui->comboBox->currentIndex(), node.Name
                                    , Global::currentDeodorationDataMap);
                if(bValue)
                {
                    iValue = 1;
                }
                else
                {
                    iValue = 0;
                }
            }
            else
            {
                float address = node.Offset + ui->comboBox->currentIndex()
                        * Global::getLengthByDataType(node.DataType);
                iValue = (int)Global::currentDeodorationDataMap.value(address).toUInt();
            }
            controlStatusModel->setData(controlStatusModel->index(row,2,QModelIndex()),iValue);

            controlStatusModel->item(row,0)->setTextAlignment(Qt::AlignCenter);
            controlStatusModel->item(row,1)->setTextAlignment(Qt::AlignCenter);
            controlStatusModel->item(row,2)->setTextAlignment(Qt::AlignCenter);

            row++;
        }
    }
}
