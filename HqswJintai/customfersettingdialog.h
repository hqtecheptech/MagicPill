#ifndef CUSTOMFERSETTINGDIALOG_H
#define CUSTOMFERSETTINGDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QList>

#include "tcpclientsocket.h"

namespace Ui {
class CustomFerSettingDialog;
}

class CustomFerSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomFerSettingDialog(QWidget *parent = 0);
    ~CustomFerSettingDialog();

    int getTankLocation() const;
    void setTankLocation(int value);

    void setRunTime(int value);

    void setSpaceTime(int value);

protected:
    bool eventFilter(QObject *, QEvent *);

private slots:
    void showSetFerAuto(QByteArray);
    void showFerStart(QByteArray);

    void on_customFerButton_pressed();
    void on_customFerButton_released();

    void on_customFerButton_2_clicked();

    void on_endCustomFerButton_pressed();

public slots:
    void updateFermentationData(QSet<int>, QMap<float,QString> dataMap);

private:
    Ui::CustomFerSettingDialog *ui;
    QMessageBox msgBox;
    TcpClientSocket *tcpClient;
    TcpClientSocket *tcpClient1;
    TcpClientSocket *aerationTimeTcpClient;
    TcpClientSocket *aerationSpaceTcpClient;
    TcpClientSocket *aerationHzTcpClient;
    int tankLocation = 0;
    int runTime = 0;
    int spaceTime = 0;
};

#endif // CUSTOMFERSETTINGDIALOG_H
