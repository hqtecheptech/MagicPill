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
    rsSeries = new QLineSeries();
    rsSeries->setPen(QPen(Qt::green,2,Qt::SolidLine));

    *rsSeries << QPoint(0, 5);
    rsValues[0] = 5;

    prsSeries = new QLineSeries();
    prsSeries->setPen(QPen(Qt::red,2,Qt::SolidLine));

    *prsSeries << QPoint(0, 5);
    prsValues[0] = 5;

    /*for(int i=0; i<5; ++i)
    {
        int x = 2*i;
        int y = 2*i;
        *series << QPoint(x, y);
    }*/

    chart->addSeries(rsSeries);
    chart->addSeries(prsSeries);
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

    chart->setAxisX(axisX, rsSeries);
    chart->setAxisY(axisY, rsSeries);

    chart->setAxisX(axisX, prsSeries);
    chart->setAxisY(axisY, prsSeries);

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

void ZsylChart::updateUI(int rsValue, int prsValue)
{
    serieValuesCount++;

    if(serieValuesCount < 15)
    {
        rsValues[serieValuesCount] = rsValue;
        *rsSeries << QPoint(serieValuesCount, rsValue);

        prsValues[serieValuesCount] = prsValue;
        *prsSeries << QPoint(serieValuesCount, prsValue);
    }
    else
    {
        for(int i=0; i < 14; i++)
        {
            rsValues[i] = rsValues[i+1];
            prsValues[i] = prsValues[i+1];
        }
        rsValues[14] = rsValue;
        prsValues[14] = prsValue;

        rsSeries->clear();
        chart->removeSeries(rsSeries);

        prsSeries->clear();
        chart->removeSeries(prsSeries);

        for(int j=0; j < 15; j++)
        {
            *rsSeries << QPoint(j, rsValues[j]);
            *prsSeries << QPoint(j, prsValues[j]);
        }
        chart->addSeries(rsSeries);
        chart->addSeries(prsSeries);
    }
}
