#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include "yhcc.h"
#include "taskmanager.h"
#include "syscontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_yhccButton_clicked();
    void on_exitButton_clicked();
    void on_syssButton_clicked();

    void checkPruState();

signals:
    void startListenTask();

private:
    Ui::MainWindow *ui;

    Yhcc* yhc;
    QThread taskManageThread;
    TaskManager* taskManager;
    QTimer* pruCheckTimer;
    Syscontroller *controller;
};

#endif // MAINWINDOW_H
