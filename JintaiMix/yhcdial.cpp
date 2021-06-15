#include "yhcdial.h"
#include "ui_yhcdial.h"

yhcDial::yhcDial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::yhcDial)
{
    ui->setupUi(this);

    upButtonStyleStr="QPushButton#upButton{background: transparent; background-image: url(:/pic/上.png);}"
                         "QPushButton#upButton:hover{background: transparent; background-image: url(:/pic/上2.png);}"
                         "QPushButton#upButton:pressed{background: transparent; background-image: url(:/pic/上2.png);}";

    //ui->upButton->setStyleSheet(upButtonStyleStr);
    ui->upButton->setStyleSheet("QPushButton#upButton{background: transparent; background-image: url(:/pic/上.png);}");
    ui->downButton->setStyleSheet("QPushButton#downButton{background: transparent; background-image: url(:/pic/下.png);}");
    ui->leftButton->setStyleSheet("QPushButton#leftButton{background: transparent; background-image: url(:/pic/左.png);}");
    ui->rightButton->setStyleSheet("QPushButton#rightButton{background: transparent; background-image: url(:/pic/右.png);}");
}

yhcDial::~yhcDial()
{
    delete ui;
}

void yhcDial::on_upButton_released()
{
    ui->upButton->setStyleSheet("QPushButton#upButton{background: transparent; background-image: url(:/pic/上.png);}");
}

void yhcDial::on_upButton_pressed()
{
    ui->upButton->setStyleSheet("QPushButton#upButton{background: transparent; background-image: url(:/pic/上2.png);}");
}

void yhcDial::on_downButton_released()
{
    ui->downButton->setStyleSheet("QPushButton#downButton{background: transparent; background-image: url(:/pic/下.png);}");
}

void yhcDial::on_downButton_pressed()
{
    ui->downButton->setStyleSheet("QPushButton#downButton{background: transparent; background-image: url(:/pic/下2.png);}");
}

void yhcDial::on_leftButton_released()
{
    ui->leftButton->setStyleSheet("QPushButton#leftButton{background: transparent; background-image: url(:/pic/左.png);}");
}

void yhcDial::on_leftButton_pressed()
{
    ui->leftButton->setStyleSheet("QPushButton#leftButton{background: transparent; background-image: url(:/pic/左2.png);}");
}

void yhcDial::on_rightButton_released()
{
    ui->rightButton->setStyleSheet("QPushButton#rightButton{background: transparent; background-image: url(:/pic/右.png);}");
}

void yhcDial::on_rightButton_pressed()
{
    ui->rightButton->setStyleSheet("QPushButton#rightButton{background: transparent; background-image: url(:/pic/右2.png);}");
}
