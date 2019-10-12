#ifndef FERCONFIGDIALOG_H
#define FERCONFIGDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QVector>
#include "ferstepform.h"

namespace Ui {
class FerConfigDialog;
}

class FerConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FerConfigDialog(QWidget *parent = 0);
    ~FerConfigDialog();

private slots:
    void on_remove_step_push_button_clicked();

    void on_append_step_push_button_clicked();

private:
    Ui::FerConfigDialog *ui;
    QVBoxLayout *centerLayout;
    QVector<QWidget *> centerLayoutWidgets;
    int w = 0;
    int h = 0;
};

#endif // FERCONFIGDIALOG_H
