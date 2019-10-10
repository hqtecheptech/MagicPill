#include "mainwindow.h"
#include <QApplication>
#include "ufftank.h"
#include "fftank.h"
#include "centerwidget.h"

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // new for immidiately exit with window closed.
    MainWindow *w = new MainWindow;
    w->show();

    return a.exec();
}
