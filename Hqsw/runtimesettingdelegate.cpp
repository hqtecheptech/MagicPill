#include "runtimesettingdelegate.h"

#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>

RuntimeSettingDelegate::RuntimeSettingDelegate(SettingType timeType, QObject *parent) :
    QItemDelegate(parent)
{
    _timeType = timeType;
    msgBox = new QMessageBox();
}

QWidget *RuntimeSettingDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //QComboBox *editor = new QComboBox(parent);
    QLineEdit *editor = new QLineEdit(parent);
    editor->installEventFilter(const_cast<RuntimeSettingDelegate*>(this));

    return editor;
}

void RuntimeSettingDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString valueStr = index.model()->data(index).toString();
    //QComboBox *box = static_cast<QComboBox*>(editor);
    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    edit->setText(valueStr);
}

void RuntimeSettingDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //QComboBox *box = static_cast<QComboBox*>(editor);
    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    QString valueStr = edit->text();
    bool ok;
    uint value = valueStr.toUInt(&ok);
    if(!ok)
    {
        msgBox->setText(QStringLiteral("数字格式不正确"));
        msgBox->show();
    }
    else
    {
        switch (_timeType) {
        case Second:            
        case Minute:
            if(value < 0 || value > 59)
            {
                msgBox->setText(QStringLiteral("值必须大于等于0小于等于59"));
                msgBox->show();
            }
            else
            {
                model->setData(index,valueStr);
            }
            break;
        case Day:
            if(value < 0 || value > 365)
            {
                msgBox->setText(QStringLiteral("值必须大于等于0小于等于365"));
                msgBox->show();
            }
            else
            {
                model->setData(index,valueStr);
            }
            break;
        case Hour:
            if(value < 0 || value > 23)
            {
                msgBox->setText(QStringLiteral("值必须大于等于0小于等于23"));
                msgBox->show();
            }
            else
            {
                model->setData(index,valueStr);
            }
            break;
        default:
            break;
        }
    }
}

void RuntimeSettingDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void RuntimeSettingDelegate::drawCheck(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, Qt::CheckState state) const
{

}
