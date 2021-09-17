#ifndef ALERTQUERYDIALOG_H
#define ALERTQUERYDIALOG_H

#include <QDialog>
#include "uiglobal.h"

namespace Ui {
class AlertQueryDialog;
}

class AlertQueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlertQueryDialog(QWidget *parent = 0);
    ~AlertQueryDialog();

protected:
    void showEvent(QShowEvent *);

public slots:
    void updateAlerts(QString alert);

private slots:
    void on_pushButton_clicked();

private:
    Ui::AlertQueryDialog *ui;

    QStandardItemModel* alertsModel = new QStandardItemModel(0,4);
};

#endif // ALERTQUERYDIALOG_H
