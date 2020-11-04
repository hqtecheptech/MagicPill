#ifndef DEOSETTINGDIALOG_H
#define DEOSETTINGDIALOG_H

#include "tcpclientsocket.h"
#include "checkboxdelegate.h"

#include <QDialog>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDataStream>
#include <QByteArray>

namespace Ui {
class DeoSettingDialog;
}

class DeoSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeoSettingDialog(QWidget *parent = 0);
    ~DeoSettingDialog();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private:
    Ui::DeoSettingDialog *ui;

    TcpClientSocket *getDeoSettingTcpClient;
    TcpClientSocket *saveSettingTcpClient;
    QStandardItemModel *deoRtSettingModel;
    QVector<uint> deoRtSettingValues;
    QVector<uint> latestDeoRtSettingValues;
    QVector<uint> deoRtSettingAddress;
    QVector<int> rowIndex;
    QMessageBox msgBox;
    CheckBoxDelegate *cbxDelegate;
    bool deviceIndexInitialized = false;
    bool deoRtSettingInitialized = false;
    bool latestDeoRtSettingInitialized = false;
    void getDeoRtSetting();

private slots:
    void showDeoRtSetting(QByteArray data);
    void showSaveSetting(QByteArray data);
    void updateDeoRtSetting(QStandardItem *item);
    void on_saveSettingPushButton_clicked();
    void on_deviceIndexComboBox_currentIndexChanged(int index);
    void on_copySettingButton_clicked();
};

#endif // DEOSETTINGDIALOG_H
