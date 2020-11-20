#ifndef ROTATEFAN_H
#define ROTATEFAN_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class RotateFan;
}

class RotateFan : public QWidget
{
    Q_OBJECT

public:
    explicit RotateFan(QWidget *parent = 0);
    ~RotateFan();

    void setDuration(int value);

protected:
    void paintEvent(QPaintEvent *event);
    QSize sizeHint() const;

private:
    Ui::RotateFan *ui;
    QTimer *updateTimer;
    QPixmap fanNegBgImg;
    int degree;
    int duration;
};

#endif // ROTATEFAN_H
