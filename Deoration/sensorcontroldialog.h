#ifndef SENSORCONTROLDIALOG_H
#define SENSORCONTROLDIALOG_H

#include <QDialog>
#include <QPalette>
#include <QMessageBox>

#include "tcpclientsocket.h"

namespace Ui {
class SensorControlDialog;
}

class SensorControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SensorControlDialog(QWidget *parent = 0);
    ~SensorControlDialog();

    int getTankIndex() const;
    void setTankIndex(int value);

private:
    Ui::SensorControlDialog *ui;

    bool isSensorRaised;
    bool isSensorFall;
    QPalette p;
    int tankIndex;
    QMessageBox msgBox;
    TcpClientSocket *tcpClient;

    void parseFerRunCtrData(QMap<float,QString> dataMap);

public slots:
    void updateFermentationData(QSet<int> changedDeviceSet, QMap<float,QString> dataMap);

private slots:
    void on_sensorFallPushButton_clicked();
    void on_sensorRaisePushButton_clicked();
    void on_sensorIndexComboBox_currentIndexChanged(int index);
    void on_sensorStopPushButton_clicked();
};

#endif // SENSORCONTROLDIALOG_H
