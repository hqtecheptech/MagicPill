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
    QString strSql = "select value from curve_hist where name='Ampere1' order by histId desc;";
    SQLiteHelper::Results.clear();
    dbHelper.select(db, table, strSql);

    ampere1Values = SQLiteHelper::Results;
    qDebug() << "Ampere1 values counts: " << ampere1Values.length();

    ui->preButton->setEnabled(false);
    ui->nextButton->setEnabled(false);

    if(ampere1Values.length() > CP)
    {
        pages = (int)floor((float)ampere1Values.length() / (float)CP);
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
            ui->widget->updateUI(0, ampere1Values.at(i).toInt());
        }
    }
}

void HistoryDlg::on_preButton_clicked()
{
    currentPage -= 1;
    qDebug() << "current page:" << currentPage;

    for(int i=CP-1; i >= 0; i--)
    {
        ui->widget->updateUI(0, ampere1Values.at(currentPage*CP + i).toInt());
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
    qDebug() << "current page:" << currentPage;

    for(int i=CP-1; i >= 0; i--)
    {
        if((currentPage*CP + i) < ampere1Values.length())
        {
            ui->widget->updateUI(0, ampere1Values.at(currentPage*CP + i).toInt());
        }
        else
        {
            ui->widget->updateUI(0, CV);
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
