#include "ferconfigdialog.h"
#include "ui_ferconfigdialog.h"
#include "global.h"

FerConfigDialog::FerConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FerConfigDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("发酵设置"));

    ui->step_seq_comboBox->setStyleSheet("QComboBox {font: bold 30px;}");
    ui->reload_push_button->setStyleSheet("QPushButton {font: bold 20px;}");
    ui->save_push_button->setStyleSheet("QPushButton {font: bold 20px;}");
    ui->exitButton->setStyleSheet("QPushButton {font: bold 20px;}");

    for(int i=0; i < Global::ferDeviceInfo.Device_Number;i++)
    {
        ui->step_seq_comboBox->addItem(QString::number(i + 1) + QStringLiteral("号发酵池"));
    }
    ui->step_seq_comboBox->setCurrentIndex(0);
    ui->step_seq_comboBox->setVisible(false);

    ui->append_step_push_button->setVisible(false);
    ui->label->setVisible(false);
    ui->remove_step_push_button->setVisible(false);

    FerConfig::loadConfig(&_ferSteps, 0);

    centerLayout = new QVBoxLayout;
    centerLayout->setSpacing(5);

    ui->step_config_frame->setLayout(centerLayout);
    ui->scrollArea->setWidget(ui->step_config_frame);

    ui->scrollArea->verticalScrollBar()
            ->setStyleSheet("QScrollBar:vertical"
                                "{"
                                "width:50px;"
                                "background:rgba(0,0,0,0%);"
                                "margin:0px,0px,0px,0px;"
                                "padding-top:9px;"
                                "padding-bottom:9px;"
                                "}"
                                "QScrollBar::handle:vertical"
                                "{"
                                "width:50px;"
                                "background:rgba(0,0,0,25%);"
                                " border-radius:4px;"
                                "min-height:20;"
                                "}"
                                "QScrollBar::handle:vertical:hover"
                                "{"
                                "width:50px;"
                                "background:rgba(0,0,0,50%);"
                                " border-radius:4px;"
                                "min-height:20;"
                                "}"
                                "QScrollBar::add-line:vertical"
                                "{"
                                "height:9px;width:8px;"
                                "border-image:url(:/images/a/3.png);"
                                "subcontrol-position:bottom;"
                                "}"
                                "QScrollBar::sub-line:vertical"
                                "{"
                                "height:9px;width:8px;"
                                "border-image:url(:/images/a/1.png);"
                                "subcontrol-position:top;"
                                "}"
                                "QScrollBar::add-line:vertical:hover"
                                "{"
                                "height:9px;width:8px;"
                                "border-image:url(:/images/a/4.png);"
                                "subcontrol-position:bottom;"
                                "}"
                                "QScrollBar::sub-line:vertical:hover"
                                "{"
                                "height:9px;width:8px;"
                                "border-image:url(:/images/a/2.png);"
                                "subcontrol-position:top;"
                                "}"
                                "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                                "{"
                                "background:rgba(0,0,0,10%);"
                                "border-radius:4px;"
                                "}");

    msgBox.setStyleSheet(
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

    initUI();
}

FerConfigDialog::~FerConfigDialog()
{
    delete ui;
}

void FerConfigDialog::on_remove_step_push_button_clicked()
{
    if(!centerLayoutWidgets.isEmpty())
    {
        QWidget *rw = centerLayoutWidgets.last();
        centerLayout->removeWidget(rw);
        centerLayoutWidgets.removeLast();
        int rh = rw->height() + 5;
        rw->deleteLater();
        _ferSteps.removeLast();

        h -= rh;
        ui->step_config_frame->resize(w, h);
        ui->step_config_frame->setMinimumSize(w, h);
        ui->step_config_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        qDebug() << h;
    }
}

void FerConfigDialog::on_append_step_push_button_clicked()
{
    FerStepForm *fsf = new FerStepForm(this);
    fsf->setMinimumSize(1300, 300);
    fsf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    centerLayout->addWidget(fsf);
    centerLayoutWidgets.append(fsf);
    connect(fsf, SIGNAL(sizeChanged(int,int)),this,SLOT(updateSize(int,int)));
    FerStep *newStep = new FerStep;
    _ferSteps.append(newStep);

    h += 305;

    ui->step_config_frame->resize(w, h);
    ui->step_config_frame->setMinimumSize(w, h);
    ui->step_config_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QScrollBar *pScrollBar = ui->scrollArea->verticalScrollBar();
    if (pScrollBar != NULL)
    {
        pScrollBar->setValue(h);
    }

    qDebug() << h;
}

void FerConfigDialog::updateSize(int aw, int ah)
{
    h += ah;

    ui->step_config_frame->resize(w, h);
    ui->step_config_frame->setMinimumSize(w, h);
    ui->step_config_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QScrollBar *pScrollBar = ui->scrollArea->verticalScrollBar();
    if (pScrollBar != NULL)
    {
        pScrollBar->setValue(pScrollBar->value() + ah);
    }

    qDebug() << h;
}

void FerConfigDialog::initUI()
{
    for(int i=0; i < _ferSteps.length(); i++)
    {
        FerStepForm *fsf = new FerStepForm(this);
        int caseNum = _ferSteps.at(i)->ferCases().length();
        fsf->setMinimumSize(1300, 300 + caseNum*100);
        fsf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        fsf->setStepSequence(i);
        fsf->initStep(_ferSteps.at(i));
        centerLayout->addWidget(fsf);
        centerLayoutWidgets.append(fsf);
        connect(fsf, SIGNAL(sizeChanged(int,int)),this,SLOT(updateSize(int,int)));
        h += 300 + caseNum*100 + 5;
    }

    w = ui->step_config_frame->width();
    ui->step_config_frame->resize(w, h);
    ui->step_config_frame->setMinimumSize(w, h);
    ui->step_config_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void FerConfigDialog::clearUI()
{
    while(!centerLayoutWidgets.isEmpty())
    {
        QWidget *rw = centerLayoutWidgets.last();
        centerLayout->removeWidget(rw);
        centerLayoutWidgets.removeLast();
        int rh = rw->height() + 5;
        rw->deleteLater();

        h -= rh;
        qDebug() << h;
    }

    ui->step_config_frame->resize(w, h);
    ui->step_config_frame->setMinimumSize(w, h);
    ui->step_config_frame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _ferSteps.clear();
}

void FerConfigDialog::on_reload_push_button_clicked()
{
    clearUI();
    FerConfig::loadConfig(&_ferSteps, ui->step_seq_comboBox->currentIndex());
    initUI();
}

void FerConfigDialog::on_save_push_button_clicked()
{
    if(centerLayoutWidgets.length() == 0)
    {
        msgBox.setText("当前方案还没有添加发酵步骤");
        msgBox.show();
        return;
    }
    else
    {
        QFile file(Global::systemConfig.ferconfigPath +  Global::systemConfig.ferconfigPrefix + QString::number(ui->step_seq_comboBox->currentIndex() + 1));
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Open configuration file failed!";
            msgBox.setText("无法打开配置文件");
            msgBox.show();
            return;
        }

        Global::ferConfigStrContent.clear();
        Global::ferConfigStrContent.append("<start>");
        Global::ferConfigStrContent.append("total_step " + QString::number(centerLayoutWidgets.length()));
        Global::ferConfigStrContent.append("");

        for(int i=0; i<centerLayoutWidgets.length();i++)
        {
            Global::ferConfigStrContent.append("<step_" + QString::number(i+1) + ">");
            FerStepForm *ferStepForm = (FerStepForm *)centerLayoutWidgets.at(i);
            QString result = ferStepForm->checkStepValidation();
            if(result != "OK")
            {
                msgBox.setText("Step " + QString::number(i+1) + ": " + result);
                msgBox.show();
                return;
            }
            Global::ferConfigStrContent.append("");
        }
        Global::ferConfigStrContent.removeLast();
        Global::ferConfigStrContent.append("<end>");

        for(int i=0; i<Global::ferConfigStrContent.length(); i++)
        {
            file.write(QByteArray(Global::ferConfigStrContent.at(i).toLocal8Bit().constData()));
            file.write("\n");
        }
        file.close();
    }

    msgBox.setText("保存成功");
    msgBox.show();
}

void FerConfigDialog::on_step_seq_comboBox_currentIndexChanged(int index)
{
    if(isVisible())
    {
        clearUI();
        FerConfig::loadConfig(&_ferSteps, index);
        initUI();
    }
}

void FerConfigDialog::on_exitButton_clicked()
{
    close();
}
