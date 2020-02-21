#ifndef SINGLEEMFORM_H
#define SINGLEEMFORM_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class SingleEmForm;
}

class SingleEmForm : public QWidget
{
    Q_OBJECT

public:
    explicit SingleEmForm(QWidget *parent = 0);
    ~SingleEmForm();

    void setStatus(bool emRum, bool emFault, bool coro, bool inve, bool unLoading);

private slots:
    void switchStatus();

private:
    Ui::SingleEmForm *ui;

    QTimer *ssTimer;
    bool emRunStatus;
    bool emFaultStatus;
    bool isCoro;
    bool isInve;
    bool isUnLoading;
    bool stateFlag;
};

#endif // SINGLEEMFORM_H
