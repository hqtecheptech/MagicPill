#ifndef RUNTIMESETTINGDELEGATE_H
#define RUNTIMESETTINGDELEGATE_H

#include "dataformat.h"

#include <QItemDelegate>
#include <QMessageBox>
#include "keyboard.h"

class RuntimeSettingDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    RuntimeSettingDelegate(SettingType settingType, QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawCheck(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, Qt::CheckState state) const;

    QWidget *getCurrentEditor() const;

    void setCurrentEditor(QWidget *value);

    int getCurrentRow() const;
    void setCurrentRow(int value);

    void closeMsgBox();

signals:
    void editorCreated(QWidget *editor, int row) const;
    void inputInfo(QString info) const;

protected:
    bool eventFilter(QObject *, QEvent *);

private slots:
    void copyEditor(QWidget *editor, int row);

private:
    SettingType _settingType;
    QMessageBox *msgBox;
    QWidget *currentEditor = NULL;
    int currentRow = -1;
};

#endif // RUNTIMESETTINGDELEGATE_H
