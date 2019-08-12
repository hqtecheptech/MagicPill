#ifndef YHCC_H
#define YHCC_H

#include <QDialog>
#include <QThread>
#include <QTimer>
#include "netstatemanageworker.h"
#include "plcdatamanageworker.h"

#include "data.h"

namespace Ui {
class Yhcc;
}

class Yhcc : public QDialog
{
    Q_OBJECT

public:
    explicit Yhcc(QWidget *parent = 0);
    ~Yhcc();

private slots:
    void on_yhcExitButton_clicked();

    void on_speedDownButton_pressed();

    void on_speedDownButton_released();

    void on_speedUpButton_pressed();

    void on_speedUpButton_released();

    void on_yhDownButton_pressed();

    void on_yhDownButton_released();

    void on_yhUpButton_pressed();

    void on_yhUpButton_released();

    void netStatChecked(bool state);

    void getNetState();

    void pollPlcDatas();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

signals:
    void checkNetState(QString type);
    void pollingDatas();

public slots:
    void updateUI(const PlcData newDatas);

private:
    Ui::Yhcc *ui;

    QThread netManageThread;
    NetStateManageWorker* nsmWorker;
    QThread plcdataManageThread;
    PlcDataManageWorker* pdmWorker;
    QTimer *checkNetStateTimer;
    QTimer *pollDatasTimer;
    int uca = 0;
};

#endif // YHCC_H
