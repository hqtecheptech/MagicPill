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

    ui->def_ae_text_edit->installEventFilter(this);
    ui->def_sta_text_edit->installEventFilter(this);
    ui->max_time_text_edit->installEventFilter(this);
    ui->min_time_text_edit->installEventFilter(this);
    ui->step_tmp_text_edit->installEventFilter(this);
    ui->expect_tmp_text_edit->installEventFilter(this);
}

FerStepForm::~FerStepForm()
{
    delete ui;
}

void FerStepForm::initStep(FerStep *step)
{
    ui->cases_frame->resize(1200, step->ferCases().length()*100);
    ui->plan_num_label->setText(QString::number(step->ferCases().length()));
    _ferStep.setPlanNum(step->planNum());
    ui->air_mode_comboBox->setCurrentIndex(step->airMode());
    _ferStep.setAirMode(step->airMode());
    ui->max_time_text_edit->setText(QString::number(step->nextStepTimeMax()));
    _ferStep.setNextStepTimeMax(step->nextStepTimeMax());
    ui->min_time_text_edit->setText(QString::number(step->nextStepTimeMin()));
    _ferStep.setNextStepTimeMin(step->nextStepTimeMin());
    ui->step_tmp_text_edit->setText(QString::number(step->nextStepTemp()));
    _ferStep.setNextStepTemp(step->nextStepTemp());
    ui->def_ae_text_edit->setText(QString::number(step->defaultParaAE()));
    _ferStep.setDefaultParaAE(step->defaultParaAE());
    ui->def_sta_text_edit->setText(QString::number(step->defaultParaSTA()));
    _ferStep.setDefaultParaSTA(step->defaultParaSTA());
    ui->expect_tmp_text_edit->setText(QString::number(step->hopeTemp()));
    _ferStep.setHopeTemp(step->hopeTemp());
    QList<FerCase*> ferCases;
    for(int i=0; i<step->ferCases().length(); i++)
    {
        FerCaseForm *fcf = new FerCaseForm(this);
        fcf->setMinimumSize(1100, 100);
        fcf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        fcf->setCaseSequence(i);
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

QString FerStepForm::checkStepValidation()
{
    QString result = "";

    if(caseLayoutWidgets.length() == 0)
    {
        return "必须至少有一个CASE";
    }

    for(int i=0; i<caseLayoutWidgets.length();i++)
    {
        result = ((FerCaseForm *)caseLayoutWidgets.at(i))->checkCaseValidation();
        if(result != "OK")
        {
            return "Case " + QString::number(i+1) + ": " + result;
        }
    }

    return _ferStep.checkStepValidation();
}

void FerStepForm::setStepSequence(int seq)
{
    ui->step_seq_label->setText(QString::number(seq+1));
}

bool FerStepForm::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::FocusIn)
    {
        if (event->type()==QEvent::FocusIn)     //然后再判断控件的具体事件 (这里指获得焦点事件)
        {
            QPalette p=QPalette();
            p.setColor(QPalette::Base,Qt::green);
            ((QWidget *)watched)->setPalette(p);
        }
    }
    else if(event->type()==QEvent::FocusOut)
    {
        QPalette p=QPalette();
        p.setColor(QPalette::Base,Qt::white);
        ((QWidget *)watched)->setPalette(p);

        bool isValid = false;
        QString textValue = "";

        if(watched == ui->step_tmp_text_edit)
        {
            textValue = ui->step_tmp_text_edit->toPlainText();
            if(textValue != "")
            {
                float value = textValue.toFloat(&isValid);
                if(isValid)
                {
                    _ferStep.setNextStepTemp(value);
                }
                else
                {
                    msgBox.setText("格式错误，请输入数值类型!");
                    msgBox.show();
                }
            }
        }
        else if(watched == ui->expect_tmp_text_edit)
        {
            textValue = ui->expect_tmp_text_edit->toPlainText();
            if(textValue != "")
            {
                float value = textValue.toFloat(&isValid);
                if(isValid)
                {
                    _ferStep.setHopeTemp(value);
                }
                else
                {
                    msgBox.setText("格式错误，请输入数值类型!");
                    msgBox.show();
                }
            }
        }
        else if(watched == ui->max_time_text_edit)
        {
            textValue = ui->max_time_text_edit->toPlainText();
            if(textValue != "")
            {
                int value = textValue.toInt(&isValid);
                if(isValid)
                {
                    if(value <= 0)
                    {
                        msgBox.setText("最长跳步时间必须大于0!");
                        msgBox.show();
                    }
                    else
                    {
                        _ferStep.setNextStepTimeMax(value);
                    }
                }
                else
                {
                    msgBox.setText("输入格式错误，请输入大于0的整数!");
                    msgBox.show();
                }
            }
        }
        else if(watched == ui->min_time_text_edit)
        {
            textValue = ui->min_time_text_edit->toPlainText();
            if(textValue != "")
            {
                int value = textValue.toInt(&isValid);
                if(isValid)
                {
                    if(value <= 0)
                    {
                        msgBox.setText("最短跳步时间必须大于0!");
                        msgBox.show();
                    }
                    else
                    {
                        _ferStep.setNextStepTimeMin(value);
                    }
                }
                else
                {
                    msgBox.setText("输入格式错误，请输入大于0的整数!");
                    msgBox.show();
                }
            }
        }
        else if(watched == ui->def_ae_text_edit)
        {
            textValue = ui->def_ae_text_edit->toPlainText();
            if(textValue != "")
            {
                int value = textValue.toInt(&isValid);
                if(isValid)
                {
                    if(value <= 0)
                    {
                        msgBox.setText("曝气时长必须大于0!");
                        msgBox.show();
                    }
                    else
                    {
                        _ferStep.setDefaultParaAE(value);
                    }
                }
                else
                {
                    msgBox.setText("输入格式错误，请输入大于0的整数!");
                    msgBox.show();
                }
            }
        }
        else if(watched == ui->def_sta_text_edit)
        {
            textValue = ui->def_sta_text_edit->toPlainText();
            if(textValue != "")
            {
                int value = textValue.toInt(&isValid);
                if(isValid)
                {
                    if(value <= 0)
                    {
                        msgBox.setText("曝气时长必须大于0!");
                        msgBox.show();
                    }
                    else
                    {
                        _ferStep.setDefaultParaSTA(value);
                    }
                }
                else
                {
                    msgBox.setText("输入格式错误，请输入大于0的整数!");
                    msgBox.show();
                }
            }
        }
    }

    return QWidget::eventFilter(watched,event);
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

void FerStepForm::on_air_mode_comboBox_currentIndexChanged(int index)
{
    _ferStep.setAirMode(index);
}
