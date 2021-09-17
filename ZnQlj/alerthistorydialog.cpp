#include "alerthistorydialog.h"
#include "ui_alerthistorydialog.h"

AlertHistoryDialog::AlertHistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertHistoryDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

    connect(this, SIGNAL(alertReceived(QString)), ui->hisWidget, SLOT(updateAlerts(QString)));
}

AlertHistoryDialog::~AlertHistoryDialog()
{
    delete ui;
}

void AlertHistoryDialog::on_closePushButton_clicked()
{
    close();
}

void AlertHistoryDialog::updateAlertForm(QString alert)
{
    emit alertReceived(alert);
}
