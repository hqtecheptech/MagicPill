#include "ferstepform.h"
#include "ui_ferstepform.h"

FerStepForm::FerStepForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FerStepForm)
{
    ui->setupUi(this);

    casesLayout = new QVBoxLayout(this);
    casesLayout->setSpacing(0);
    ui->cases_frame->setLayout(casesLayout);
}

FerStepForm::~FerStepForm()
{
    delete ui;
}

void FerStepForm::initCases(int num)
{
    ui->cases_frame->resize(1200, num*100);
    for(int i=0; i<num; i++)
    {
        FerCaseForm *fcf = new FerCaseForm(this);
        fcf->setMinimumSize(1100, 100);
        fcf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        casesLayout->addWidget(fcf);
        caseLayoutWidgets.append(fcf);
    }
    w = width();
    h = height();
}

void FerStepForm::on_reduce_case_push_button_clicked()
{
    if(!caseLayoutWidgets.isEmpty())
    {
        QWidget *lastWidget = caseLayoutWidgets.last();
        casesLayout->removeWidget(caseLayoutWidgets.last());
        caseLayoutWidgets.removeLast();
        lastWidget->deleteLater();

        int cw = ui->cases_frame->width();
        int ch = ui->cases_frame->height() - 100;
        ui->cases_frame->resize(cw, ch);
        ui->cases_frame->setMinimumSize(cw, ch);
        ui->cases_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        w = width();
        h = height();
        h -= 100;
        resize(w, h);
        setMinimumSize(w, h);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        emit sizeChanged(0, -100);
    }
}

void FerStepForm::on_add_case_push_button_clicked()
{
    FerCaseForm *fcf = new FerCaseForm(this);
    fcf->setMinimumSize(1100, 100);
    fcf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    casesLayout->addWidget(fcf);
    caseLayoutWidgets.append(fcf);

    int cw = ui->cases_frame->width();
    int ch = ui->cases_frame->height() + 100;
    ui->cases_frame->resize(cw, ch);
    ui->cases_frame->setMinimumSize(cw, ch);
    ui->cases_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    w = width();
    h = height();
    h += 100;
    resize(w, h);
    setMinimumSize(w, h);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    emit sizeChanged(0, 100);
}
