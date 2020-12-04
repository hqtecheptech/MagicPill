#ifndef CONTROLHISTORY_H
#define CONTROLHISTORY_H

#include <QWidget>
#include "uiglobal.h"

namespace Ui {
class ControlHistory;
}

class ControlHistory : public QWidget
{
    Q_OBJECT

public:
    explicit ControlHistory(QWidget *parent = 0);
    ~ControlHistory();

private:
    Ui::ControlHistory *ui;
};

#endif // CONTROLHISTORY_H
