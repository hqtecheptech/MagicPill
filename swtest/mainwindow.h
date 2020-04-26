#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QVector>
#include <QLineEdit>

#include "data.h"
#include "sharehelper.h"

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


private slots:
    void refreshData();
    void switchState();
    void autoUpdateValue();

    void on_modifyButton_clicked();

    void on_setDataButton_clicked();

    void on_batchModifyButton_clicked();

    void on_setCtrlButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

signals:


private:
    Ui::MainWindow *ui;

    ShareHelper *ctrShare, *dbShare;
    QTimer *timer1, *timer2, *timer3;

    QVector<QLineEdit *> dataVector;
    bool stateFlag = false;
    bool isFormatCorrect = true;
    uint8_t status = 0;
};

#endif // MAINWINDOW_H
