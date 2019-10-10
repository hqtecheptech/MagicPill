#include "adduserdialog.h"
#include "ui_adduserdialog.h"
#include "dataformat.h"

AddUserDialog::AddUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUserDialog)
{
    ui->setupUi(this);
    work = new RequestWorkThread(this);
    ui->testButton->setVisible(false);
}

AddUserDialog::~AddUserDialog()
{
    delete ui;
    work->terminate();
}

void AddUserDialog::on_addPushButton_clicked()
{
    if(ui->usernameLineEdit->text().trimmed() != ""
        && ui->pwdLineEdit->text().trimmed() != ""
        && ui->confirmLineEdit->text().trimmed() != "")
    {
        if(ui->pwdLineEdit->text().trimmed() == ui->confirmLineEdit->text().trimmed())
        {
            StreamPack bpack;
            QDateTime currentdt = QDateTime::currentDateTime();
            uint stime =currentdt.toTime_t();
            uint etime =currentdt.toTime_t();
            bpack = {sizeof(StreamPack),1,0,w_AddUser,String,0,0,1,0,stime,etime};
            bpack.bStartTime =stime;
            bpack.bEndTime =etime;
            QString loginInfo = ui->usernameLineEdit->text().trimmed() + "," + ui->pwdLineEdit->text().trimmed();
            QTextCodec *codec = QTextCodec::codecForLocale();
            QByteArray data = codec->fromUnicode(loginInfo);
            QVariant var_data = QVariant(data);
            work->setPack(bpack);
            work->setDataToSend(var_data);
            work->setLen(data.length()+4);
            qDebug() << data;
            qDebug() << "login info sizeof = "<<data.length()+4;
            disconnect(work,0,0,0);
            connect(work,SIGNAL(updateParentThread(QByteArray)),this,SLOT(showAddUser(QByteArray)));
            work->start();
        }
        ui->promptLabel->setText(QStringLiteral("密码不匹配！"));
    }
    else
    {
        ui->promptLabel->setText(QStringLiteral("用户名和密码不能为空！"));
    }
}

void AddUserDialog::showAddUser(QByteArray result)
{
    StreamPack bDevice;
    memcpy(&bDevice,result,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {
        ui->promptLabel->setText(QStringLiteral("新用户已添加！"));
    }
    else
    {
        ui->promptLabel->setText(QStringLiteral("添加新用户失败！"));
    }
}

void AddUserDialog::showEvent(QShowEvent *)
{

}

void AddUserDialog::closeEvent(QCloseEvent *)
{
    ui->promptLabel->setText(QStringLiteral(""));
}


void AddUserDialog::on_testButton_clicked()
{
    m_NetManger = new QNetworkAccessManager;
    QObject::connect(m_NetManger, SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(finishedSlot(QNetworkReply*)));
    QUrl url("http://localhost:30691/PullAllPlcData");
    QByteArray array("{\"ProjectId\":\"xy\", \"DeviceId\":1,\"DeviceIndex\":2}");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

    m_NetManger->post(request, array);
}

void AddUserDialog::finishedSlot(QNetworkReply * reply)
{
    reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        QString string = QString::fromUtf8(bytes);

        QJsonParseError err;
        QJsonDocument json = QJsonDocument::fromJson(bytes, &err);
        if(err.error != QJsonParseError::NoError)
        {
            qDebug() << "IpQuery, json error - " << err.errorString();
            return;
        }
        if(json.isArray())
        {
            QJsonArray dataArray = json.array();
            int size = dataArray.size();
            QJsonObject info = dataArray.first().toObject();
            string = QString::number(info.find("DeviceIndex").value().toInt());
        }

        /*QJsonParseError err;
        QJsonDocument json = QJsonDocument::fromJson(bytes, &err);
        if(err.error != QJsonParseError::NoError)
        {
            qDebug() << "IpQuery, json error - " << err.errorString();
            return;
        }
        QJsonObject obj = json.object();
        QString string = obj.value("BB").toString();
        QJsonObject::const_iterator it = obj.find("data");
        if(it != obj.constEnd())
        {
            QJsonArray dataArray = it.value().toArray();
            QJsonObject info = dataArray.first().toObject();
            QString area = info.find("location").value().toString();
        }*/

        qDebug() << string;

        ui->promptLabel->setText(string);
    }
    else
    {
        qDebug()<< reply->errorString();
    }

    reply->deleteLater();
}
