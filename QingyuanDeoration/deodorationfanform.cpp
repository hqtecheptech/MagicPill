#include "deodorationfanform.h"
#include "ui_deodorationfanform.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

DeodorationFanForm::DeodorationFanForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeodorationFanForm)
{
    ui->setupUi(this);

    LixinfanBgImg.load("://image/lixinfan.png");
    QLabel *fanLabel = new QLabel();
    fanLabel->setPixmap(LixinfanBgImg);
    fanLabel->resize(LixinfanBgImg.width(), LixinfanBgImg.height());

    qDebug() << "fanLabel Width" << fanLabel->width();

    valveFront = new TankAirValveForm();
    valveBack = new TankAirValveForm();

    QHBoxLayout *upLayout = new QHBoxLayout();
    upLayout->setMargin(0);
    upLayout->setSpacing(0);
    upLayout->setSizeConstraint(QLayout::SetFixedSize);
    upLayout->addWidget(valveFront);
    upLayout->addWidget(fanLabel);
    upLayout->addWidget(valveBack);

    QLabel *fanStateLabel = new QLabel();
    fanStateLabel->setText("Open");
    fanStateLabel->setMargin(10);
    fanStateLabel->setAlignment(Qt::AlignHCenter);
    rotateFan = new RotateFan();
    QVBoxLayout *downLayout = new QVBoxLayout();
    downLayout->setSpacing(0);
    downLayout->setMargin(0);
    downLayout->setSizeConstraint(QLayout::SetFixedSize);
    downLayout->addWidget(fanStateLabel);
    downLayout->addWidget(rotateFan);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(upLayout);
    mainLayout->addLayout(downLayout);
    mainLayout->setAlignment(downLayout, Qt::AlignHCenter);

    setLayout(mainLayout);
}

DeodorationFanForm::~DeodorationFanForm()
{
    delete ui;
}
