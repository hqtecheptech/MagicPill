#ifndef CONTROLLERSTATUSDIALOG_H
#define CONTROLLERSTATUSDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QTimer>

namespace Ui {
class ControllerStatusDialog;
}

class ControllerStatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ControllerStatusDialog(QWidget *parent = 0);
    ~ControllerStatusDialog();

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_comboBox_currentIndexChanged(int index);

public slots:
    void updatePlcData();

private:
    Ui::ControllerStatusDialog *ui;

    QStandardItemModel *controlStatusModel;
    QTimer *timer;
};

#endif // CONTROLLERSTATUSDIALOG_H
