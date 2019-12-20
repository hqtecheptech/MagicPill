#ifndef SIMPLEDEOCONTROLDIALOG_H
#define SIMPLEDEOCONTROLDIALOG_H

#include <QDialog>

namespace Ui {
class SimpleDeoControlDialog;
}

class SimpleDeoControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SimpleDeoControlDialog(QWidget *parent = 0);
    ~SimpleDeoControlDialog();

private:
    Ui::SimpleDeoControlDialog *ui;
};

#endif // SIMPLEDEOCONTROLDIALOG_H
