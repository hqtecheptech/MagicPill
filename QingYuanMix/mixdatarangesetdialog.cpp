#include "mixdatarangesetdialog.h"
#include "ui_mixdatarangesetdialog.h"

#include "global.h"
#include <QScrollBar>
#include <QLineEdit>

MixDataRangeSetDialog::MixDataRangeSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MixDataRangeSetDialog)
{
    ui->setupUi(this);

    this->installEventFilter(this);

    deviceNodeModel = new QStandardItemModel(Global::mixDeviceNodes.length() - Global::mixRunCtrDeviceNodes.length(),3);
    deviceNodeModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("控制点名称"));
    deviceNodeModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("正常值下限"));
    deviceNodeModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("正常值上限"));
    ui->range_tableView->setModel(deviceNodeModel);
    ui->range_tableView->setColumnWidth(0, 300);
    ui->range_tableView->setColumnWidth(1, 150);
    ui->range_tableView->setColumnWidth(2, 150);

    ui->range_tableView->horizontalHeader()
                        ->setStyleSheet(
                            "QHeaderView::section {"
                            "color: black;padding-left: 0px;border: 1px solid #6c6c6c;"
                            "font: 25 18pt;"
                            "height: 40px;}");

    ui->range_tableView->verticalScrollBar()
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

    leftRangeDelegate = new RuntimeSettingDelegate(Range);
    rightRangeDelegate = new RuntimeSettingDelegate(Range);
    ui->range_tableView->setItemDelegateForColumn(1,leftRangeDelegate);
    ui->range_tableView->setItemDelegateForColumn(2,rightRangeDelegate);
    connect(leftRangeDelegate, SIGNAL(inputInfo(QString)),this,SLOT(showEditorInfo(QString)));
    connect(leftRangeDelegate,SIGNAL(dataChanged(float,int)),this,SLOT(updateLeftRangeData(float,int)));
    connect(rightRangeDelegate, SIGNAL(inputInfo(QString)),this,SLOT(showEditorInfo(QString)));
    connect(rightRangeDelegate,SIGNAL(dataChanged(float,int)),this,SLOT(updateRightRangeData(float,int)));

    deviceNodeModel->removeRows(0,deviceNodeModel->rowCount(QModelIndex()),QModelIndex());

    int row = 0;
    foreach (DeviceNode node, Global::mixDeviceNodes) {
        if(node.DataType != "x0")
        {
            deviceNodeModel->insertRows(row,1,QModelIndex());
            deviceNodeModel->setData(deviceNodeModel->index(row,0,QModelIndex()),node.Cname);
            deviceNodeModel->setData(deviceNodeModel->index(row,1,QModelIndex()),node.leftRange);
            deviceNodeModel->setData(deviceNodeModel->index(row,2,QModelIndex()),node.rightRange);

            deviceNodeModel->item(row,0)->setTextAlignment(Qt::AlignCenter);
            deviceNodeModel->item(row,1)->setTextAlignment(Qt::AlignCenter);
            deviceNodeModel->item(row,2)->setTextAlignment(Qt::AlignCenter);
            deviceNodeModel->item(row,0)->setFont(QFont("Timers" , 20 ,  QFont::Normal));
            deviceNodeModel->item(row,1)->setFont(QFont("Timers" , 20 ,  QFont::Normal));
            deviceNodeModel->item(row,2)->setFont(QFont("Timers" , 20 ,  QFont::Bold));
            ui->range_tableView->setRowHeight(row, 50);

            row++;
        }
    }

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
}

MixDataRangeSetDialog::~MixDataRangeSetDialog()
{
    delete ui;
}

void MixDataRangeSetDialog::closeMsgBox()
{
    msgBox->close();
}

void MixDataRangeSetDialog::showEditorInfo(QString info)
{

}

bool MixDataRangeSetDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == msgBox && event->type() == QEvent::WindowDeactivate)
    {
        msgBox->close();
    }

    return false;
}

void MixDataRangeSetDialog::updateLeftRangeData(float value, int row)
{
    DeviceNode* head;
    head = Global::mixDeviceNodes.data();
    head = head + row;
    head->leftRange = value;
}

void MixDataRangeSetDialog::updateRightRangeData(float value, int row)
{
    DeviceNode* head;
    head = Global::mixDeviceNodes.data();
    head = head + row;
    head->rightRange = value;
}

void MixDataRangeSetDialog::on_close_pushButton_clicked()
{
    hide();
}

void MixDataRangeSetDialog::on_save_pushButton_clicked()
{
    DeviceNode* head;
    int currentRow = -1;
    float newValue = 0.0;
    QString valueStr;
    bool ok;

    QWidget* currentDelegateEditor = leftRangeDelegate->getCurrentEditor();
    if(currentDelegateEditor != NULL)
    {
        currentRow = leftRangeDelegate->getCurrentRow();
        valueStr = static_cast<QLineEdit*>(currentDelegateEditor)->text();
        newValue = valueStr.toFloat(&ok);
        if(ok)
        {
            head = Global::mixDeviceNodes.data();
            head = head + currentRow;
            head->leftRange = newValue;
        }
    }
    else
    {
        currentDelegateEditor = rightRangeDelegate->getCurrentEditor();
        if(currentDelegateEditor != NULL)
        {
            currentRow = rightRangeDelegate->getCurrentRow();
            valueStr = static_cast<QLineEdit*>(currentDelegateEditor)->text();
            newValue = valueStr.toFloat(&ok);
            if(ok)
            {
                head = Global::mixDeviceNodes.data();
                head = head + currentRow;
                head->rightRange = newValue;
            }
        }
    }

    Global::modifyMixNodeValueRange();
}
