#ifndef FERCONTROLDIALOG_H
#define FERCONTROLDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QList>
#include <QVector>
#include <QMap>

#include "dataformat.h"
#include "tcpclientsocket.h"
#include "fftank.h"
#include "requestworkthread.h"
#include "mytimerthread.h"
#include "customfersettingdialog.h"
#include "ufftank.h"
#include "ferconfig.h"
#include "ferstep.h"

namespace Ui {
class FerControlDialog;
}

class FerControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FerControlDialog(QWidget *parent = 0);
    ~FerControlDialog();

    int getTankIndex() const;
    void setTankIndex(int value);

    CustomFerSettingDialog *getDialog() const;
    void setDialog(CustomFerSettingDialog *value);

protected:
    void timerEvent( QTimerEvent *event );
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *);

private:
    Ui::FerControlDialog *ui;

    QMessageBox *msgBox;
    MyTimerThread *myTimerThread;
    ushort currentStep = 1;
    ushort newStep = 1;
    int tankIndex = 0;
    QPalette p;
    QPixmap icoGreen;
    QPixmap icoYellow;
    QPixmap icoRed;
    QVector<FFTank *> tanks;
    //QVector<UFFTank *> tanks;
    FFTank* _tank;
    //UFFTank* _tank;
    TcpClientSocket *tcpClient;
    TcpClientSocket *tcpClient1;
    RequestWorkThread *work;
    QStandardItemModel *ferModel;
    QDataWidgetMapper *ferMapper = Q_NULLPTR;
    QStandardItemModel *timeModel;
    QDataWidgetMapper *timeMapper = Q_NULLPTR;
    QStandardItemModel *stepModel;
    QDataWidgetMapper *stepMapper = Q_NULLPTR;
    QStandardItemModel *runctrModel;
    QDataWidgetMapper *runctrMapper = Q_NULLPTR;
    CustomFerSettingDialog *customFerSettingDialog;
    //bool isFanRemote = false;
    bool isFanAuto = false;
    bool isFanRun = false;
    bool isFerStarted = false;
    bool isFerPaused = false;
    QMap<int, QList<FerStep *>> _ferConfigs;

    int handRunTime = 0;
    int handSpaceTime = 0;
    int handFrequency = 0;

    void parseFermentationData(QMap<float,QString> dataMap);
    void parseFerRunTimeData(QMap<float,QString> dataMap);
    void parseFerStepData(QMap<float,QString> dataMap);
    void parseFerRunCtrData(QMap<float,QString> dataMap);
    void parseFerStartEndTime(QMap<float,QString> dataMap);

    QString formatLongDateString(uint value);
    void setFerRunSteps();

public slots:
    void updateFermentationData(QSet<int>, QMap<float,QString> dataMap);

private slots:
    void showFerStart(QByteArray);
    void showSetFerAuto(QByteArray);
    void showFerStop(QByteArray);
    void showStepChange(QByteArray);
    void tankIndexChanged(int index);
    void stepIndexChanged(int index);
    void on_customFerButton_clicked();
    void on_startFerButton_pressed();
    void on_startFerButton_released();
    void on_endFerButton_pressed();
    void on_endFerButton_released();
    void on_tankIndexComboBox_currentIndexChanged(int index);
    void on_changeStepButton_clicked();
    void on_pauseFerButton_clicked();
    void on_closeButton_clicked();
};

#endif // FERCONTROLDIALOG_H
