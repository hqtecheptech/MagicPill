#ifndef RENHUAINEW_H
#define RENHUAINEW_H

#include "logindialog.h"
#include "fanvalvecontroldialog.h"
#include "sharedfancontroldialog.h"
#include "renhuaitabwidget.h"
#include "historydialog.h"
#include "reportdialog.h"
#include "alertquerydialog.h"
#include "settingdialog.h"
#include "fercontroldialog.h"
#include "environmentparams.h"
#include "fancontroldialog.h"
#include "sensorcontroldialog.h"
#include "ferconfigdialog.h"

#include <QWidget>
#include <QTimer>

namespace Ui {
class RenhuaiNew;
}

class RenhuaiNew : public QWidget
{
    Q_OBJECT

public:
    explicit RenhuaiNew(QWidget *parent = 0);
    ~RenhuaiNew();

protected:
    void timerEvent( QTimerEvent *event );
    int m_nTimerId;

private slots:
    void on_loginButton_clicked();
    void on_ferControlButton_clicked();
    void on_SettingButton_clicked();

    void on_logoutPushButton_clicked();

    void on_switchUserPushButton_clicked();

    void on_fanControlButton_clicked();

    void on_fanValveControlButton_clicked();

    void on_HistoryQueryButton_clicked();

    void on_realTimeChartButton_clicked();

    void on_ReportButton_clicked();

    void on_AlertQueryButton_clicked();

    void on_exitButton_clicked();

public slots:
    void updateLoginState();
    void updateDataChanged();
    void updateServerConnectionChanged(bool isServerConnected);
    void updateFerData(QSet<int>,QMap<float,QString>);
    void updateDeoData(QSet<int>,QMap<float,QString>);
    void updateAlert(QString alter);

signals:
    void ferDataChanged(QSet<int>, QMap<float,QString>);
    void deoDataChanged(QSet<int>, QMap<float,QString>);

private:
    Ui::RenhuaiNew *ui;

    RenhuaiTabWidget *tabWidget;
    SharedFanControlDialog *sfcontrolDialog;
    FerControlDialog *fercontrolDialog;
    LoginDialog *loginDialog;
    FanValveControlDialog *fanValveControlDialog;
    HistoryDialog *historyDialog;
    ReportDialog *reportDialog;
    //SettingDialog *settingDialog;
    FerConfigDialog *ferconfigDialog;
    AlertQueryDialog *alertQueryDialog;
    FanControlDialog *fanControlDialog;
    SensorControlDialog *sensorControlDialog;

    QVector<EnvironmentParams *> envParamsArray;
    QTimer *touchTimer;
};

#endif // RENHUAINEW_H
