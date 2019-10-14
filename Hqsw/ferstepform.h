#ifndef FERSTEPFORM_H
#define FERSTEPFORM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QVector>
#include "fercaseform.h"

namespace Ui {
class FerStepForm;
}

class FerStepForm : public QWidget
{
    Q_OBJECT

public:
    explicit FerStepForm(QWidget *parent = 0);
    ~FerStepForm();
    void initCases(int num);

signals:
    void sizeChanged(int w, int h);

private slots:
    void on_reduce_case_push_button_clicked();
    void on_add_case_push_button_clicked();

private:
    Ui::FerStepForm *ui;
    QVBoxLayout *casesLayout;
    QVector<QWidget *> caseLayoutWidgets;
    int w = 0;
    int h = 0;
};

#endif // FERSTEPFORM_H
