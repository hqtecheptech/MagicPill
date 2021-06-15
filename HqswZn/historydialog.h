#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>

#include "dataformat.h"
#include "requestworkthread.h"
#include "tcpclientsocket.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class HistoryDialog;
}

class HistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryDialog(QWidget *parent = 0);
    ~HistoryDialog();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_queryDataButton_clicked();
    void showHistory(QByteArray);
    void on_prePushButton_clicked();
    void on_nextPushButton_clicked();
    void on_gotoPushButton_clicked();

private:
    Ui::HistoryDialog *ui;
    //RequestWorkThread *queryWork;
    TcpClientSocket *queryTcpClient;
    QChartView *chartView;
    QByteArray allValueBytes;
    QByteArray allTimeBytes;
    int allPage;
    int currentPage;
    void showChartAtPage(int page);
};

#endif // HISTORYDIALOG_H
