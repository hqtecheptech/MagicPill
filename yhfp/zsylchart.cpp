#include "zsylchart.h"
#include "ui_zsylchart.h"
#include <QByteArray>

ZsylChart::ZsylChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZsylChart)
{
    ui->setupUi(this);

    chart = new QChart;
    chart->setMargins(QMargins(13,0,0,45));
    chart->setTheme(QChart::ChartThemeBlueNcs);
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
    chartView = new QChartView(this);
    chartView->resize(420, 120);
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QByteArray data;
    showAllData(data);

    //ui->horizontalLayout->addWidget(chartView);
    //ui->horizontalLayout->setAlignment(chartView, Qt::AlignCenter);
}

ZsylChart::~ZsylChart()
{
    delete ui;
}

void ZsylChart::showEvent(QShowEvent *event)
{

}

void ZsylChart::closeEvent(QCloseEvent *event)
{

}

void ZsylChart::showRealTime(QByteArray)
{

}

void ZsylChart::showAllData(QByteArray data)
{
    series = new QLineSeries();
    series->setPen(QPen(Qt::red,1,Qt::SolidLine));

    *series << QPoint(0, 5);
    values[0] = 5;
    /*for(int i=0; i<5; ++i)
    {
        int x = 2*i;
        int y = 2*i;
        *series << QPoint(x, y);
    }*/

    chart->addSeries(series);
    //chart->setTitle(ui->comboBox->currentText());
    //chart->setAnimationOptions(QChart::SeriesAnimations);//设置曲线呈动画显示

    axisY = new QValueAxis; //定义Y轴
    axisY->setRange(0, 33); //设置范围
    axisY->setLabelFormat("%g"); //设置刻度的格式
    axisY->setGridLineVisible(false); //设置是否显示网格线
    axisY->setTickCount(12);
    axisY->setLabelsBrush(QBrush(QColor(255, 255, 255)));
    //axisY->setLabelsFont(QFont("Microsoft YaHei", 50, QFont::Bold));

    axisX = new QValueAxis;
    axisX->setRange(0, 30);
    axisX->setLabelFormat(QStringLiteral("%g"));
    axisX->setGridLineVisible(false);
    axisX->setTickCount(16);
    axisX->setLabelsBrush(QBrush(QColor(255, 255, 255)));
    //axisX->setLabelsFont(QFont("Microsoft YaHei", 50, QFont::Bold));

    chart->setAxisX(axisX, series);
    chart->setAxisY(axisY, series);
    chart->axisX()->hide();
    chart->axisY()->hide();

    chart->setBackgroundVisible(false);

    resetChartSuccess = true;
    //qDebug() << "Reset chart success!";
}

void ZsylChart::resetChart()
{
    chart->removeAxis(axisX);
    chart->removeAxis(axisY);
    chart->removeAllSeries();
    resetChartSuccess = false;
}

void ZsylChart::updateUI(int newValue)
{
    serieValuesCount++;

    if(serieValuesCount < 15)
    {
        values[serieValuesCount] = newValue;
        *series << QPoint(serieValuesCount, newValue);
    }
    else
    {
        for(int i=0; i < 14; i++)
        {
            values[i] = values[i+1];
        }
        values[14] = newValue;

        series->clear();
        chart->removeSeries(series);
        for(int j=0; j < 15; j++)
        {
            *series << QPoint(j, values[j]);
        }
        chart->addSeries(series);
    }
}
