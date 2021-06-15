#include "fercaseform.h"
#include "ui_fercaseform.h"
#include "keyboard.h"

FerCaseForm::FerCaseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FerCaseForm)
{
    ui->setupUi(this);

    msgBox = new QMessageBox(this);
    msgBox->setStyleSheet(
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

    ui->low_tmp_text_edit->installEventFilter(this);
    ui->high_tmp_text_edit->installEventFilter(this);
    ui->ae_text_edit->installEventFilter(this);
    ui->sta_text_edit->installEventFilter(this);
    ui->freq_text_edit->installEventFilter(this);
    this->installEventFilter(this);

    Keyboard *keyboard = Keyboard::getInstance();
    connect(ui->low_tmp_text_edit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_textEdit()));
    connect(ui->high_tmp_text_edit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_textEdit()));
    connect(ui->ae_text_edit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_textEdit()));
    connect(ui->sta_text_edit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_textEdit()));
    connect(ui->freq_text_edit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_textEdit()));

    connect(ui->low_tmp_text_edit, SIGNAL(textChanged()), this, SLOT(handleTextChanged()));
    connect(ui->high_tmp_text_edit, SIGNAL(textChanged()), this, SLOT(handleTextChanged()));
    connect(ui->ae_text_edit, SIGNAL(textChanged()), this, SLOT(handleTextChanged()));
    connect(ui->sta_text_edit, SIGNAL(textChanged()), this, SLOT(handleTextChanged()));
    connect(ui->freq_text_edit, SIGNAL(textChanged()), this, SLOT(handleTextChanged()));
}

FerCaseForm::~FerCaseForm()
{
    delete ui;
}

FerCase FerCaseForm::fercase() const
{
    return _fercase;
}

void FerCaseForm::setFercase(const FerCase &fercase)
{
    _fercase.setAe(fercase.ae());
    ui->ae_text_edit->setText(QString::number(fercase.ae()));
    _fercase.setSta(fercase.sta());
    ui->sta_text_edit->setText(QString::number(fercase.sta()));
    _fercase.setHighTempture(fercase.highTempture());
    ui->high_tmp_text_edit->setText(QString::number(fercase.highTempture()));
    _fercase.setLowTempture(fercase.lowTempture());
    ui->low_tmp_text_edit->setText(QString::number(fercase.lowTempture()));
    _fercase.setFreq(fercase.freq());
    ui->freq_text_edit->setText(QString::number(fercase.freq()));
}

QString FerCaseForm::checkCaseValidation()
{
    return _fercase.checkCaseValidation(_seq);
}

void FerCaseForm::setCaseSequence(int seq)
{
    _seq = seq;
    ui->seq_label->setText(QString::number(seq+1));
}

bool FerCaseForm::eventFilter(QObject *watched, QEvent *event)
{
    /*if (event->type()==QEvent::FocusIn)     //然后再判断控件的具体事件 (这里指获得焦点事件)
    {
        QPalette p=QPalette();
        p.setColor(QPalette::Base,Qt::green);
        ((QWidget *)watched)->setPalette(p);
    }*/

    if(watched == this && event->type() == QEvent::WindowDeactivate)
    {
        msgBox->close();
    }

    if(watched == ui->low_tmp_text_edit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->low_tmp_text_edit->selectionChanged();
        }
    }
    else if(watched == ui->high_tmp_text_edit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->high_tmp_text_edit->selectionChanged();
        }
    }
    else if(watched == ui->ae_text_edit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->ae_text_edit->selectionChanged();
        }
    }
    else if(watched == ui->sta_text_edit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->sta_text_edit->selectionChanged();
        }
    }
    else if(watched == ui->freq_text_edit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->freq_text_edit->selectionChanged();
        }
    }

    return QWidget::eventFilter(watched,event);
}

void FerCaseForm::handleTextChanged()
{
    QTextEdit *te = (QTextEdit *)sender();

    QPalette p=QPalette();
    p.setColor(QPalette::Base,Qt::white);
    te->setPalette(p);

    bool isValid = false;
    QString textValue = "";

    if(te == ui->high_tmp_text_edit)
    {
        textValue = ui->high_tmp_text_edit->toPlainText();
        if(textValue != "")
        {
            float value = textValue.toFloat(&isValid);
            if(isValid)
            {
                _fercase.setHighTempture(value);
            }
            else
            {
                msgBox->setText("格式错误，请输入数值类型!");
                msgBox->show();
                p.setColor(QPalette::Base,Qt::red);
                te->setPalette(p);
                te->setText("");
            }
        }
    }
    else if(te == ui->low_tmp_text_edit)
    {
        textValue = ui->low_tmp_text_edit->toPlainText();
        if(textValue != "")
        {
            float value = textValue.toFloat(&isValid);
            if(isValid)
            {
                _fercase.setLowTempture(value);
            }
            else
            {
                msgBox->setText("格式错误，请输入数值类型!");
                msgBox->show();
                p.setColor(QPalette::Base,Qt::red);
                te->setPalette(p);
                te->setText("");
            }
        }
    }
    else if(te == ui->ae_text_edit)
    {
        textValue = ui->ae_text_edit->toPlainText();
        if(textValue != "")
        {
            int value = textValue.toInt(&isValid);
            if(isValid)
            {
                if(value <= 0)
                {
                    //msgBox->setText("曝气时长必须大于0!");
                    //msgBox->show();
                    p.setColor(QPalette::Base,Qt::red);
                    te->setPalette(p);
                }
                else
                {
                    _fercase.setAe(value);
                }
            }
            else
            {
                msgBox->setText("输入格式错误，请输入大于0的整数!");
                msgBox->show();
                p.setColor(QPalette::Base,Qt::red);
                te->setPalette(p);
                te->setText("");
            }
        }
    }
    else if(te == ui->sta_text_edit)
    {
        textValue = ui->sta_text_edit->toPlainText();
        if(textValue != "")
        {
            int value = textValue.toInt(&isValid);
            if(isValid)
            {
                if(value <= 0)
                {
                    //msgBox->setText("间隔时长必须大于0!");
                    //msgBox->show();
                    p.setColor(QPalette::Base,Qt::red);
                    te->setPalette(p);
                }
                else
                {
                    _fercase.setSta(value);
                }
            }
            else
            {
                msgBox->setText("输入格式错误，请输入大于0的整数!");
                msgBox->show();
                p.setColor(QPalette::Base,Qt::red);
                te->setPalette(p);
                te->setText("");
            }
        }
    }
    else if(te == ui->freq_text_edit)
    {
        textValue = ui->freq_text_edit->toPlainText();
        if(textValue != "")
        {
            int value = textValue.toInt(&isValid);
            if(isValid)
            {
                if(value <= 0)
                {
                    //msgBox->setText("频率必须大于0!");
                    //msgBox->show();
                    p.setColor(QPalette::Base,Qt::red);
                    te->setPalette(p);
                }
                else
                {
                    _fercase.setFreq(value);
                }
            }
            else
            {
                msgBox->setText("输入格式错误，请输入大于0的整数!");
                msgBox->show();
                p.setColor(QPalette::Base,Qt::red);
                te->setPalette(p);
                te->setText("");
            }
        }
    }
}
