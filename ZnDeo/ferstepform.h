#ifndef FERSTEPFORM_H
#define FERSTEPFORM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QMessageBox>
#include "fercaseform.h"
#include "ferstep.h"
#include "global.h"

namespace Ui {
class FerStepForm;
}

class FerStepForm : public QWidget
{
    Q_OBJECT

public:
    explicit FerStepForm(QWidget *parent = 0);
    ~FerStepForm();
    void initStep(FerStep *step);
    void addNewCase();
    QString checkStepValidation();
    void setStepSequence(int seq);

signals:
    void sizeChanged(int w, int h);

public slots:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_reduce_case_push_button_clicked();
    void on_add_case_push_button_clicked();

    void on_air_mode_comboBox_currentIndexChanged(int index);
    void handleTextChanged();

private:
    Ui::FerStepForm *ui;
    QVBoxLayout *casesLayout;
    QVector<QWidget *> caseLayoutWidgets;
    int w = 0;
    int h = 0;
    FerStep _ferStep;
    QMessageBox *msgBox;
};

#endif // FERSTEPFORM_H
