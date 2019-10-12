#include "ferstepform.h"
#include "ui_ferstepform.h"

FerStepForm::FerStepForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FerStepForm)
{
    ui->setupUi(this);

    casesLayout = new QVBoxLayout(this);
    ui->cases_frame->setLayout(casesLayout);
}

FerStepForm::~FerStepForm()
{
    delete ui;
}

void FerStepForm::initCases(int num)
{
    ui->cases_frame->resize(1200, (num+1)*100);
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

        int height = ui->cases_frame->height();
        int width = ui->cases_frame->width();
        ui->cases_frame->resize(width, height - 100);

        h -= 100;
        setMinimumSize(w, h);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        resize(w, h);
    }
}

void FerStepForm::on_add_case_push_button_clicked()
{
    FerCaseForm *fcf = new FerCaseForm(this);
    fcf->setMinimumSize(1100, 100);
    fcf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    casesLayout->addWidget(fcf);
    caseLayoutWidgets.append(fcf);

    int height = ui->cases_frame->height();
    int width = ui->cases_frame->width();
    ui->cases_frame->resize(width, height + 100);

    h += 100;
    setMinimumSize(w, h);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    resize(w, h);
}
