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

private slots:
    void on_pushButton_clicked();

private:
    Ui::AlertQueryDialog *ui;
};

#endif // ALERTQUERYDIALOG_H
