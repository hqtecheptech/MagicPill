#ifndef ALERTHISTORYFORM_H
#define ALERTHISTORYFORM_H

#include <QWidget>
#include <QStandardItem>

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
    void updateAlerts(QString alert);

private:
    Ui::AlertHistoryForm *ui;

    QStandardItemModel* alertsModel = new QStandardItemModel(0,4);
};

#endif // ALERTHISTORYFORM_H
