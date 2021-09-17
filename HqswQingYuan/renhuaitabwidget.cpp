#include "renhuaitabwidget.h"
#include "ui_renhuaitabwidget.h"

#include "renhuaifercontroltabpagewidget.h"
#include "xinyifercontroltabwidget.h"

RenhuaiTabWidget::RenhuaiTabWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::RenhuaiTabWidget)
{
    ui->setupUi(this);

    //ferControlTabPageWidget = new RenhuaiFerControlTabPageWidget(this);
    ferControlTabPageWidget = new XinyiFerControlTabWidget(this);
    //deoControlTabPageWidget = new DeoControlTabPageWidget(this);
    ui->fer_tab_horizontalLayout->addWidget(ferControlTabPageWidget);
    //ui->deo_tab_horizontalLayout->addWidget(deoControlTabPageWidget);

    connect(ferControlTabPageWidget, SIGNAL(alertReady(QString)),this,SLOT(alertUpdate(QString)));
    connect(ferControlTabPageWidget, SIGNAL(dataChanged()), this, SLOT(handleDataChange()));
    connect(ferControlTabPageWidget, SIGNAL(serverConnectionChanged(bool)), this, SLOT(handleServerConnectionChanged(bool)));
    connect(ferControlTabPageWidget, SIGNAL(dataUpdate(QSet<int>, QMap<float,QString>)), this, SLOT(handleFerDataUpdate(QSet<int>, QMap<float,QString>)));
    //connect(deoControlTabPageWidget, SIGNAL(dataUpdate(QSet<int>, QMap<float,QString>)), this, SLOT(handleDeoDataUpdate(QSet<int>, QMap<float,QString>)));

    setStyleSheet("QTabBar::tab{background: transparent;}");
}

RenhuaiTabWidget::~RenhuaiTabWidget()
{
    delete ui;
}

void RenhuaiTabWidget::handleDataChange()
{
    emit dataChanged();
}

void RenhuaiTabWidget::handleServerConnectionChanged(bool isServerConnected)
{
    emit serverConnectionChanged(isServerConnected);
}

void RenhuaiTabWidget::handleFerDataUpdate(QSet<int> changedDeviceSet, QMap<float,QString> dataMap)
{
    emit ferDataUpdate(changedDeviceSet, dataMap);
}

void RenhuaiTabWidget::handleDeoDataUpdate(QSet<int> changedDeviceSet, QMap<float,QString> dataMap)
{
    emit deoDataUpdate(changedDeviceSet, dataMap);
}

void RenhuaiTabWidget::alertUpdate(QString alert)
{
    emit alertReceived(alert);
}
