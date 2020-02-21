#include "doubleemform.h"
#include "ui_doubleemform.h"

DoubleEmForm::DoubleEmForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoubleEmForm)
{
    ui->setupUi(this);

    ssTimer = new QTimer();
    connect(ssTimer, SIGNAL(timeout()), this, SLOT(switchStatus()));

    ssTimer->start(2000);
}

DoubleEmForm::~DoubleEmForm()
{
    delete ui;
}

void DoubleEmForm::setStatus(bool emRum, bool emFault, bool coro1, bool inve1, bool coro2, bool inve2)
{
    emRunStatus = emRum;
    emFaultStatus = emFault;
    isCoro1 = coro1;
    isInve1 = inve1;
    isCoro2 = coro2;
    isInve2 = inve2;

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

    if(isCoro1)
    {
        ui->coro_1_label->setStyleSheet("QLabel#coro_1_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->coro_1_label->setStyleSheet("QLabel#coro_1_label{background-image:url(:/pic/inve.png)}");
    }

    if(isCoro2)
    {
        ui->coro_2_label->setStyleSheet("QLabel#coro_2_label{background-image:url(:/pic/coro.png)}");
    }
    else
    {
        ui->coro_2_label->setStyleSheet("QLabel#coro_2_label{background-image:url(:/pic/inve.png)}");
    }

    if(isInve1)
    {
        ui->inve_1_label->setStyleSheet("QLabel#inve_1_label{background-image:url(:/pic/inve.png)}");
    }
    else
    {
        ui->inve_1_label->setStyleSheet("QLabel#inve_1_label{background-image:url(:/pic/coro.png)}");
    }

    if(isInve2)
    {
        ui->inve_2_label->setStyleSheet("QLabel#inve_2_label{background-image:url(:/pic/inve.png)}");
    }
    else
    {
        ui->inve_2_label->setStyleSheet("QLabel#inve_2_label{background-image:url(:/pic/coro.png)}");
    }
}

void DoubleEmForm::switchStatus()
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
