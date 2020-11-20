#include "alertquerydialog.h"
#include "ui_alertquerydialog.h"
#include "global.h"

#include <QScrollBar>

AlertQueryDialog::AlertQueryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertQueryDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("报警查询"));

    ui->alertTableView->verticalScrollBar()
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

AlertQueryDialog::~AlertQueryDialog()
{
    delete ui;
}

void AlertQueryDialog::showEvent(QShowEvent *)
{
    ui->alertTableView->setModel(UiGlobal::alertsModel);
    ui->alertTableView->setColumnWidth(0, 200);
    ui->alertTableView->sortByColumn(0,Qt::DescendingOrder);
}

void AlertQueryDialog::on_pushButton_clicked()
{
    close();
}
