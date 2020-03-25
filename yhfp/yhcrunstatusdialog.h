#ifndef YHCRUNSTATUSDIALOG_H
#define YHCRUNSTATUSDIALOG_H

#include <QDialog>

#include <QTimer>
#include <data.h>
#include <dataformat.h>
#include <global.h>

namespace Ui {
class YhcRunStatusDialog;
}

class YhcRunStatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit YhcRunStatusDialog(QWidget *parent = 0);
    ~YhcRunStatusDialog();

    int deviceIndex() const;
    void setDeviceIndex(int deviceIndex);

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void showStatus();

private:
    Ui::YhcRunStatusDialog *ui;

    QTimer *timer;
    int _deviceIndex = 0;
};

#endif // YHCRUNSTATUSDIALOG_H
