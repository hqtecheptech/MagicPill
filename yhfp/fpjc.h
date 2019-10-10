#ifndef FPJC_H
#define FPJC_H

#include <QDialog>
#include <QTimer>
#include <QTime>
#include "syscontroller.h"
#include "netstatemanageworker.h"

#include "data.h"

namespace Ui {
class fpjc;
}

class Fpjc : public QDialog
{
    Q_OBJECT

public:
    explicit Fpjc(QWidget *parent = 0);
    ~Fpjc();

private slots:
    void on_yhcExitButton_clicked();

private:
    Ui::fpjc *ui;

    QThread netManageThread;
    NetStateManageWorker* nsmWorker;
    QTimer* checkNetStateTimer;
    QTimer* testTimer;
    QTime st;
    int uca = 0;
    Syscontroller* controller;
    Plc_Db oldPlcDb;
    int deviceIndex = 0;
};

#endif // FPJC_H
