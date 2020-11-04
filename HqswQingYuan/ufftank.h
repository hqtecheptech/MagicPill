#ifndef UFFTANK_H
#define UFFTANK_H

#include "dataformat.h"
#include "tcpclientsocket.h"
#include "mytimerthread.h"
#include "singelfertankvalvecontroldialog.h"
#include "basetankwidget.h"

#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include <QPalette>
#include <QSplitter>
#include <QDataWidgetMapper>

namespace Ui {
class UFFTank;
}


class UFFTank : public BaseTankWidget
{
    Q_OBJECT
public:
    explicit UFFTank(QWidget *parent = 0);
    ~UFFTank();
    void setupFermentationMapper(QVector<float> values, int tankIndex);
    void setupStepRunningTimeMapper(QVector<uint> values, int tankIndex);
    void setupStepMapper(QVector<ushort> values, int tankIndex);
    void setupRunCtrMapper(QVector<bool> values, int tankIndex);

    QStandardItemModel *getTimeModel1() const;
    QStandardItemModel *getFerModel() const;
    QStandardItemModel *getStepModel() const;
    QStandardItemModel *getRunctrModel() const;

    int getTankIndex() const;
    void setTankIndex(int value);

    int getLocation() const;
    void setLocation(int value);

private slots:
    void updateDeviceState();

    void on_valveButton_clicked();

protected:
    void showEvent(QShowEvent *event);

private:
    Ui::UFFTank *ui;
    TcpClientSocket* tcpClient;
    MyTimerThread *myTimerThread;
    SingelFerTankValveControlDialog *sftValveDialog;
    QPalette p;
    QPixmap tankRunBgImg;
    QPixmap tankRunQBgImg;
    QPixmap tankStopBgImg;
    QPixmap valveOpenBgImg;
    QPixmap valveCloseBgImg;
    QPixmap tankRunFanOpenBgImg;
    QPixmap ferAutoBgImg;
    QPixmap ferCustomBgImg;
    bool isFanRemote = false;
    bool isFanOpen = false;
    bool isValveOpen = false;
    bool isValveFault = false;
    bool isFanAuto = false;
    bool isFerStart = false;
    bool isAeration = false;
    bool isFerAuto = false;
    bool switchFlag = false;
    int bgChange = 0;
    int tankIndex = 0;
    int location = 0;
    QStandardItemModel *ferModel;
    QDataWidgetMapper *ferMapper = Q_NULLPTR;
    QStandardItemModel *timeModel;
    QStandardItemModel *timeModel1;
    QDataWidgetMapper *timeMapper = Q_NULLPTR;
    QStandardItemModel *stepModel;
    QDataWidgetMapper *stepMapper = Q_NULLPTR;
    QStandardItemModel *runctrModel;
    QDataWidgetMapper *runctrMapper = Q_NULLPTR;
    QString formatLongDateString(uint value);
};

#endif // UFFTANK_H
