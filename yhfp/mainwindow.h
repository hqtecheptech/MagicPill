#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "yhcc.h"
#include <QThread>
#include <QTimer>
#include "taskmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_yhccButton_clicked();
    void on_exitButton_clicked();

    void on_syssButton_clicked();

signals:
    void startListenTask();

private:
    Ui::MainWindow *ui;

    Yhcc* yhc;
    QThread taskManageThread;
    TaskManager* taskManager;
};

#endif // MAINWINDOW_H
