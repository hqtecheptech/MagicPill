#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>

#include "dataformat.h"
#include "requestworkthread.h"

namespace Ui {
class AddUserDialog;
}

class AddUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddUserDialog(QWidget *parent = 0);
    ~AddUserDialog();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void on_addPushButton_clicked();
    void showAddUser(QByteArray result);

    void on_testButton_clicked();
    void finishedSlot(QNetworkReply*);


private:
    Ui::AddUserDialog *ui;
    RequestWorkThread *work;
    QNetworkAccessManager *m_NetManger;
    QNetworkReply* m_Reply;
};

#endif // ADDUSERDIALOG_H
