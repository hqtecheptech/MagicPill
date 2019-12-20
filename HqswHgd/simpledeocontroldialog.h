#ifndef SIMPLEDEOCONTROLDIALOG_H
#define SIMPLEDEOCONTROLDIALOG_H

#include <QDialog>
#include <QSet>
#include <QMap>
#include <QMessageBox>

#include "dataformat.h"
#include "tcpclientsocket.h"
#include "mytimerthread.h"

namespace Ui {
class SimpleDeoControlDialog;
}

class SimpleDeoControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SimpleDeoControlDialog(QWidget *parent = 0);
    ~SimpleDeoControlDialog();

public slots:
    void updateFermentationData(QSet<int> changedDataSet, QMap<float,QString> dataMap);

private slots:
    void on_startWaterValveButton_clicked();

    void on_stopWaterValveButton_clicked();

    void on_startSparyValveButton_clicked();

    void on_stopSparyValveButton_clicked();

    void on_otherDeviceAutoButton_clicked();

    void on_otherDeviceManualButton_clicked();

private:
    Ui::SimpleDeoControlDialog *ui;

    bool spRunState, spFaultState, wpStartState, spStartState, otherDeviceAutoState;
    TcpClientSocket *tcpClient;
    QMessageBox msgBox;

    void parseFermentationData(QMap<float,QString> dataMap);
    void parseFerRunCtrData(QMap<float,QString> dataMap);
};

#endif // SIMPLEDEOCONTROLDIALOG_H
