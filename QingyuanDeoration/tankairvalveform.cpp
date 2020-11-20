#include "tankairvalveform.h"
#include "ui_tankairvalveform.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QSizePolicy>

TankAirValveForm::TankAirValveForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TankAirValveForm)
{
    ui->setupUi(this);

    QImage image ;
    image.load("://image/hp1.png");

    valveCloseBgImg.load("://image/valve_close.png");
    valveOpenBgImg.load("://image/valve_open.png");
    hPipeBgImg.load("://image/hp1.png");

    qDebug() << "hPipeBgImg Width" << hPipeBgImg.width();
    qDebug() << "valveOpenBgImg Width" << valveOpenBgImg.width();

    QLabel *l1,*l2,*l3;
    l1 = new QLabel();
    l2 = new QLabel();
    l3 = new QLabel();
    l1->setPixmap(QPixmap::fromImage(image));
    //l1->resize(QSize(image.width(),image.height()));
    l2->setPixmap(valveOpenBgImg);
    //l2->resize(QSize(valveOpenBgImg.width(),valveOpenBgImg.height()));
    l3->setPixmap(QPixmap::fromImage(image));
    //l3->resize(QSize(image.width(),image.height()));

    QHBoxLayout *upLayout = new QHBoxLayout();
    upLayout->setSpacing(0);
    upLayout->setMargin(0);
    upLayout->addWidget(l1);
    upLayout->addWidget(l2);
    upLayout->addWidget(l3);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addLayout(upLayout);

    setLayout(layout);
}

TankAirValveForm::~TankAirValveForm()
{
    delete ui;
}
