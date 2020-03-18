#ifndef DOUBLEEMFORM_H
#define DOUBLEEMFORM_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class DoubleEmForm;
}

class DoubleEmForm : public QWidget
{
    Q_OBJECT

public:
    explicit DoubleEmForm(QWidget *parent = 0);
    ~DoubleEmForm();

    void setStatus(bool emRum, bool emFault, bool coro1, bool inve1, bool coro2, bool inve2);

private slots:
    void switchStatus();

private:
    Ui::DoubleEmForm *ui;

    QTimer *ssTimer;
    bool emRunStatus = false;
    bool emFaultStatus = false;
    bool isCoro1 = false;
    bool isInve1 = false;
    bool isCoro2 = false;
    bool isInve2 = false;
    bool stateFlag = false;
};

#endif // DOUBLEEMFORM_H
