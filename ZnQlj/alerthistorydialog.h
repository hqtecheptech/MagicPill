#ifndef ALERTHISTORYDIALOG_H
#define ALERTHISTORYDIALOG_H

#include <QDialog>

namespace Ui {
class AlertHistoryDialog;
}

class AlertHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlertHistoryDialog(QWidget *parent = 0);
    ~AlertHistoryDialog();

private slots:
    void on_closePushButton_clicked();

public slots:
    void updateAlertForm(QString alert);

signals:
    void alertReceived(QString alert);

private:
    Ui::AlertHistoryDialog *ui;
};

#endif // ALERTHISTORYDIALOG_H