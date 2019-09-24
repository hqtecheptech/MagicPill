#ifndef HISTORYDLG_H
#define HISTORYDLG_H

#include <QDialog>
#include <QVector>
#include "sqlitehelper.h"
#include "data.h"

namespace Ui {
class HistoryDlg;
}

class HistoryDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryDlg(QWidget *parent = 0);
    ~HistoryDlg();

private slots:
    void on_oneButton_clicked();

    void on_preButton_clicked();

    void on_nextButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::HistoryDlg *ui;

    SQLiteHelper dbHelper;
    QVector<QString> ampere1Values;
    // History page values
    int ampere1ChartValues[CP];
    int pages, currentPage;
};

#endif // HISTORYDLG_H
