#ifndef FERCASEFORM_H
#define FERCASEFORM_H

#include <QWidget>
#include <QMessageBox>
#include "fercase.h"

namespace Ui {
class FerCaseForm;
}

class FerCaseForm : public QWidget
{
    Q_OBJECT

public:
    explicit FerCaseForm(QWidget *parent = 0);
    ~FerCaseForm();

    FerCase fercase() const;
    void setFercase(const FerCase &fercase);

    QString checkCaseValidation();
    void setCaseSequence(int seq);

public slots:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::FerCaseForm *ui;
    FerCase _fercase;
    QMessageBox msgBox;
};

#endif // FERCASEFORM_H
