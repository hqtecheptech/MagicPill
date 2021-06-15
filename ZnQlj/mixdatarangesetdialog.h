#ifndef MIXDATARANGESETDIALOG_H
#define MIXDATARANGESETDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QMessageBox>

#include "runtimesettingdelegate.h"

namespace Ui {
class MixDataRangeSetDialog;
}

class MixDataRangeSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MixDataRangeSetDialog(QWidget *parent = 0);
    ~MixDataRangeSetDialog();

    void closeMsgBox();

public slots:
    void showEditorInfo(QString info);
    bool eventFilter(QObject *, QEvent *);
    void updateLeftRangeData(float value, int row);
    void updateRightRangeData(float value, int row);

private slots:
    void on_close_pushButton_clicked();
    void on_save_pushButton_clicked();

private:
    Ui::MixDataRangeSetDialog *ui;

    QStandardItemModel *deviceNodeModel;
    RuntimeSettingDelegate *leftRangeDelegate;
    RuntimeSettingDelegate *rightRangeDelegate;
    QVector<DeviceNode> changedNodes;
    QMessageBox *msgBox;
};

#endif // MIXDATARANGESETDIALOG_H
