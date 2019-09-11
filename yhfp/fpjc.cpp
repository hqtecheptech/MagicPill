#include "fpjc.h"
#include "ui_fpjc.h"

#include <QMap>
#include <QVector>

Fpjc::Fpjc(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fpjc)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    deviceIndex = 0;

    st.start();

    qRegisterMetaType<Plc_Db>("Plc_Db");

    QString eixtStyleStr="QPushButton#yhcExitButton{background: transparent; background-image: url(:/pic/退出.png);}"
                         "QPushButton#yhcExitButton:hover{background: transparent; background-image: url(:/pic/退出.png);}"
                         "QPushButton#yhcExitButton:pressed{background: transparent; background-image: url(:/pic/退出.png);}";
    ui->yhcExitButton->setStyleSheet(eixtStyleStr);

    ui->speedDownButton->setStyleSheet("QPushButton#speedDownButton{background: transparent; background-image: url(:/pic/_02.png);}");
    ui->speedUpButton->setStyleSheet("QPushButton#speedUpButton{background: transparent; background-image: url(:/pic/+02.png);}");

    ui->yhDownButton->setStyleSheet("QPushButton#yhDownButton{background: transparent; background-image: url(:/pic/_02.png);}");
    ui->yhUpButton->setStyleSheet("QPushButton#yhUpButton{background: transparent; background-image: url(:/pic/+02.png);}");

    QString speedLabelStyleStr = "background: transparent;"
                                 "background-position:center;"
                                 "background-repeat:no-repeat;"
                                 "background-image: url(:/pic/输入框.png);"
                                 "color:white;";
    ui->speedLabel->setStyleSheet(speedLabelStyleStr);
    ui->yhChoiseLabel->setStyleSheet(speedLabelStyleStr);
}

Fpjc::~Fpjc()
{
    delete ui;
}

void Fpjc::on_yhcExitButton_clicked()
{
    close();
}
