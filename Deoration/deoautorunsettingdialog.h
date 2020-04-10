#ifndef DEOAUTORUNSETTINGDIALOG_H
#define DEOAUTORUNSETTINGDIALOG_H

#include <QDialog>
#include <tcpclientsocket.h>
#include <QStandardItemModel>
#include <QMessageBox>
#include "runtimesettingdelegate.h"

namespace Ui {
class DeoAutoRunSettingDialog;
}

class DeoAutoRunSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeoAutoRunSettingDialog(QWidget *parent = 0);
    ~DeoAutoRunSettingDialog();

    void closeMsgBox();

private:
    Ui::DeoAutoRunSettingDialog *ui;

    TcpClientSocket *saveSettingTcpClient;
    QStandardItemModel *settingModel;
    RuntimeSettingDelegate *inputDelegate;
    QVector<ushort> settingAddress;
    QVector<uint> settingValues;
    QVector<int> settingRowIndex;
    QMessageBox *msgBox;
    bool settingInitialized = false;

protected:
    void showEvent(QShowEvent *);
    bool eventFilter(QObject *, QEvent *);

private slots:
    void showRateSetting();
    void updateRateSetting(QStandardItem *item);
    void showSaveSetting(QByteArray data);
    void showModelValue(QModelIndex);
    void on_close_pushButton_clicked();
    void on_save_pushButton_clicked();

public slots:
    void showEditorInfo(QString info);
};

#endif // DEOAUTORUNSETTINGDIALOG_H
