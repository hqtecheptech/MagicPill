#include "checkboxdelegate.h"

#include <QApplication>
#include <QMouseEvent>

CheckBoxDelegate::CheckBoxDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}

void CheckBoxDelegate::drawCheck(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, Qt::CheckState state) const
{
    /*const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;

    QRect checkRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
                        check(option, option.rect, Qt::Checked).size(),
                        QRect(option.rect.x() + textMargin, option.rect.y(),
                        option.rect.width() - (textMargin * 2), option.rect.height()));
    QItemDelegate::drawCheck(painter, option, checkRect, state);*/

    const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;

    QRect checkRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
                        option.rect.size(),
                        QRect(option.rect.x() + textMargin, option.rect.y(),
                        option.rect.width() - (textMargin * 2), option.rect.height()));
    QItemDelegate::drawCheck(painter, option, checkRect, state);
}

void CheckBoxDelegate::drawFocus(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect) const
{
    QItemDelegate::drawFocus(painter, option, option.rect);
}

bool CheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    // make sure that the item is checkable
    Qt::ItemFlags flags = model->flags(index);
    if (!(flags & Qt::ItemIsUserCheckable) || !(flags & Qt::ItemIsEnabled))
        return false;
    // make sure that we have a check state
    QVariant value = index.data(Qt::CheckStateRole);
    if (!value.isValid())
        return false;
    // make sure that we have the right event type
    if (event->type() == QEvent::MouseButtonRelease) {
        const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
        QRect checkRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
            option.rect.size(),
            QRect(option.rect.x() + textMargin, option.rect.y(),
            option.rect.width() - (2 * textMargin), option.rect.height()));
        if (!checkRect.contains(static_cast<QMouseEvent*>(event)->pos()))
        {
            return false;
        }
        else
        {
            bool data = model->data(index, Qt::UserRole).toBool();
            model->setData(index, !data, Qt::UserRole);
        }
    }
    /*else if (event->type() == QEvent::KeyPress) {
        if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space
            && static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select)
            return false;
    } */
    else
    {
        return false;
    }
    Qt::CheckState state = (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked
        ? Qt::Unchecked : Qt::Checked);

    /*QMessageBox::information(0,
        QString((state == Qt::Checked) ? "Qt::Checked" : "Qt::Unchecked"),
        QString("[%1/%2]").arg(index.row()).arg(index.column()));*/

    return model->setData(index, state, Qt::CheckStateRole);
    //return QItemDelegate::editorEvent(event, model, option, index);
}
