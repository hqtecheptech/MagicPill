#include "fercaseform.h"
#include "ui_fercaseform.h"

FerCaseForm::FerCaseForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FerCaseForm)
{
    ui->setupUi(this);
    ui->low_tmp_text_edit->installEventFilter(this);
    ui->high_tmp_text_edit->installEventFilter(this);
    ui->ae_text_edit->installEventFilter(this);
    ui->sta_text_edit->installEventFilter(this);
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
}

QString FerCaseForm::checkCaseValidation()
{
    return _fercase.checkCaseValidation();
}

void FerCaseForm::setCaseSequence(int seq)
{
    ui->seq_label->setText(QString::number(seq+1));
}

bool FerCaseForm::eventFilter(QObject *watched, QEvent *event)
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

        if(watched == ui->high_tmp_text_edit)
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
                    msgBox.setText("格式错误，请输入数值类型!");
                    msgBox.show();
                }
            }
        }
        else if(watched == ui->low_tmp_text_edit)
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
                    msgBox.setText("格式错误，请输入数值类型!");
                    msgBox.show();
                }
            }
        }
        else if(watched == ui->ae_text_edit)
        {
            textValue = ui->ae_text_edit->toPlainText();
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
                        _fercase.setAe(value);
                    }
                }
                else
                {
                    msgBox.setText("输入格式错误，请输入大于0的整数!");
                    msgBox.show();
                }
            }
        }
        else if(watched == ui->sta_text_edit)
        {
            textValue = ui->sta_text_edit->toPlainText();
            if(textValue != "")
            {
                int value = textValue.toInt(&isValid);
                if(isValid)
                {
                    if(value <= 0)
                    {
                        msgBox.setText("间隔时长必须大于0!");
                        msgBox.show();
                    }
                    else
                    {
                        _fercase.setSta(value);
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
