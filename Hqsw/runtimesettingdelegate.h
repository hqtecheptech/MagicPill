#ifndef RUNTIMESETTINGDELEGATE_H
#define RUNTIMESETTINGDELEGATE_H

#include "dataformat.h"

#include <QItemDelegate>
#include <QMessageBox>

class RuntimeSettingDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    RuntimeSettingDelegate(SettingType timeType, QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void drawCheck(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, Qt::CheckState state) const;

private:
    SettingType _timeType;
    QMessageBox *msgBox;
};

#endif // RUNTIMESETTINGDELEGATE_H
