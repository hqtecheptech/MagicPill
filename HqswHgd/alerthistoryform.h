#ifndef ALERTHISTORYFORM_H
#define ALERTHISTORYFORM_H

#include <QWidget>

namespace Ui {
class AlertHistoryForm;
}

class AlertHistoryForm : public QWidget
{
    Q_OBJECT

public:
    explicit AlertHistoryForm(QWidget *parent = 0);
    ~AlertHistoryForm();

private:
    Ui::AlertHistoryForm *ui;
};

#endif // ALERTHISTORYFORM_H
