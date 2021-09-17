#ifndef CONTROLHISTORY_H
#define CONTROLHISTORY_H

#include <QWidget>
#include "uiglobal.h"

namespace Ui {
class ControlHistory;
}

class ControlHistory : public QWidget
{
    Q_OBJECT

public:
    explicit ControlHistory(QWidget *parent = 0);
    ~ControlHistory();

public slots:
    void addAlertTable(QString alert);

private:
    Ui::ControlHistory *ui;

    QStandardItemModel* simpleAlertsModel = new QStandardItemModel(0,1);
};

#endif // CONTROLHISTORY_H
