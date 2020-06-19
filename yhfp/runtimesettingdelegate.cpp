#include "runtimesettingdelegate.h"

#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include "mixsettingdialog.h"

RuntimeSettingDelegate::RuntimeSettingDelegate(SettingType settingType, QObject *parent) :
    QItemDelegate(parent)
{
    _settingType = settingType;
    msgBox = new QMessageBox();
    msgBox->setStyleSheet(
            "QPushButton {"
                " background-color:#89AFDE;"
                " border-style: outset;"
                " border-width: 2px;"
                " border-radius: 10px;"
                " border-color: beige;"
                " font: bold 24px;"
                " min-width: 5em;"
                " min-height: 5em;"
            "}"
            "QLabel { min-width: 20em;min-height:10em;font:24px;background-color:#89AFDE;padding:10px;}"
        );

    connect(this,SIGNAL(editorCreated(QWidget*,int)),this,SLOT(copyEditor(QWidget*,int)));
}

QWidget *RuntimeSettingDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //QComboBox *editor = new QComboBox(parent);
    Keyboard *keyboard = Keyboard::getInstance();
    QLineEdit *editor = new QLineEdit(parent);
    editor->setFont(QFont("Timers" , 28 ,  QFont::Bold));
    editor->setAlignment( Qt::AlignHCenter);
    editor->installEventFilter(const_cast<RuntimeSettingDelegate*>(this));
    connect(editor, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_lineEdit()));

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
    float value = valueStr.toFloat(&ok);
    if(!ok)
    {
        //msgBox->setText(QStringLiteral("数字格式不正确"));
        //msgBox->show();
        emit inputInfo(QStringLiteral("数字格式不正确"));
    }
    else
    {
        switch (_settingType) {
        case Second:            
        case Minute:
            if(value < 2 || value > 59)
            {
                //msgBox->setText(QStringLiteral("值必须大于等于2小于等于59"));
                //msgBox->show();
                emit inputInfo(QStringLiteral("值必须大于等于2小于等于59"));
            }
            else
            {
                model->setData(index,valueStr);
            }
            break;
        case Day:
            if(value < 0 || value > 365)
            {
                //msgBox->setText(QStringLiteral("值必须大于等于0小于等于365"));
                //msgBox->show();
                emit inputInfo(QStringLiteral("值必须大于等于0小于等于365"));
            }
            else
            {
                model->setData(index,valueStr);
            }
            break;
        case Hour:
            if(value < 0 || value > 23)
            {
                //msgBox->setText(QStringLiteral("值必须大于等于0小于等于23"));
                //msgBox->show();
                emit inputInfo(QStringLiteral("值必须大于等于0小于等于23"));
            }
            else
            {
                model->setData(index,valueStr);
            }
            break;
        case Rate:
            if(value <= 100 || value >= 30)
            {
                //msgBox->setText(QStringLiteral("值必须大于等于30小于等于100"));
                //msgBox->show();
                emit inputInfo(QStringLiteral("值必须大于等于30小于等于100"));
            }
            else
            {
                model->setData(index,valueStr);
            }
            break;
        case Range:
            model->setData(index,valueStr);
            emit dataChanged(value, index.row());
            break;
        default:
            break;
        }
    }

    emit editorCreated(NULL,-1);
}

void RuntimeSettingDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
    emit editorCreated(editor,index.row());
}

void RuntimeSettingDelegate::drawCheck(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, Qt::CheckState state) const
{

}

bool RuntimeSettingDelegate::eventFilter(QObject *watched, QEvent *event)
{
    QLineEdit *edit = static_cast<QLineEdit*>(watched);

    if(event->type() == QEvent::MouseButtonPress)
    {
        emit edit->selectionChanged();
    }

    return false;
}

void RuntimeSettingDelegate::setCurrentEditor(QWidget *value)
{
    currentEditor = value;
}

void RuntimeSettingDelegate::copyEditor(QWidget *editor, int row)
{
    currentEditor = editor;
    currentRow = row;
}

int RuntimeSettingDelegate::getCurrentRow() const
{
    return currentRow;
}

void RuntimeSettingDelegate::setCurrentRow(int value)
{
    currentRow = value;
}

void RuntimeSettingDelegate::closeMsgBox()
{
    msgBox->close();
}

QWidget *RuntimeSettingDelegate::getCurrentEditor() const
{
    return currentEditor;
}
