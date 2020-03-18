#include "controlhistory.h"
#include "ui_controlhistory.h"
#include "global.h"

#include <QScrollBar>

ControlHistory::ControlHistory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlHistory)
{
    ui->setupUi(this);
    ui->hisTableView->setModel(UiGlobal::simpleAlertsModel);
    ui->hisTableView->horizontalHeader()->setVisible(false);
    ui->hisTableView->verticalHeader()->setVisible(false);
    ui->hisTableView->setShowGrid(false);
    ui->hisTableView->setColumnWidth(0, 685);

    ui->hisTableView->verticalScrollBar()
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

ControlHistory::~ControlHistory()
{
    delete ui;
}
