#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QByteArray>
#include "dataformat.h"
#include "tcpclientsocket.h"
//#include "datasender.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void on_loginButton_clicked();
    void showLogin(QByteArray result);
    void on_addUserButton_clicked();
    void on_logoutButton_clicked();

    void on_testSendControlButton_clicked();

    void on_testSendControlButton_2_clicked();

signals:
    void LogingStateChanged();

private:
    Ui::LoginDialog *ui;
    TcpClientSocket *tcpClient;
    //DataSender *ds;

    int exportExcel(QString filename);
    int index = 0;
};

#endif // LOGINDIALOG_H
