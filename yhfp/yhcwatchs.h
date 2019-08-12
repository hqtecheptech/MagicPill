#ifndef YHCWATCHS_H
#define YHCWATCHS_H

#include <QWidget>

namespace Ui {
class yhcWatchs;
}

class yhcWatchs : public QWidget
{
    Q_OBJECT

public:
    explicit yhcWatchs(QWidget *parent = 0);
    ~yhcWatchs();

public slots:
    void updateDydl(int leftValue, int rightValue);
    void updateWdyw(int leftValue, int rightValue);

private:
    Ui::yhcWatchs *ui;
};

#endif // YHCWATCHS_H
