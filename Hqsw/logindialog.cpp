#include "logindialog.h"
#include "ui_logindialog.h"
#include "identity.h"
#include "adduserdialog.h"
#include "global.h"

//#include <winsock.h>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    //setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowTitle(QStringLiteral("用户登录"));

    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    tcpClient = new TcpClientSocket(this);
    connect(tcpClient, SIGNAL(updateClients(QByteArray)), this, SLOT(showLogin(QByteArray)));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginButton_clicked()
{
    if(ui->userNameLineEdit->text().trimmed() != ""
        && ui->pwdLineEdit->text().trimmed() != "")
    {
        StreamPack bpack;
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        bpack = {sizeof(StreamPack),1,0,r_SignIn,String,0,0,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        QString loginInfo = ui->userNameLineEdit->text().trimmed() + "," + ui->pwdLineEdit->text().trimmed();
        QTextCodec *codec = QTextCodec::codecForLocale();
        QByteArray data = codec->fromUnicode(loginInfo);
        bpack.bStreamLength += data.length() + 4;

        QByteArray allPackData, SData, crcData;
        QDataStream out(&SData,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out.setByteOrder(QDataStream::LittleEndian);

        allPackData.append((char*)&bpack, sizeof(bpack));

        int len = allPackData.length();

        SData.insert(0, allPackData);
        len = SData.length();

        SData.append(loginInfo);
        len = SData.length();

        uint scrc = tcpClient->StreamLen_CRC32(SData);

        QDataStream out1(&crcData,QIODevice::WriteOnly);
        out1.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out1.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out1.setByteOrder(QDataStream::LittleEndian);
        out1 << scrc;

        SData.append(crcData);

        len = SData.length();

        tcpClient->sendRequestWithResults(SData);

        //ds = new DataSender();
        //connect(ds, SIGNAL(updateClients(QByteArray)), this, SLOT(showLogin(QByteArray)));
        //ds->sendRequestWithResults(SData);
    }
    else
    {
        ui->promptLabel->setText(QStringLiteral("用户名和密码不能为空！"));
    }
}

void LoginDialog::showLogin(QByteArray result)
{
    StreamPack bDevice;
    memcpy(&bDevice,result,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {
        Identity::getInstance()->setUser(new User(ui->userNameLineEdit->text().trimmed(), 0 ,1));
        ui->promptLabel->setText(QStringLiteral("登录成功！"));
        index += 1;
        QString temp = QString::number(index) + QStringLiteral(": 管理员") + Identity::getInstance()->getUser()->getUsername() + QStringLiteral("登入 ") + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

        QStandardItem *item = new QStandardItem(temp);
        QList<QStandardItem *> newItemList;
        newItemList.append(item);
        Global::loginHistoryModel->insertRow(0, newItemList);

        emit LogingStateChanged();

        close();
    }
    else
    {
        ui->promptLabel->setText(QStringLiteral("用户名或密码不正确！"));
    }
}

void LoginDialog::showEvent(QShowEvent *)
{

}

void LoginDialog::closeEvent(QCloseEvent *)
{

}

void LoginDialog::on_addUserButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        ui->promptLabel->setText("");
        AddUserDialog *dialog = new AddUserDialog(this);
        dialog->show();
    }
    else
    {
        ui->promptLabel->setText(QStringLiteral("您无权创建用户，请先登录！"));
    }
}

void LoginDialog::on_logoutButton_clicked()
{
    close();
}

void LoginDialog::on_testSendControlButton_clicked()
{
    /*QDateTime currentdt = QDateTime::currentDateTime();
    StreamPack bpack;
    uint stime =currentdt.toTime_t();
    uint etime =currentdt.toTime_t();
    //ushort address = Global::ferDeviceInfo.Aeration_Runtime_Setting_Address + 4 * ui->tankComboBox->currentIndex();
    //bpack = {sizeof(StreamPack),1,0,w_SetData,UInt,address,(quint16)ui->tankComboBox->currentIndex(),Global::ferDeviceInfo.Aeration_Runtime_Setting_Category,0,stime,etime};
    ushort address = 0;
    bpack = {sizeof(StreamPack),6,0,W_Send_Control,Float,address,0,3,0,stime,etime};

    bpack.bStreamLength += 3 * (2 + 4) + 4;

    quint16 strlen= sizeof(bpack);
    BYTE *p = new BYTE[strlen];
    QByteArray SData;
    memcpy(p,reinterpret_cast<char*>(&bpack),sizeof(bpack));
    QDataStream out(&SData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_8); //设计数据流版本
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out.setByteOrder(QDataStream::LittleEndian);
    for(int i=0;i<strlen;i++) {out << *(p+i);}
    delete p;

    QVector<ushort> settingAddress;
    QVector<float> settingValues;

    settingAddress.append(0);
    settingAddress.append(4);
    settingAddress.append(8);

    settingValues.append(101.3);
    settingValues.append(201.0);
    settingValues.append(512.3);

    int len = SData.length();
    foreach(ushort value, settingAddress)
    {
        out << value;
    }

    len = SData.length();
    foreach(float value, settingValues)
    {
        out << value;
    }

    len = SData.length();
    uint scrc = tcpClient->StreamLen_CRC32(SData);
    out << scrc;

    tcpClient->sendRequestWithResults(SData);*/

    QDateTime currentdt = QDateTime::currentDateTime();
    uint stime =currentdt.toTime_t();
    uint etime =currentdt.toTime_t();

    //ushort offset = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset / 8;
    //ushort index = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset % 8;
    ushort offset = 1;
    ushort index = 1;

    //DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
    //ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
    //ushort address = Global::ferDeviceInfo.Runctr_Address + (tankIndex - info.startIndex) * runctrlByteSize + offset;
    ushort address = 2800 + offset;

    StreamPack bpack;
    bpack = {sizeof(StreamPack),6,0,W_Send_Control,Bool,address,index,1,0,stime,etime};
    bpack.bStartTime = stime;
    bpack.bEndTime = etime;
    bool data = true;
    QVariant var_data = QVariant(data);

    tcpClient->sendRequestWithResult(bpack,var_data,1);

    /*QDateTime currentdt = QDateTime::currentDateTime();
    StreamPack bpack;
    uint stime =currentdt.toTime_t();
    uint etime =currentdt.toTime_t();
    bpack = {sizeof(StreamPack),6,0,W_Send_Control,Float,0,0,3,0,stime,etime};
    //bpack = {sizeof(StreamPack),1,0,w_SetData,UInt,address,0,Global::ferDeviceInfo.Aeration_Runtime_Setting_Category,0,stime,etime};

    QString values = "101";
    values += ",201";
    values += ",301";
    QByteArray ba = values.toLatin1();
    int baLen = ba.size();
    char* chValues = ba.data();
    bpack.bStreamLength += 3 * sizeof(float) + baLen + 4;

    QVector<float> address;
    address.append(4.0);
    address.append(8.0);
    address.append(12.0);

    quint16 strlen= sizeof(bpack);
    BYTE *p = new BYTE[strlen];
    QByteArray SData;
    memcpy(p,reinterpret_cast<char*>(&bpack),sizeof(bpack));
    QDataStream out(&SData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_8); //设计数据流版本
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    //QDataStream::BigEndian或QDataStream::LittleEndian
    out.setByteOrder(QDataStream::LittleEndian);
    for(int i=0;i<strlen;i++) {out << *(p+i);}
    delete p;

    int len = SData.length();

    BYTE *pStr = new BYTE[baLen];
    memcpy(pStr,chValues,baLen);
    for(int i=0;i<baLen;i++) {out << *(pStr+i);}
    delete pStr;

    len = SData.length();
    foreach(float item, address)
    {
        out << item;
    }

    len = SData.length();
    uint scrc = tcpClient->StreamLen_CRC32(SData);
    out << scrc;

    tcpClient->sendRequestWithResults(SData);*/
}

void LoginDialog::on_testSendControlButton_2_clicked()
{
    QDateTime currentdt = QDateTime::currentDateTime();
    uint stime =currentdt.toTime_t();
    uint etime =currentdt.toTime_t();

    //ushort offset = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset / 8;
    //ushort index = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset % 8;
    ushort offset = 1;
    ushort index = 1;

    //DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
    //ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
    //ushort address = Global::ferDeviceInfo.Runctr_Address + (tankIndex - info.startIndex) * runctrlByteSize + offset;
    ushort address = 2800 + offset;

    StreamPack bpack;
    bpack = {sizeof(StreamPack),6,0,W_Send_Control,Bool,address,index,1,0,stime,etime};
    bpack.bStartTime = stime;
    bpack.bEndTime = etime;
    bool data = false;
    QVariant var_data = QVariant(data);

    tcpClient->sendRequestWithResult(bpack,var_data,1);
}
