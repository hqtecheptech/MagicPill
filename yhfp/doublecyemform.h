#ifndef DOUBLECYEMFORM_H
#define DOUBLECYEMFORM_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class DoubleCyEmForm;
}

class DoubleCyEmForm : public QWidget
{
    Q_OBJECT

public:
    explicit DoubleCyEmForm(QWidget *parent = 0);
    ~DoubleCyEmForm();

    void setStatus(bool emRum, bool emFault, bool coro1, bool inve1, bool coro2, bool inve2);

private slots:
    void switchStatus();

private:
    Ui::DoubleCyEmForm *ui;

    QTimer *ssTimer;
    bool emRunStatus = false;
    bool emFaultStatus = false;
    bool isCoro1 = false;
    bool isInve1 = false;
    bool isCoro2 = false;
    bool isInve2 = false;
    bool stateFlag = false;
};

#endif // DOUBLECYEMFORM_H
