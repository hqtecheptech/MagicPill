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

void FerStepForm::initStep(FerStep *step)
{
    ui->cases_frame->resize(1200, step->ferCases().length()*100);
    _ferStep.setPlanNum(step->planNum());
    _ferStep.setAirMode(step->airMode());
    _ferStep.setNextStepTimeMax(step->nextStepTimeMax());
    _ferStep.setNextStepTimeMin(step->nextStepTimeMin());
    _ferStep.setNextStepTemp(step->nextStepTemp());
    _ferStep.setDefaultParaAE(step->defaultParaAE());
    _ferStep.setDefaultParaSTA(step->defaultParaSTA());
    _ferStep.setHopeTemp(step->hopeTemp());
    QList<FerCase*> ferCases;
    for(int i=0; i<step->ferCases().length(); i++)
    {
        FerCaseForm *fcf = new FerCaseForm(this);
        fcf->setMinimumSize(1100, 100);
        fcf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        casesLayout->addWidget(fcf);
        caseLayoutWidgets.append(fcf);
        FerCase *ferCase = new FerCase;
        ferCase->setAe(step->ferCases().at(i)->ae());
        ferCase->setSta(step->ferCases().at(i)->sta());
        ferCase->setLowTempture(step->ferCases().at(i)->lowTempture());
        ferCase->setHighTempture(step->ferCases().at(i)->highTempture());
        ferCases.append(ferCase);
        fcf->setFercase(*ferCase);
    }
    _ferStep.setFerCases(ferCases);
    w = width();
    h = height();
}

void FerStepForm::addNewCase()
{
    FerCaseForm *fcf = new FerCaseForm(this);
    fcf->setMinimumSize(1100, 100);
    fcf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    casesLayout->addWidget(fcf);
    caseLayoutWidgets.append(fcf);
    FerCase *ferCase = new FerCase;
    _ferStep.appendCase(ferCase);

    int cw = ui->cases_frame->width();
    int ch = ui->cases_frame->height() + 100;
    ui->cases_frame->resize(cw, ch);
    //ui->cases_frame->setMinimumSize(cw, ch);
    //ui->cases_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    w = width();
    h = height();
    h += 100;
    resize(w, h);
    setMinimumSize(w, h);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    cw = ui->cases_frame->width();
    ch = ui->cases_frame->height();

    emit sizeChanged(0, 100);
}

void FerStepForm::on_reduce_case_push_button_clicked()
{
    if(!caseLayoutWidgets.isEmpty())
    {
        QWidget *lastWidget = caseLayoutWidgets.last();
        casesLayout->removeWidget(caseLayoutWidgets.last());
        caseLayoutWidgets.removeLast();
        lastWidget->deleteLater();
        _ferStep.removeLastCase();

        int cw = ui->cases_frame->width();
        int ch = ui->cases_frame->height() - 100;
        ui->cases_frame->resize(cw, ch);
        //ui->cases_frame->setMinimumSize(cw, ch);
        //ui->cases_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        w = width();
        h = height();
        h -= 100;
        resize(w, h);
        setMinimumSize(w, h);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        cw = ui->cases_frame->width();
        ch = ui->cases_frame->height();

        emit sizeChanged(0, -100);
    }
}

void FerStepForm::on_add_case_push_button_clicked()
{
    addNewCase();
}
