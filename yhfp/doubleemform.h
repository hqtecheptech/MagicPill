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
    bool emRunStatus;
    bool emFaultStatus;
    bool isCoro1;
    bool isInve1;
    bool isCoro2;
    bool isInve2;
    bool stateFlag;
};

#endif // DOUBLEEMFORM_H
