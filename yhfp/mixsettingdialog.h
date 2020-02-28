#ifndef MIXSETTINGDIALOG_H
#define MIXSETTINGDIALOG_H

#include <QDialog>
#include <tcpclientsocket.h>
#include <QStandardItemModel>
#include <QMessageBox>
#include "runtimesettingdelegate.h"

namespace Ui {
class MixSettingDialog;
}

class MixSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MixSettingDialog(QWidget *parent = 0);
    ~MixSettingDialog();

    void setCurrentEditor(QWidget *value);

protected:
    void showEvent(QShowEvent *event);

private:
    Ui::MixSettingDialog *ui;

    TcpClientSocket *saveSettingTcpClient;
    QStandardItemModel *rateSettingModel;
    RuntimeSettingDelegate *rateDelegate;
    QVector<ushort> rateSettingAddress;
    QVector<uint> rateSettingValues;
    QVector<int> rateRowIndex;
    QMessageBox msgBox;
    bool rateSettingInitialized = false;

private slots:
    void showRateSetting();
    void showSaveSetting(QByteArray data);
    void showModelValue(QModelIndex);
    void on_close_pushButton_clicked();
    void on_save_pushButton_clicked();
    void updateRateSetting(QStandardItem *item);
};

#endif // MIXSETTINGDIALOG_H
