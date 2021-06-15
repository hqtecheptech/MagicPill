#ifndef YHCDIAL_H
#define YHCDIAL_H

#include <QWidget>

namespace Ui {
class yhcDial;
}

class yhcDial : public QWidget
{
    Q_OBJECT

public:
    explicit yhcDial(QWidget *parent = 0);
    ~yhcDial();

private slots:
    void on_upButton_released();
    void on_upButton_pressed();
    void on_downButton_released();
    void on_downButton_pressed();
    void on_leftButton_released();
    void on_leftButton_pressed();
    void on_rightButton_released();
    void on_rightButton_pressed();

private:
    Ui::yhcDial *ui;

    QString upButtonStyleStr;
};

#endif // YHCDIAL_H
