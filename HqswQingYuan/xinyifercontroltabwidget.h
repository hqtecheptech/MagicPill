#ifndef XINYIFERCONTROLTABWIDGET_H
#define XINYIFERCONTROLTABWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVector>
#include <QThread>
#include <QVBoxLayout>

#include "basetankwidget.h"
#include "fftank.h"
#include "parseserverdataworker.h"
#include "uiglobal.h"

namespace Ui {
class XinyiFerControlTabWidget;
}

class XinyiFerControlTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit XinyiFerControlTabWidget(QWidget *parent = 0);
    ~XinyiFerControlTabWidget();

private slots:
    void testRunCode();

protected:
    void timerEvent(QTimerEvent *event);
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::XinyiFerControlTabWidget *ui;

    QThread workerThread;
    ParseServerDataWorker *worker;

    QVector<BaseTankWidget*> tanks;
    QTimer *myTimerThread;

    QPixmap fanOpenBgImg;
    QPixmap fanCloseBgImg;

    QVector<QPushButton*> fanButtons;

    TcpClientSocket* getServerConnectStateTcpClient;
    TcpClientSocket* getAllFerDataTcpClient;
    //TcpClientSocket* getAllFanDataTcpClient;
    bool isServerConnected = false;
    bool fanStateSwitchFlag = false;
    bool ferDataDiff = false;

    void initUI();

    quint64 readStartTime = 0;
    int readNumber = 0;

    QVBoxLayout *allTankLayout;
    QVector<QWidget *> tankLayoutWidgets;

public slots:
    void showFerData(QByteArray);
    void localServerConnected(bool);
    void dispatchFerData(QSet<int>, QMap<float,QString>);

signals:
    void dataChanged();
    void serverConnectionChanged(bool isConnected);
    void dataUpdate(QSet<int>, QMap<float,QString>);
    void serverDataReceived(QByteArray);
    void alertReady(QString);
    void simpleAlertReady(QString);

private slots:
    void read_server_data();
};

#endif // XINYIFERCONTROLTABWIDGET_H
