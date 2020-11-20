#ifndef FERCONFIGDIALOG_H
#define FERCONFIGDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QList>
#include <QFile>
#include <QMessageBox>
#include <QScrollBar>
#include "ferstepform.h"
#include "ferstep.h"
#include "fercase.h"
#include "ferconfig.h"

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

    void on_reload_push_button_clicked();

    void on_save_push_button_clicked();

    void on_step_seq_comboBox_currentIndexChanged(int index);

    void on_exitButton_clicked();

public slots:
    void updateSize(int aw, int ah);

private:
    Ui::FerConfigDialog *ui;
    QVBoxLayout *centerLayout;
    QVector<QWidget *> centerLayoutWidgets;
    QMessageBox msgBox;
    int w = 0;
    int h = 0;

    QList<FerStep *> _ferSteps;
    void initUI();
    void clearUI();
};

#endif // FERCONFIGDIALOG_H
