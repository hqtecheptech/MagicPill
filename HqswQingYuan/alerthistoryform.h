#ifndef ALERTHISTORYFORM_H
#define ALERTHISTORYFORM_H

#include <QWidget>
#include <QStandardItemModel>
#include "uiglobal.h"

namespace Ui {
class AlertHistoryForm;
}

class AlertHistoryForm : public QWidget
{
    Q_OBJECT

public:
    explicit AlertHistoryForm(QWidget *parent = 0);
    ~AlertHistoryForm();

public slots:
    void addAlertTable(QString alert);

private:
    Ui::AlertHistoryForm *ui;

    QStandardItemModel* simpleAlertsModel = new QStandardItemModel(0,1);
};

#endif // ALERTHISTORYFORM_H
