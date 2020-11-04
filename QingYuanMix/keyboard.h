#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QtGui>
#include <QtSql>
#include <QTimer>

class QTextEdit;
class QLineEdit;
class QPushButton;

enum EditType {
    LineEdit,
    TextEdit
} ;

namespace Ui {
class Keyboard;
}

#define N_CANDIDATE 7

class Keyboard : public QWidget
{
    Q_OBJECT
public:
    static Keyboard *getInstance();
private:
    explicit Keyboard(QWidget *parent = 0);
    Keyboard(const Keyboard&);
    Keyboard& operator=(const Keyboard&);
    static Keyboard *instance;

    void setLineEdit(QLineEdit * );
    void setTextEdit(QTextEdit * );
    ~Keyboard();
    QPushButton *enterButton;

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    
private slots:
    void keyboardHandler();
    void slotSelectWord();
    void slotSelectDigital();
    void on_shift_clicked();
    void on_char_2_toggled(bool checked);
    void on_clear_clicked();
    void on_enterButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_returnPressed();

    void on_backButton_clicked();
    void run_keyboard_lineEdit();

    void run_keyboard_textEdit();

    void on_btnPrev_clicked();

    void on_btnNext_clicked();

    void on_btnEnCh_clicked();

private:
    void handlePY(QString key);
    void clearText();
    void setUpperLetter(bool upper);
    QStringList getCandidate(const QString &keys);
private:
    Ui::Keyboard *ui;
    QString outputText;
    QLineEdit *outputLineEdit;
    QTextEdit *outputTextEdit;
    QPushButton *btnText[N_CANDIDATE];
    bool shift;
    bool bCh;
    QStringList strResults;
    int total_pages, cur_page;
    QSqlDatabase db;
    EditType et;
};

#endif // KEYBOARD_H
