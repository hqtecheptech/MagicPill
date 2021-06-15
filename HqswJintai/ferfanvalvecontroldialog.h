#ifndef FERFANVALVECONTROLDIALOG_H
#define FERFANVALVECONTROLDIALOG_H

#include <QDialog>

namespace Ui {
class FerFanValveControlDialog;
}

class FerFanValveControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FerFanValveControlDialog(QWidget *parent = 0);
    ~FerFanValveControlDialog();

private:
    Ui::FerFanValveControlDialog *ui;
};

#endif // FERFANVALVECONTROLDIALOG_H
