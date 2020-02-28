#include "mixsettingdialog.h"
#include "ui_mixsettingdialog.h"

#include "global.h"
#include <QLineEdit>

MixSettingDialog::MixSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MixSettingDialog)
{
    ui->setupUi(this);
    //Avoid all windeows closed.
    //setAttribute(Qt::WA_QuitOnClose, false);

    rateSettingModel = new QStandardItemModel(Global::mixDeviceInfo.Rate_Setting_Category,2);
    connect(rateSettingModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(updateRateSetting(QStandardItem*)));
    ui->rate_tableView->setModel(rateSettingModel);
    ui->rate_tableView->setColumnWidth(0, 300);
    ui->rate_tableView->setColumnWidth(1, 100);
    ui->rate_tableView->horizontalHeader()
                        ->setStyleSheet(
                            "QHeaderView::section {"
                            "color: black;padding-left: 0px;border: 1px solid #6c6c6c;"
                            "font: 25 18pt;"
                            "height: 40px;}");

    rateDelegate = new RuntimeSettingDelegate(Rate);
    ui->rate_tableView->setItemDelegateForColumn(1,rateDelegate);

    saveSettingTcpClient = new TcpClientSocket(this);
    connect(saveSettingTcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showSaveSetting(QByteArray)));
}

MixSettingDialog::~MixSettingDialog()
{
    delete ui;
}

void MixSettingDialog::showEvent(QShowEvent *event)
{
    showRateSetting();
}

void MixSettingDialog::showRateSetting()
{
    rateSettingModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("参数名称"));
    rateSettingModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("转/秒"));
    rateSettingModel->removeRows(0,rateSettingModel->rowCount(QModelIndex()),QModelIndex());

    int row = 0;
    int index = 0;
    rateRowIndex.clear();
    rateSettingAddress.clear();
    rateSettingValues.clear();

    QVector<DeviceNode> nodes = Global::mixDeviceNodes;
    uint startAddress = Global::mixDeviceInfo.Rate_Setting_Address;
    uint settingNum = Global::mixDeviceInfo.Rate_Setting_Num;
    foreach (DeviceNode node, nodes) {
        uint dataItemLen = Global::getLengthByDataType(node.DataType);
        if(node.Offset >= startAddress && node.Offset < (startAddress + (settingNum * dataItemLen)))
        {
            index = (node.Offset - node.StartAddress)
                    / (Global::getLengthByDataType(node.DataType) * Global::mixDeviceInfo.Device_Number);

            rateSettingAddress.append(node.Offset);
            rateSettingValues.append(Global::currentMixDataMap[node.Offset].toUInt());

            rateRowIndex.append(index);
            rateSettingModel->insertRows(row,1,QModelIndex());
            rateSettingModel->setData(rateSettingModel->index(row,0,QModelIndex()),node.Cname);
            rateSettingModel->setData(rateSettingModel->index(row,1,QModelIndex()),Global::currentMixDataMap[node.Offset]);
            rateSettingModel->item(row,0)->setTextAlignment(Qt::AlignCenter);
            rateSettingModel->item(row,1)->setTextAlignment(Qt::AlignCenter);
            rateSettingModel->item(row,0)->setFont(QFont("Timers" , 20 ,  QFont::Normal));
            rateSettingModel->item(row,1)->setFont(QFont("Timers" , 35 ,  QFont::Bold));
            ui->rate_tableView->setRowHeight(row, 50);
            row++;
        }
    }

    rateSettingInitialized = true;
}

void MixSettingDialog::showSaveSetting(QByteArray data)
{

}

void MixSettingDialog::showModelValue(QModelIndex index)
{
    uint value = rateSettingModel->data(index).toUInt();
    qDebug() << "model value" << value;
}

void MixSettingDialog::on_close_pushButton_clicked()
{
    hide();
}

void MixSettingDialog::on_save_pushButton_clicked()
{
    QWidget* currentDelegateEditor = rateDelegate->getCurrentEditor();
    int currentRow = rateDelegate->getCurrentRow();
    if(currentDelegateEditor != NULL)
    {
        QString valueStr = static_cast<QLineEdit*>(currentDelegateEditor)->text();
        bool ok;
        uint newValue = valueStr.toUInt(&ok);
        if(ok)
        {
            rateSettingValues[currentRow] = newValue;
        }
    }
}

void MixSettingDialog::updateRateSetting(QStandardItem *item)
{
    if(rateSettingInitialized)
    {
        rateDelegate->setCurrentEditor(NULL);
        rateDelegate->setCurrentRow(-1);
        uint newValue = item->data(Qt::EditRole).toUInt();
        if(item->column() == 1)
        {
            rateSettingValues[item->row()] = newValue;
        }
    }
}
