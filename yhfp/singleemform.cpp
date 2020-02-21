#include "singleemform.h"
#include "ui_singleemform.h"

SingleEmForm::SingleEmForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleEmForm)
{
    ui->setupUi(this);

    ssTimer = new QTimer();
    connect(ssTimer, SIGNAL(timeout()), this, SLOT(switchStatus()));

    ssTimer->start(2000);
}

SingleEmForm::~SingleEmForm()
{
    delete ui;
}

void SingleEmForm::setStatus(bool emRum, bool emFault, bool coro, bool inve, bool unLoading)
{
    emRunStatus = emRum;
    emFaultStatus = emFault;
    isCoro = coro;
    isInve = inve;
    isUnLoading = unLoading;

    if(!emFaultStatus)
    {
        if(emRunStatus)
        {
            ui->em_label->setStyleSheet("QLabel#em_label{background-image:url(:/pic/em_run.png)}");
        }
        else
        {
            ui->em_label->setStyleSheet("QLabel#em_label{background-image:url(:/pic/em_stop.png)}");
        }
    }

    if(isCoro)
    {
        ui->coro_1_label->setStyleSheet("QLabel#coro_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->coro_1_label->setStyleSheet("QLabel#coro_1_label{background-image:url(:/pic/inve.png)}");
    }

    if(isInve)
    {
        ui->inve_1_label->setStyleSheet("QLabel#inve_1_label{background-image:url(:/pic/inve.png)}");
    }
    else
    {
        ui->inve_1_label->setStyleSheet("QLabel#inve_1_label{background-image:url(:/pic/coro.png)}");
    }

    if(isUnLoading)
    {
        ui->dp_1_label->setStyleSheet("QLabel#inve_1_label{background-image:url(:/pic/wheel_run.png)}");
    }
    else
    {
        ui->dp_1_label->setStyleSheet("QLabel#inve_1_label{background-image:url(:/pic/wheel_stop.png)}");
    }
}

void SingleEmForm::switchStatus()
{
    if(emFaultStatus)
    {
        if(stateFlag)
        {
            ui->em_label->setStyleSheet("QLabel#em_label{background-image:url(:/pic/em_fault.png)}");
        }
        else
        {
            ui->em_label->setStyleSheet("QLabel#em_label{background-image:url(:/pic/em_stop.png)}");
        }
    }

    stateFlag = !stateFlag;
}
