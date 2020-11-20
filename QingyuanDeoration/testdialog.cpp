#include "testdialog.h"
#include "ui_testdialog.h"

#include <QHBoxLayout>

TestDialog::TestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestDialog)
{
    ui->setupUi(this);

    fanForm = new DeodorationFanForm(this);

    QHBoxLayout *lt = new QHBoxLayout(this);
    lt->addWidget(fanForm);

    setLayout(lt);
}

TestDialog::~TestDialog()
{
    delete ui;
}
