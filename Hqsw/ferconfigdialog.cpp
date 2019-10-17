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

    _ferConfig.loadConfig(&_ferSteps);

    centerLayout = new QVBoxLayout;
    centerLayout->setSpacing(5);

    ui->step_config_frame->setLayout(centerLayout);
    ui->scrollArea->setWidget(ui->step_config_frame);

    initUI();
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
        _ferSteps.removeLast();

        h -= rh;
        ui->step_config_frame->resize(w, h);
        ui->step_config_frame->setMinimumSize(w, h);
        ui->step_config_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        qDebug() << h;
    }
}

void FerConfigDialog::on_append_step_push_button_clicked()
{
    FerStepForm *fsf = new FerStepForm(this);
    fsf->setMinimumSize(1300, 300);
    fsf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    centerLayout->addWidget(fsf);
    centerLayoutWidgets.append(fsf);
    connect(fsf, SIGNAL(sizeChanged(int,int)),this,SLOT(updateSize(int,int)));
    FerStep *newStep = new FerStep;
    _ferSteps.append(newStep);

    h += 305;

    ui->step_config_frame->resize(w, h);
    ui->step_config_frame->setMinimumSize(w, h);
    ui->step_config_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QScrollBar *pScrollBar = ui->scrollArea->verticalScrollBar();
    if (pScrollBar != NULL)
    {
        pScrollBar->setValue(h);
    }

    qDebug() << h;
}

void FerConfigDialog::updateSize(int aw, int ah)
{
    h += ah;

    ui->step_config_frame->resize(w, h);
    ui->step_config_frame->setMinimumSize(w, h);
    ui->step_config_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QScrollBar *pScrollBar = ui->scrollArea->verticalScrollBar();
    if (pScrollBar != NULL)
    {
        pScrollBar->setValue(pScrollBar->value() + ah);
    }

    qDebug() << h;
}

bool FerConfigDialog::loadConfig(QList<FerStep *> *steps)
{
    QFile file("fer_para_conf_1");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Read configuration file failed!";
        return false;
    }

    int stepNum = 0;
    while(!file.atEnd())
    {
        QString rawStr = QString(file.readLine());
        QStringList contentStrList = rawStr.split("\n", QString::SplitBehavior::SkipEmptyParts);
        if(contentStrList.length() > 0 && contentStrList.at(0).toLocal8Bit().constData() != "<end>")
        {
            QStringList fieldsStrList = contentStrList.at(0).split(" ", QString::SplitBehavior::SkipEmptyParts);
            if(fieldsStrList.length() > 0)
            {
                QString value = fieldsStrList.at(0);
                if(value == "<step_" + QString::number(stepNum + 1) + ">")
                {
                    FerStep* ferStep = new FerStep();
                    QList<FerCase *> ferCases;

                    int caseNum = 0;
                    while(!file.atEnd())
                    {
                        rawStr = "";
                        contentStrList.clear();
                        fieldsStrList.clear();
                        rawStr = QString(file.readLine());
                        contentStrList = rawStr.split("\n", QString::SplitBehavior::SkipEmptyParts);

                        QString endStr = "";
                        if(contentStrList.length() > 0)
                        {
                            endStr = contentStrList.at(0).toLocal8Bit().constData();
                        }

                        if(contentStrList.length() > 0 && endStr != "<end>")
                        {
                            fieldsStrList = contentStrList.at(0).split(" ", QString::SplitBehavior::SkipEmptyParts);
                            if(fieldsStrList.length() > 0)
                            {
                                value = fieldsStrList.at(0).toLocal8Bit().constData();
                                if(value == "plan_numb")
                                {
                                    ferStep->setPlanNum(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toInt());
                                }
                                else if(value == "air_mode")
                                {
                                    ferStep->setAirMode(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toInt());
                                }
                                else if(value == "next_step_time_max")
                                {
                                    ferStep->setNextStepTimeMax(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toInt());
                                }
                                else if(value == "next_step_time_min")
                                {
                                    ferStep->setNextStepTimeMin(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toInt());
                                }
                                else if(value == "next_step_temp")
                                {
                                    ferStep->setNextStepTemp(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toFloat());
                                }
                                else if(value == "hope_temp")
                                {
                                    ferStep->setHopeTemp(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toFloat());
                                }
                                else if(value == "case_" + QString::number(caseNum+1))
                                {
                                    FerCase* ferCase = new FerCase;
                                    ferCase->setLowTempture(QByteArray(fieldsStrList.at(1).toLocal8Bit().constData()).toFloat());
                                    ferCase->setHighTempture(QByteArray(fieldsStrList.at(5).toLocal8Bit().constData()).toFloat());
                                    ferCase->setAe(QByteArray(fieldsStrList.at(6).toLocal8Bit().constData()).toFloat());
                                    ferCase->setSta(QByteArray(fieldsStrList.at(7).toLocal8Bit().constData()).toFloat());
                                    ferCases.append(ferCase);
                                    caseNum++;
                                }
                            }
                        }
                        else
                        {
                            ferStep->setFerCases(ferCases);
                            break;
                        }
                    }

                    steps->append(ferStep);
                    stepNum++;
                }
            }
        }
    }

    return true;
}

void FerConfigDialog::initUI()
{
    for(int i=0; i < _ferSteps.length(); i++)
    {
        FerStepForm *fsf = new FerStepForm(this);
        int caseNum = _ferSteps.at(i)->ferCases().length();
        fsf->setMinimumSize(1300, 300 + caseNum*100);
        fsf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        fsf->setStepSequence(i);
        fsf->initStep(_ferSteps.at(i));
        centerLayout->addWidget(fsf);
        centerLayoutWidgets.append(fsf);
        connect(fsf, SIGNAL(sizeChanged(int,int)),this,SLOT(updateSize(int,int)));
        h += 300 + caseNum*100 + 5;
    }

    w = ui->step_config_frame->width();
    ui->step_config_frame->resize(w, h);
}

void FerConfigDialog::clearUI()
{
    while(!centerLayoutWidgets.isEmpty())
    {
        QWidget *rw = centerLayoutWidgets.last();
        centerLayout->removeWidget(rw);
        centerLayoutWidgets.removeLast();
        int rh = rw->height() + 5;
        rw->deleteLater();

        h -= rh;
        //ui->step_config_frame->setMinimumSize(w, h);
        //ui->step_config_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        ui->step_config_frame->resize(w, h);
        qDebug() << h;
    }
    _ferSteps.clear();
}

void FerConfigDialog::on_reload_push_button_clicked()
{
    clearUI();
    _ferConfig.loadConfig(&_ferSteps);
    initUI();
}

void FerConfigDialog::on_save_push_button_clicked()
{
    if(centerLayoutWidgets.length() == 0)
    {
        msgBox.setText("当前方案还没有添加发酵步骤");
        msgBox.show();
        return;
    }
    else
    {
        for(int i=0; i<centerLayoutWidgets.length();i++)
        {
            QString result = ((FerStepForm *)centerLayoutWidgets.at(i))->checkStepValidation();
            if(result != "OK")
            {
                msgBox.setText("Step " + QString::number(i+1) + ": " + result);
                msgBox.show();
                return;
            }
        }
    }

    msgBox.setText("保存成功");
    msgBox.show();
}
