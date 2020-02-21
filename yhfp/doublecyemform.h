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
    bool emRunStatus;
    bool emFaultStatus;
    bool isCoro1;
    bool isInve1;
    bool isCoro2;
    bool isInve2;
    bool stateFlag;
};

#endif // DOUBLECYEMFORM_H
