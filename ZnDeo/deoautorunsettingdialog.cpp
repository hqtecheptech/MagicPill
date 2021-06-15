#include "deoautorunsettingdialog.h"
#include "ui_deoautorunsettingdialog.h"

#include "global.h"
#include <QLineEdit>
#include <QScrollBar>

DeoAutoRunSettingDialog::DeoAutoRunSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeoAutoRunSettingDialog)
{
    ui->setupUi(this);

    this->installEventFilter(this);

    ui->save_pushButton->setStyleSheet("QPushButton {font: 11px;}");
    ui->close_pushButton->setStyleSheet("QPushButton {font: 11px;}");

    settingModel = new QStandardItemModel(Global::deoDeviceInfo.Deo_Setting_Category,2);
    connect(settingModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(updateRateSetting(QStandardItem*)));
    ui->tableView->setModel(settingModel);
    ui->tableView->setColumnWidth(0, 480);
    ui->tableView->setColumnWidth(1, 130);
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

    msgBox = new QMessageBox();
    msgBox->installEventFilter(this);
    msgBox->setStyleSheet(
            "QPushButton {"
                " background-color:#89AFDE;"
                " border-style: outset;"
                " border-width: 2px;"
                " border-radius: 10px;"
                " border-color: beige;"
                " font: bold 24px;"
                " min-width: 5em;"
                " min-height: 5em;"
            "}"
            "QLabel { min-width: 20em;min-height:10em;font:24px;background-color:#89AFDE;padding:10px;}"
        );

    inputDelegate = new RuntimeSettingDelegate(Others);
    ui->tableView->setItemDelegateForColumn(1,inputDelegate);
    connect(inputDelegate, SIGNAL(inputInfo(QString)),this,SLOT(showEditorInfo(QString)));

    saveSettingTcpClient = new TcpClientSocket(this);
    connect(saveSettingTcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showSaveSetting(QByteArray)));
}

DeoAutoRunSettingDialog::~DeoAutoRunSettingDialog()
{
    delete ui;
}

void DeoAutoRunSettingDialog::closeMsgBox()
{
    msgBox->close();
}

void DeoAutoRunSettingDialog::showEvent(QShowEvent *)
{
    showRateSetting();
}

void DeoAutoRunSettingDialog::showRateSetting()
{
    settingModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("参数名称"));
    settingModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("设定值"));
    settingModel->removeRows(0,settingModel->rowCount(QModelIndex()),QModelIndex());

    int row = 0;
    int index = 0;
    settingRowIndex.clear();
    settingAddress.clear();
    settingValues.clear();

    QVector<DeviceNode> nodes = Global::deoDeviceNodes;
    uint startAddress = Global::deoDeviceInfo.Deo_Setting_Address;
    uint settingNum = Global::deoDeviceInfo.Deo_Setting_Num;
    foreach (DeviceNode node, nodes) {
        uint dataItemLen = Global::getLengthByDataType(node.DataType);
        if(node.Offset >= startAddress && node.Offset < (startAddress + (settingNum * dataItemLen)))
        {
            index = (node.Offset - node.StartAddress)
                    / (Global::getLengthByDataType(node.DataType) * Global::deoDeviceInfo.Device_Number);

            settingAddress.append(node.Offset);
            settingValues.append(Global::currentDeodorationDataMap[node.Offset].toUInt());

            settingRowIndex.append(index);
            settingModel->insertRows(row,1,QModelIndex());
            settingModel->setData(settingModel->index(row,0,QModelIndex()),node.Cname);
            settingModel->setData(settingModel->index(row,1,QModelIndex()),Global::currentDeodorationDataMap[node.Offset]);
            settingModel->item(row,0)->setTextAlignment(Qt::AlignCenter);
            settingModel->item(row,1)->setTextAlignment(Qt::AlignCenter);
            settingModel->item(row,0)->setFlags(settingModel->item(row,0)->flags() & ~Qt::ItemIsEditable);
            settingModel->item(row,0)->setFont(QFont("Timers" , 10 ,  QFont::Normal));
            settingModel->item(row,1)->setFont(QFont("Timers" , 20 ,  QFont::Bold));
            ui->tableView->setRowHeight(row, 50);
            row++;
        }
    }

    settingInitialized = true;
}

void DeoAutoRunSettingDialog::updateRateSetting(QStandardItem *item)
{
    if(settingInitialized)
    {
        inputDelegate->setCurrentEditor(NULL);
        inputDelegate->setCurrentRow(-1);
        uint newValue = item->data(Qt::EditRole).toUInt();
        if(item->column() == 1)
        {
            settingValues[item->row()] = newValue;
        }
    }
}

void DeoAutoRunSettingDialog::showSaveSetting(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {
        msgBox->setText(QStringLiteral("设置更新成功！"));
        msgBox->show();
    }
}

void DeoAutoRunSettingDialog::showModelValue(QModelIndex index)
{
    uint value = settingModel->data(index).toUInt();
    qDebug() << "model value" << value;
}

void DeoAutoRunSettingDialog::showEditorInfo(QString info)
{
    msgBox->setText(info);
    msgBox->exec();
}

bool DeoAutoRunSettingDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == msgBox && event->type() == QEvent::WindowDeactivate)
    {
        msgBox->close();
    }

    return false;
}

void DeoAutoRunSettingDialog::on_close_pushButton_clicked()
{
    hide();
}

void DeoAutoRunSettingDialog::on_save_pushButton_clicked()
{
    QWidget* currentDelegateEditor = inputDelegate->getCurrentEditor();
    int currentRow = inputDelegate->getCurrentRow();
    if(currentDelegateEditor != NULL)
    {
        QString valueStr = static_cast<QLineEdit*>(currentDelegateEditor)->text();
        bool ok;
        uint newValue = valueStr.toUInt(&ok);
        if(ok)
        {
            settingValues[currentRow] = newValue;
        }
    }

    StreamPack bpack;
    bpack = {sizeof(StreamPack),2,0,W_Send_Control,UInt,0,0,1,0,0,0};
    //Length of ushort address and value, plus length of scrc.
    bpack.bDataLength = settingValues.length();
    bpack.bStreamLength += (2+Global::getLengthByDataType("dw"))*bpack.bDataLength + 4;

    QList<ushort> addrs;
    QList<uint> values;

    for(int i=0; i<settingValues.length();i++)
    {
        addrs.append(settingAddress[i]);
        values.append(settingValues[i]);
    }

    QByteArray allPackData, SData, crcData;
    QDataStream out(&SData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out.setByteOrder(QDataStream::LittleEndian);

    allPackData.append((char*)&bpack, sizeof(bpack));

    foreach(ushort item, addrs)
    {
        out << item;
    }

    foreach(uint item, values)
    {
        out << item;
    }

    SData.insert(0, allPackData);

    uint scrc = saveSettingTcpClient->StreamLen_CRC32(SData);

    QDataStream out1(&crcData,QIODevice::WriteOnly);
    out1.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out1.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out1.setByteOrder(QDataStream::LittleEndian);
    out1 << scrc;

    SData.append(crcData);

    saveSettingTcpClient->sendRequestWithResults(SData);
}
