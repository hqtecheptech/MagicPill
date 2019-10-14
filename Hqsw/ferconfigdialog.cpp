#include "ferconfigdialog.h"
#include "ui_ferconfigdialog.h"
#include "global.h"

FerConfigDialog::FerConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FerConfigDialog)
{
    ui->setupUi(this);

    for(int i=0; i < Global::ferDeviceInfo.Device_Number;i++)
    {
        ui->step_seq_comboBox->addItem(QString::number(i + 1) + QStringLiteral("号发酵池"));
    }

    centerLayout = new QVBoxLayout;
    centerLayout->setSpacing(5);

    ui->step_config_frame->setLayout(centerLayout);
    ui->scrollArea->setWidget(ui->step_config_frame);

    for(int i=0; i < 5; i++)
    {
        FerStepForm *fsf = new FerStepForm(this);
        fsf->setMinimumSize(1300, 700);
        fsf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        fsf->initCases(3);
        centerLayout->addWidget(fsf);
        centerLayoutWidgets.append(fsf);
        connect(fsf, SIGNAL(sizeChanged(int,int)),this,SLOT(updateSize(int,int)));
    }

    w = ui->step_config_frame->width();
    h = 5 * 705;
    ui->step_config_frame->resize(w, h);
}

FerConfigDialog::~FerConfigDialog()
{
    delete ui;
}

void FerConfigDialog::on_remove_step_push_button_clicked()
{
    if(!centerLayoutWidgets.isEmpty())
    {
        QWidget *rw = centerLayoutWidgets.last();
        centerLayout->removeWidget(rw);
        centerLayoutWidgets.removeLast();
        int rh = rw->height() + 5;
        rw->deleteLater();

        h -= rh;
        ui->step_config_frame->setMinimumSize(w, h);
        ui->step_config_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        ui->step_config_frame->resize(w, h);
        qDebug() << h;
    }
}

void FerConfigDialog::on_append_step_push_button_clicked()
{
    FerStepForm *fsf = new FerStepForm(this);
    fsf->setMinimumSize(1300, 400);
    fsf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    fsf->initCases(0);
    centerLayout->addWidget(fsf);
    centerLayoutWidgets.append(fsf);
    connect(fsf, SIGNAL(sizeChanged(int,int)),this,SLOT(updateSize(int,int)));

    h += 405;
    ui->step_config_frame->setMinimumSize(w, h);
    ui->step_config_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    ui->step_config_frame->resize(w, h);
    qDebug() << h;
}

void FerConfigDialog::updateSize(int aw, int ah)
{
    h += ah;
    ui->step_config_frame->setMinimumSize(w, h);
    ui->step_config_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    ui->step_config_frame->resize(w, h);
    qDebug() << h;
}
