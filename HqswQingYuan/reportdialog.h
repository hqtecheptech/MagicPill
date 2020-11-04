#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
//#include <QAxObject>
#include <QMap>
#include "requestworkthread.h"
#include "tcpclientsocket.h"

namespace Ui {
class ReportDialog;
}

class ReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDialog(QWidget *parent = 0);
    ~ReportDialog();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void on_getReportButton_clicked();
    void showReport(QByteArray);
    void showDailyReport(QByteArray);
    void on_importReportButton_clicked();

    void on_prePushButton_clicked();

    void on_nextPushButton_clicked();

    void on_gotoPushButton_clicked();

    void on_getDailyReportButton_clicked();

    void on_exmportDailyReportButton_clicked();

private:
    Ui::ReportDialog *ui;
    RequestWorkThread *getReportWork;
    TcpClientSocket *getReportTcpClient;
    TcpClientSocket *getDailyReportTcpClient;
    //QStringList reportItemList = {"FER_HT_R","FER_MT_R","FER_START_UDI","FER_STEPCTR_UI","FAN_ALERT_BOOL"};
    QList<ushort> reportAddressList;
    QMap<ushort, QString> reportItemMap;
    QMap<ushort, float*> reportMap;
    QMap<ushort, QMap<int, float> *> reportRawData;
    QStandardItemModel *reportModel;
    int exportExcel(QString filePath);
    int exportDailyReportExcel(QString filePath);

    //bool writeCurrentSheet(QAxObject *sheet, const QList<QList<QVariant> > &cells, int rowOffset = 0, int colOffSet = 0);
    void convertToColName(int data, QString &res);
    QString to26AlphabetString(int data);
    void castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant> > &res);
    void castListListVariant2Variant(const QList<QList<QVariant> > &res, QVariant &var);

    QStringList reportValueArray;
    QByteArray allAddressBytes;
    QByteArray allTimeBytes;
    QByteArray dailyReportData;
    int allPage;
    int currentPage;
    void showReportAtPage(int page);
};

#endif // REPORTDIALOG_H
