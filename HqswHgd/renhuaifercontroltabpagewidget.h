#ifndef RENHUAIFERCONTROLTABPAGEWIDGET_H
#define RENHUAIFERCONTROLTABPAGEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVector>

#include "ufftank.h"
#include "sharedfancontroldialog.h"
#include "uiglobal.h"

namespace Ui {
class RenhuaiFerControlTabPageWidget;
}

class RenhuaiFerControlTabPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RenhuaiFerControlTabPageWidget(QWidget *parent = 0);
    ~RenhuaiFerControlTabPageWidget();

    QVector<UFFTank *> getTanks() const;

protected:
    void timerEvent(QTimerEvent *event);
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::RenhuaiFerControlTabPageWidget *ui;

    QVector<UFFTank*> tanks;
    MyTimerThread *myTimerThread;

    SharedFanControlDialog *sfcDialog;

    QPixmap fanOpenBgImg;
    QPixmap fanCloseBgImg;

    QVector<QPushButton*> fanButtons;

    TcpClientSocket* getServerConnectStateTcpClient;
    TcpClientSocket* getAllFerDataTcpClient;
    TcpClientSocket* getAllFanDataTcpClient;
    bool isServerConnected = false;
    bool fanStateSwitchFlag = false;

    void parseFermentationData(QMap<float,QString> dataMap);
    void parseFerRunTimeData(QMap<float,QString> dataMap);
    void parseFerStepData(QMap<float,QString> dataMap);
    void parseFerRunCtrData(QMap<float,QString> dataMap);
    void parseFanRunCtrData(QMap<float,QString> dataMap);

    quint64 readStartTime = 0;
    int readNumber = 0;

public slots:
    void showFerData(QByteArray);
    void showFanData(QByteArray);
    void localServerConnected(bool);

signals:
    void dataChanged();
    void serverConnectionChanged(bool isConnected);
    void dataUpdate(QMap<float,QString> dataMap);

private slots:
    void read_server_data();
    void on_fanPushButton_1_clicked();
    void on_fanPushButton_2_clicked();
    void on_fanPushButton_3_clicked();
    void on_fanPushButton_4_clicked();
};

#endif // RENHUAIFERCONTROLTABPAGEWIDGET_H
