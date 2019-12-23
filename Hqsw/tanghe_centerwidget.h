#ifndef TANGHE_CENTERWIDGET_H
#define TANGHE_CENTERWIDGET_H

#include "dataformat.h"
#include "fftank.h"
#include "global.h"
#include "tankstatebarchart.h"
#include "environment.h"
#include "fercontroldialog.h"
#include "fancontroldialog.h"
#include "historydialog.h"
#include "requestworkthread.h"
#include "mytimerthread.h"
#include "reportdialog.h"
#include "logindialog.h"
#include "realtimedialog.h"
#include "settingdialog.h"
#include "alertquerydialog.h"
#include "deocontroldialog.h"
#include "uiglobal.h"

#include <QWidget>
#include <QSplitter>
#include <QStandardItemModel>
#include <QThread>
#include <QTimer>

namespace Ui {
class TangHe_CenterWidget;
}

class TangHe_CenterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TangHe_CenterWidget(QWidget *parent = 0);
    ~TangHe_CenterWidget();

    QVector<FFTank *> getTanks() const;

protected:
    void timerEvent(QTimerEvent *event);
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::TangHe_CenterWidget *ui;

    MyTimerThread *myTimerThread;
    QVector<FFTank*> tanks;
    TankStateBarChart* tankChart;
    FerControlDialog *fercontrolDialog;
    FanControlDialog *fancontrolDialog;
    HistoryDialog *historyDialog;
    ReportDialog *reportDialog;
    LoginDialog *loginDialog;
    SettingDialog *settingDialog;
    AlertQueryDialog *alertQueryDialog;
    DeoControlDialog *deoControlDialog;
    Environment* envWidget;
    QStandardItemModel *envModel;
    QDataWidgetMapper *envMapper = Q_NULLPTR;
    TcpClientSocket* getRunTimeTcpClient;
    TcpClientSocket* getFerTcpClient;
    TcpClientSocket* getStepTcpClient;
    TcpClientSocket* getRunCtrTcpClient;
    TcpClientSocket* getServerConnectStateTcpClient;
    TcpClientSocket* getAllPlcDataTcpClient;
    QPixmap logoBgImg;
    bool isServerConnected = false;

    void requestFermentationData(StreamPack requestPack);
    void requestRunTimeData(StreamPack requestPack);
    void requestStepData(StreamPack requestPack);
    void requestRunCtrData(StreamPack requestPack);

    void parseFermentationData(QMap<float,QString> dataMap);
    void parseRunTimeData(QMap<float,QString> dataMap);
    void parseStepData(QMap<float,QString> dataMap);
    void parseRunCtrData(QMap<float,QString> dataMap);

    quint64 readStartTime = 0;
    int readNumber = 0;

public slots:
    void showFermentationData(QByteArray);
    void showRunTimeData(QByteArray);
    void showStepData(QByteArray);
    void showRunCtrData(QByteArray);
    void showAllPlcData(QByteArray);

private slots:
    void read_tank_data();
    void on_fanControlButton_clicked();
    void on_ferControlButton_clicked();
    void localServerConnected(bool);
    void on_HistoryQueryButton_clicked();
    void on_ReportButton_clicked();
    void on_toolButton_2_clicked();
    void on_realTimeChartButton_clicked();
    void on_exitButton_clicked();
    void on_SettingButton_clicked();
    void on_AlertQueryButton_clicked();
    void on_deoControlButton_clicked();
};

#endif // TANGHE_CENTERWIDGET_H
