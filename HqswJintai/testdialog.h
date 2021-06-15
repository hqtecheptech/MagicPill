#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>
#include <deodorationfanform.h>

namespace Ui {
class TestDialog;
}

class TestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestDialog(QWidget *parent = 0);
    ~TestDialog();

private:
    Ui::TestDialog *ui;

    DeodorationFanForm *fanForm;
};

#endif // TESTDIALOG_H
