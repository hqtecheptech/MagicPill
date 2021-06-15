#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include "runtimesettingdelegate.h"

#include <tcpclientsocket.h>
#include <QDialog>
#include <QStandardItemModel>
#include <QMessageBox>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();

protected:
    void showEvent(QShowEvent *);

private:
    Ui::SettingDialog *ui;
    TcpClientSocket *getAerationRtSettingTcpClient;
    TcpClientSocket *getFerTimeSettingTcpClient;
    TcpClientSocket *getFerTemptureSettingTcpClient;
    TcpClientSocket *getFerHzSettingTcpClient;
    TcpClientSocket *saveSettingTcpClient;
    QStandardItemModel *aerationRtSettingModel;
    QStandardItemModel *ferTimeSettingModel;
    QStandardItemModel *ferTemptureSettingModel;
    QStandardItemModel *ferHzSettingModel;
    RuntimeSettingDelegate *minuteDelegate;
    RuntimeSettingDelegate *secondDelegate;
    RuntimeSettingDelegate *dayDelegate;
    RuntimeSettingDelegate *hourDelegate;

    QVector<ushort> aerationRtSettingAddress;
    QVector<ushort> ferTimeRtSettingAddress;
    QVector<ushort> ferHzSettingRtSettingAddress;
    QVector<ushort> ferTemptureSettingAddress;

    QVector<uint> aerationRtSettingValues;
    QVector<uint> ferTimeSettingValues;
    QVector<uint> ferHzSettingValues;
    QVector<float> ferTemptureSettingValues;

    QVector<uint> latestAerationRtSettingValues;
    QVector<uint> latestFerTimeSettingValues;
    QVector<uint> latestFerHzSettingValues;
    QVector<float> latestFerTemptureSettingValues;

    QVector<int> aerationRowIndex;
    QVector<int> timeRowIndex;
    QVector<int> hzRowIndex;
    QVector<int> temptureRowIndex;

    QMessageBox msgBox;
    bool tankIndexInitialized = false;
    bool aerationRtSettingInitialized = false;
    bool ferTimeSettingInitialized = false;
    bool ferTemptureSettingInitialized = false;
    bool ferHzSettingInitialized = false;
    bool latestAerationRtSettingInitialized = false;
    bool latestFerTimeSettingInitialized = false;
    bool latestFerTemptureSettingInitialized = false;
    bool latestFerHzSettingInitialized = false;
    void getAerationRtSetting();
    void getFerTimeSetting();
    void getFerTemptureSetting();
    void getFerHzSetting();

private slots:
    void showAerationRtSetting(QByteArray data);
    void showFerTimeSetting(QByteArray data);
    void showFerTemptureSetting(QByteArray data);
    void showFerHzSetting(QByteArray data);
    void showSaveSetting(QByteArray data);
    void showModelValue(QModelIndex);
    void updateAerationRtSetting(QStandardItem *item);
    void updateFerTimeSetting(QStandardItem *item);
    void updateFerTemptureSetting(QStandardItem *item);
    void updateFerHzSetting(QStandardItem *item);

    void on_saveAerationRtSettingButton_clicked();
    void on_tankComboBox_currentIndexChanged(int index);
    void on_saveFerTimeSettingButton_clicked();
    void on_saveFerTemptureSettingButton_clicked();
    void on_copySettingButton_clicked();
    void on_saveFerHzSettingButton_clicked();
};

#endif // SETTINGDIALOG_H
