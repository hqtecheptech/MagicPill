#ifndef CONTROSTATUSDIALOG_H
#define CONTROSTATUSDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class ControStatusDialog;
}

class ControStatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ControStatusDialog(QWidget *parent = 0);
    ~ControStatusDialog();

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

public slots:
    void handlePlcDataUpdate(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);

private:
    Ui::ControStatusDialog *ui;

    QStandardItemModel *controlStatusModel;
};

#endif // CONTROSTATUSDIALOG_H
