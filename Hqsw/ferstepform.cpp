#include "ferstepform.h"
#include "ui_ferstepform.h"
#include "keyboard.h"
#include <QTextEdit>

FerStepForm::FerStepForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FerStepForm)
{
    ui->setupUi(this);

    msgBox.setStyleSheet(
        "QPushButton {"
        " background-color: #89AFDE;"
        " border-style: outset;"
        " border-width: 2px;"
        " border-radius: 10px;"
        " border-color: beige;"
        " font: bold 24px;"
        " min-width: 5em;"
        " min-height:5em;"
        " padding: 20px;"
        "}"
        "QLabel {"
        " min-height:5em;"
        " font:24px;"
        " background-color: #89AFDE;"
        " border-style: outset;"
        " border-width: 2px;"
        " border-radius: 10px;"
        " border-color: beige;"
        " padding: 20px;"
        "}"
    );

    casesLayout = new QVBoxLayout(this);
    casesLayout->setSpacing(0);
    ui->cases_frame->setLayout(casesLayout);

    ui->def_ae_text_edit->installEventFilter(this);
    ui->def_sta_text_edit->installEventFilter(this);
    ui->def_frq_text_edit->installEventFilter(this);
    ui->max_time_text_edit->installEventFilter(this);
    ui->min_time_text_edit->installEventFilter(this);
    ui->step_tmp_text_edit->installEventFilter(this);
    ui->expect_tmp_text_edit->installEventFilter(this);

    Keyboard *keyboard = Keyboard::getInstance();
    connect(ui->def_ae_text_edit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_textEdit()));
    connect(ui->def_sta_text_edit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_textEdit()));
    connect(ui->def_frq_text_edit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_textEdit()));
    connect(ui->max_time_text_edit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_textEdit()));
    connect(ui->min_time_text_edit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_textEdit()));
    connect(ui->step_tmp_text_edit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_textEdit()));
    connect(ui->expect_tmp_text_edit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_textEdit()));

    connect(ui->def_ae_text_edit, SIGNAL(textChanged()), this, SLOT(handleTextChanged()));
    connect(ui->def_sta_text_edit, SIGNAL(textChanged()), this, SLOT(handleTextChanged()));
    connect(ui->def_frq_text_edit, SIGNAL(textChanged()), this, SLOT(handleTextChanged()));
    connect(ui->max_time_text_edit, SIGNAL(textChanged()), this, SLOT(handleTextChanged()));
    connect(ui->min_time_text_edit, SIGNAL(textChanged()), this, SLOT(handleTextChanged()));
    connect(ui->step_tmp_text_edit, SIGNAL(textChanged()), this, SLOT(handleTextChanged()));
    connect(ui->expect_tmp_text_edit, SIGNAL(textChanged()), this, SLOT(handleTextChanged()));
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
    ui->def_frq_text_edit->setText(QString::number(step->defaultParaFEQ()));
    _ferStep.setDefaultParaFEQ(step->defaultParaFEQ());
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
        ferCase->setFreq(step->ferCases().at(i)->freq());
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
    fcf->setCaseSequence(caseLayoutWidgets.length());
    casesLayout->addWidget(fcf);
    caseLayoutWidgets.append(fcf);
    FerCase *ferCase = new FerCase;
    _ferStep.appendCase(ferCase);
    ui->plan_num_label->setText(QString::number(_ferStep.ferCases().length()));

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

    Global::ferConfigStrContent.append("plan_numb " + QString::number(caseLayoutWidgets.length()));
    Global::ferConfigStrContent.append("next_step_time_max " + QString::number(_ferStep.nextStepTimeMax()));
    Global::ferConfigStrContent.append("next_step_time_min " + QString::number(_ferStep.nextStepTimeMin()));
    Global::ferConfigStrContent.append("next_step_temp " + QString::number(_ferStep.nextStepTemp()));
    Global::ferConfigStrContent.append("hope_temp " + QString::number(_ferStep.hopeTemp()));
    Global::ferConfigStrContent.append("default_para " + QString::number(_ferStep.defaultParaAE())
                                       + " " + QString::number(_ferStep.defaultParaSTA())
                                       + " " + QString::number(_ferStep.defaultParaFEQ()));

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
    /*if (event->type()==QEvent::FocusIn)     //然后再判断控件的具体事件 (这里指获得焦点事件)
    {
        QPalette p=QPalette();
        p.setColor(QPalette::Base,Qt::green);
        ((QWidget *)watched)->setPalette(p);
    }*/

    if(watched == ui->step_tmp_text_edit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->step_tmp_text_edit->selectionChanged();
        }
    }
    else if(watched == ui->expect_tmp_text_edit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->expect_tmp_text_edit->selectionChanged();
        }
    }
    else if(watched == ui->max_time_text_edit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->max_time_text_edit->selectionChanged();
        }
    }
    else if(watched == ui->min_time_text_edit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->min_time_text_edit->selectionChanged();
        }
    }
    else if(watched == ui->def_ae_text_edit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->def_ae_text_edit->selectionChanged();
        }
    }
    else if(watched == ui->def_sta_text_edit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->def_sta_text_edit->selectionChanged();
        }
    }
    else if(watched == ui->def_frq_text_edit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->def_frq_text_edit->selectionChanged();
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
        ui->plan_num_label->setText(QString::number(_ferStep.ferCases().length()));

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

void FerStepForm::handleTextChanged()
{
    QTextEdit *te = (QTextEdit *)sender();

    QPalette p=QPalette();
    p.setColor(QPalette::Base,Qt::white);
    te->setPalette(p);

    bool isValid = false;
    QString textValue = "";

    if(te == ui->step_tmp_text_edit)
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
                p.setColor(QPalette::Base,Qt::red);
                te->setPalette(p);
            }
        }
    }
    else if(te == ui->expect_tmp_text_edit)
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
                p.setColor(QPalette::Base,Qt::red);
                te->setPalette(p);
            }
        }
    }
    else if(te == ui->max_time_text_edit)
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
                    p.setColor(QPalette::Base,Qt::red);
                    te->setPalette(p);
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
                p.setColor(QPalette::Base,Qt::red);
                te->setPalette(p);
            }
        }
    }
    else if(te == ui->min_time_text_edit)
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
                    p.setColor(QPalette::Base,Qt::red);
                    te->setPalette(p);
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
                p.setColor(QPalette::Base,Qt::red);
                te->setPalette(p);
            }
        }
    }
    else if(te == ui->def_ae_text_edit)
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
                    p.setColor(QPalette::Base,Qt::red);
                    te->setPalette(p);
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
                p.setColor(QPalette::Base,Qt::red);
                te->setPalette(p);
            }
        }
    }
    else if(te == ui->def_sta_text_edit)
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
                    p.setColor(QPalette::Base,Qt::red);
                    te->setPalette(p);
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
                p.setColor(QPalette::Base,Qt::red);
                te->setPalette(p);
            }
        }
    }
    else if(te == ui->def_frq_text_edit)
    {
        textValue = ui->def_frq_text_edit->toPlainText();
        if(textValue != "")
        {
            int value = textValue.toInt(&isValid);
            if(isValid)
            {
                if(value <= 0)
                {
                    msgBox.setText("频率必须大于0!");
                    msgBox.show();
                    p.setColor(QPalette::Base,Qt::red);
                    te->setPalette(p);
                }
                else
                {
                    _ferStep.setDefaultParaFEQ(value);
                }
            }
            else
            {
                msgBox.setText("输入格式错误，请输入大于0的整数!");
                msgBox.show();
                p.setColor(QPalette::Base,Qt::red);
                te->setPalette(p);
            }
        }
    }
}
