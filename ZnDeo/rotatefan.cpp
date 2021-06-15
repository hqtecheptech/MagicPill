#include "rotatefan.h"
#include "ui_rotatefan.h"

#include <QPainter>
#include <QDebug>
#include <QSize>

RotateFan::RotateFan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RotateFan)
{
    ui->setupUi(this);

    degree = 0;
    fanNegBgImg.load("://image/fan_neg.png");
    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    setDuration(500);
}

RotateFan::~RotateFan()
{
    delete ui;
}

QSize RotateFan::sizeHint() const
{
    return QSize(fanNegBgImg.width(), fanNegBgImg.height());
}

void RotateFan::setDuration(int value)
{
    if(value > 0 && duration != value)
    {
        duration = value;
        if(updateTimer->timerId() == -1)
        {
            updateTimer->start(value);
        }
        else
        {
            updateTimer->stop();
            updateTimer->start(value);
        }
    }
    else if(value < 0)
    {
        if(updateTimer->timerId() != -1)
        {
            updateTimer->stop();
        }
    }
    duration = value;
}

void RotateFan::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    /*painter.save();
        degree = (degree++) % 8;
            qDebug() << "width = " << width();
            qDebug() << "height = " << height();

            int side = qMin(width(),height());
            qDebug() << "side = " << side;
            painter.setViewport((width() - side) / 2, (height() - side) / 2, 50, 50);
            painter.setWindow(-25, -25, 50, 50);

    QTransform transForm;
    transForm.translate(0, 0);
    transForm.rotate(45.0 * degree);
    painter.setWorldTransform(transForm);
    painter.drawPixmap(-25, -25, fanNegBgImg);
    painter.restore();*/
    degree = (degree++) % 8;
    painter.translate(25,25);
    painter.rotate(45.0 * degree);
    painter.translate(-25,-25);

    painter.drawPixmap(0, 0, fanNegBgImg);
}
