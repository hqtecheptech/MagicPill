#include "historydlg.h"
#include "ui_historydlg.h"

#include <QDebug>
#include <math.h>

HistoryDlg::HistoryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoryDlg)
{
    ui->setupUi(this);

    ui->preButton->setEnabled(false);
    ui->nextButton->setEnabled(false);
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

    if(ampere1Values.length() > 0)
    {
        pages = (int)ceil((float)ampere1Values.length() / 15.0);
        currentPage = 1;

        if(currentPage < pages)
        {
            ui->nextButton->setEnabled(true);
        }

        if(currentPage == pages)
        {
            ui->nextButton->setEnabled(false);
        }

        if(ampere1Values.length() > 15)
        {
            for(int i=14; i >= 0; i--)
            {
                ui->widget->updateUI(ampere1Values.at(i).toInt(), 0);
            }
        }
    }
}

void HistoryDlg::on_preButton_clicked()
{
    currentPage -= 1;

    for(int i=14; i >= 0; i--)
    {
        ui->widget->updateUI(ampere1Values.at(currentPage*15 + i).toInt(), 0);
    }

    if(currentPage == 1)
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

    for(int i=14; i >= 0; i--)
    {
        ui->widget->updateUI(ampere1Values.at(currentPage*15 + i).toInt(), 0);
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
