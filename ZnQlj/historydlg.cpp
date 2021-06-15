#include "historydlg.h"
#include "ui_historydlg.h"

#include <QDebug>
#include <math.h>

HistoryDlg::HistoryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoryDlg)
{
    ui->setupUi(this);
    setWindowTitle("历史记录");
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

    ui->preButton->setEnabled(false);
    ui->nextButton->setEnabled(false);

    //Set dialog style.
    ui->bg_frame->setStyleSheet("QFrame#bg_frame{ border-image: url(:/pic/comm_bg.png); border:none; border-radius:5px; }");
}

HistoryDlg::~HistoryDlg()
{
    delete ui;
}

void HistoryDlg::on_oneButton_clicked()
{
    QString db = "hq_yhfp_data.db";
    QString table = "curve_hist";

    QString name = "";
    if(ui->comboBox->currentText() == "压力")
    {
        name = "Yhc_Pressure";
        ui->widget->setRange(300, 100);
        flag = 0;
    }
    else if(ui->comboBox->currentText() == "转速")
    {
        name = "Yhc_Speed";
        ui->widget->setRange(300, 100);
        flag = 1;
    }
    else if(ui->comboBox->currentText() == "电流")
    {
        name = "Yhc_Total_Ampere";
        ui->widget->setRange(500, 450);
        flag = 1;
    }
    else if(ui->comboBox->currentText() == "电压")
    {
        name = "Yhc_Total_Voltage";
        ui->widget->setRange(500, 450);
        flag = 0;
    }
    else if(ui->comboBox->currentText() == "左翻抛压力")
    {
        name = "Fpj_Left_Roller_Pressure";
        ui->widget->setRange(300, 180);
        flag = 0;
    }
    else if(ui->comboBox->currentText() == "左翻抛转速")
    {
        name = "Fpj_Left_Roller_Speed";
        ui->widget->setRange(300, 180);
        flag = 1;
    }
    else if(ui->comboBox->currentText() == "左履带压力")
    {
        name = "Fpj_Left_Walking_Pressure";
        ui->widget->setRange(300, 25000);
        flag = 0;
    }
    else if(ui->comboBox->currentText() == "左履带转速")
    {
        name = "Fpj_Left_Servo_Pulse";
        ui->widget->setRange(300, 25000);
        flag = 1;
    }
    else if(ui->comboBox->currentText() == "右翻抛压力")
    {
        name = "Fpj_Right_Roller_Pressure";
        ui->widget->setRange(300, 180);
        flag = 0;
    }
    else if(ui->comboBox->currentText() == "右翻抛转速")
    {
        name = "Fpj_Right_Roller_Speed";
        ui->widget->setRange(300, 180);
        flag = 1;
    }
    else if(ui->comboBox->currentText() == "右履带压力")
    {
        name = "Fpj_Right_Walking_Pressure";
        ui->widget->setRange(300, 25000);
        flag = 0;
    }
    else if(ui->comboBox->currentText() == "右履带转速")
    {
        name = "Fpj_Right_Servo_Pulse";
        ui->widget->setRange(300, 25000);
        flag = 1;
    }
    else if(ui->comboBox->currentText() == "左履带行走压力")
    {
        name = "SLJ_Left_Walking_Pressure";
        ui->widget->setRange(300, 25000);
        flag = 0;
    }
    else if(ui->comboBox->currentText() == "右履带行走压力")
    {
        name = "SLJ_Right_Walking_Pressure";
        ui->widget->setRange(300, 25000);
        flag = 0;
    }
    else if(ui->comboBox->currentText() == "左履带行走转速")
    {
        name = "SLJ_Left_Servo_Pulse";
        ui->widget->setRange(300, 25000);
        flag = 0;
    }
    else if(ui->comboBox->currentText() == "右履带行走转速")
    {
        name = "SLJ_Right_Servo_Pulse";
        ui->widget->setRange(300, 25000);
        flag = 0;
    }

    QString strSql = "select value from curve_hist where name='" + name + "' order by histId desc;";

    SQLiteHelper::Results.clear();
    dbHelper.select(db, table, strSql);

    reslutValues = SQLiteHelper::Results;
    qDebug() << "Result values counts: " << reslutValues.length();

    ui->preButton->setEnabled(false);
    ui->nextButton->setEnabled(false);

    if(reslutValues.length() > CP)
    {
        pages = (int)floor((float)reslutValues.length() / (float)CP);
        currentPage = 0;

        if(currentPage < pages)
        {
            ui->nextButton->setEnabled(true);
        }

        if(currentPage == pages)
        {
            ui->nextButton->setEnabled(false);
        }

        for(int i=CP-1; i >= 0; i--)
        {
            if(flag == 0)
            {
                ui->widget->updateUI(reslutValues.at(i).toInt(), 0);
            }
            else
            {
                ui->widget->updateUI(0, reslutValues.at(i).toInt());
            }
        }
    }
}

void HistoryDlg::on_preButton_clicked()
{
    currentPage -= 1;
    //qDebug() << "current page:" << currentPage;

    for(int i=CP-1; i >= 0; i--)
    {
        if(flag == 1)
        {
            ui->widget->updateUI(0, reslutValues.at(currentPage*CP + i).toInt());
        }
        else
        {
            ui->widget->updateUI(reslutValues.at(currentPage*CP + i).toInt(), 0);
        }
    }

    if(currentPage == 0)
    {
        ui->preButton->setEnabled(false);
    }

    if(!ui->nextButton->isEnabled())
    {
        ui->nextButton->setEnabled(true);
    }
}

void HistoryDlg::on_nextButton_clicked()
{
    currentPage += 1;
    //qDebug() << "current page:" << currentPage;

    for(int i=CP-1; i >= 0; i--)
    {
        if((currentPage*CP + i) < reslutValues.length())
        {
            if(flag == 1)
            {
                ui->widget->updateUI(0, reslutValues.at(currentPage*CP + i).toInt());
            }
            else
            {
                ui->widget->updateUI(reslutValues.at(currentPage*CP + i).toInt(), 0);
            }
        }
        else
        {
            if(flag == 1)
            {
                ui->widget->updateUI(0, CV);
            }
            else
            {
                ui->widget->updateUI(CV, 0);
            }
        }
    }

    if(currentPage == pages)
    {
        ui->nextButton->setEnabled(false);
    }

    if(!ui->preButton->isEnabled())
    {
        ui->preButton->setEnabled(true);
    }
}

void HistoryDlg::on_closeButton_clicked()
{
    close();
}

void HistoryDlg::setQueryItems(QStringList items)
{
    ui->comboBox->clear();
    ui->comboBox->addItems(items);
}
