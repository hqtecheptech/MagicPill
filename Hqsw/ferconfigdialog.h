#ifndef FERCONFIGDIALOG_H
#define FERCONFIGDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QList>
#include <QFile>
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

public slots:
    void updateSize(int aw, int ah);

private:
    Ui::FerConfigDialog *ui;
    QVBoxLayout *centerLayout;
    QVector<QWidget *> centerLayoutWidgets;
    int w = 0;
    int h = 0;

    FerConfig _ferConfig;
    QList<FerStep *> fersteps;
    bool loadConfig(QList<FerStep *> *steps);
};

#endif // FERCONFIGDIALOG_H
