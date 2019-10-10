#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fftank.h"
#include "centerwidget.h"
#include "tankstatebarchart.h"
#include "tanghe_centerwidget.h"
#include "renhuai_centerwidget.h"
#include "renhuainew.h"

#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar->setVisible(false);
    //CenterWidget *cw = new CenterWidget(this);
    //setCentralWidget(cw);
    //TangHe_CenterWidget *tcw = new TangHe_CenterWidget(this);
    //setCentralWidget(tcw);
    //RenHuai_CenterWidget *rcw = new RenHuai_CenterWidget(this);
    RenhuaiNew *rcw = new RenhuaiNew(this);
    setCentralWidget(rcw);

    setGeometry(QRect(0, 0, 1920, 1080));
    resize(1920,1080);
    setFixedSize(1920,1080);
    setWindowFlags(Qt::FramelessWindowHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}
