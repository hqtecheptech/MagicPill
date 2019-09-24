#ifndef FPJWATCHS_H
#define FPJWATCHS_H

#include <QWidget>

namespace Ui {
class FpjWatchs;
}

class FpjWatchs : public QWidget
{
    Q_OBJECT

public:
    explicit FpjWatchs(QWidget *parent = 0);
    ~FpjWatchs();

private:
    Ui::FpjWatchs *ui;
};

#endif // FPJWATCHS_H
