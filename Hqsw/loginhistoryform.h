#ifndef LOGINHISTORYFORM_H
#define LOGINHISTORYFORM_H

#include <QWidget>

namespace Ui {
class loginHistoryForm;
}

class loginHistoryForm : public QWidget
{
    Q_OBJECT

public:
    explicit loginHistoryForm(QWidget *parent = 0);
    ~loginHistoryForm();

private:
    Ui::loginHistoryForm *ui;

};

#endif // LOGINHISTORYFORM_H
