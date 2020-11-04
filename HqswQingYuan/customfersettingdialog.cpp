#include "customfersettingdialog.h"
#include "ui_customfersettingdialog.h"
#include "dataformat.h"
#include "global.h"
#include "identity.h"
#include "keyboard.h"

CustomFerSettingDialog::CustomFerSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomFerSettingDialog)
{
    ui->setupUi(this);

    setWindowTitle(QStringLiteral("发酵控制"));

    msgBox.setStyleSheet(
        "QPushButton {"
        " background-color: #89AFDE;"
        " border-style: outset;"
        " border-width: 2px;"
        " border-radius: 10px;"
        " border-color: beige;"
        " font: bold 24px;"
        " min-width: 5em;"
        " min-height:5em;"
        " padding: 20px;"
        "}"
        "QLabel {"
        " min-height:5em;"
        " font:24px;"
        " background-color: #89AFDE;"
        " border-style: outset;"
        " border-width: 2px;"
        " border-radius: 10px;"
        " border-color: beige;"
        " padding: 20px;"
        "}"
    );

    Keyboard *keyboard = Keyboard::getInstance();
    ui->aerationTimeLineEdit->installEventFilter(this);
    ui->aerationSpaceLineEdit->installEventFilter(this);
    connect(ui->aerationTimeLineEdit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_lineEdit()));
    connect(ui->aerationSpaceLineEdit, SIGNAL(selectionChanged()), keyboard, SLOT(run_keyboard_lineEdit()));

    tcpClient = new TcpClientSocket(this);
    tcpClient1 = new TcpClientSocket(this);
    aerationTimeTcpClient = new TcpClientSocket(this);
    aerationSpaceTcpClient = new TcpClientSocket(this);
}

CustomFerSettingDialog::~CustomFerSettingDialog()
{
    delete ui;
}

void CustomFerSettingDialog::showSetFerAuto(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFermenationNodeInfoByName("FER_Startctr_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FER_Startctr_BOOL").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankLocation);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankLocation - info.startIndex) * runctrlByteSize + offset;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient1->abort();
        disconnect(tcpClient1,0,0,0);
        connect(tcpClient1, SIGNAL(updateClients(QByteArray)),this,SLOT(showFerStart(QByteArray)));
        tcpClient1->sendRequestWithResult(bpack,var_data,1);
    }
}

void CustomFerSettingDialog::showFerStart(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {

    }
}

int CustomFerSettingDialog::getTankLocation() const
{
    return tankLocation;
}

void CustomFerSettingDialog::setTankLocation(int value)
{
    tankLocation = value;
}

void CustomFerSettingDialog::on_customFerButton_pressed()
{
    bool ok;
    ushort aerationTimeValue = ui->aerationTimeLineEdit->text().toUShort(&ok);
    if(!ok)
    {
        msgBox.setText(QStringLiteral("发酵时长格式不正确"));
        msgBox.show();
        return;
    }
    else
    {
        if(aerationTimeValue >999 || aerationTimeValue < 1)
        {
            msgBox.setText(QStringLiteral("发酵时长超出允许范围"));
            msgBox.show();
            return;
        }
    }

    ushort aerationSpaceValue = ui->aerationSpaceLineEdit->text().toUShort(&ok);
    if(!ok)
    {
        msgBox.setText(QStringLiteral("发酵间隔时长格式不正确"));
        msgBox.show();
        return;
    }
    else
    {
        if(aerationSpaceValue >999 || aerationSpaceValue < 1)
        {
            msgBox.setText(QStringLiteral("发酵间隔时长超出允许范围"));
            msgBox.show();
            return;
        }
    }

    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankLocation);

        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,UShort,0,0,1,0,0,0};
        //Length of ushort address and value, plus length of scrc.
        bpack.bDataLength = 2;
        bpack.bStreamLength += (2+2)*2 + 4;

        QList<ushort> addrs;
        QList<ushort> values;
        DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_Hand_RunTime");
        ushort addr = deviceNode.Offset + (info.offset + tankLocation - info.startIndex)
                * Global::getLengthByDataType(deviceNode.DataType);
        addrs.append(addr);
        ushort ushortData = aerationTimeValue * 60;
        values.append(ushortData);

        deviceNode = Global::getFermenationNodeInfoByName("FER_Hand_SpaceTime");
        addr = deviceNode.Offset + (info.offset + tankLocation - info.startIndex)
                * Global::getLengthByDataType(deviceNode.DataType);
        addrs.append(addr);
        ushortData = aerationSpaceValue * 60;
        values.append(ushortData);

        QByteArray allPackData, SData, crcData;
        QDataStream out(&SData,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out.setByteOrder(QDataStream::LittleEndian);

        allPackData.append((char*)&bpack, sizeof(bpack));

        foreach(ushort item, addrs)
        {
            out << item;
        }

        foreach(ushort item, values)
        {
            out << item;
        }

        SData.insert(0, allPackData);

        uint scrc = aerationTimeTcpClient->StreamLen_CRC32(SData);

        QDataStream out1(&crcData,QIODevice::WriteOnly);
        out1.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out1.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out1.setByteOrder(QDataStream::LittleEndian);
        out1 << scrc;

        SData.append(crcData);

        aerationTimeTcpClient->sendRequestWithResults(SData);

        ushort offset = Global::getFermenationNodeInfoByName("Aeration_Start").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("Aeration_Start").Offset % 8;

        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        addr = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankLocation - info.startIndex) * runctrlByteSize + offset;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Bool,addr,index,1,0,0,0};
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient1->abort();
        disconnect(tcpClient1,0,0,0);
        connect(tcpClient1, SIGNAL(updateClients(QByteArray)),this,SLOT(showFerStart(QByteArray)));
        tcpClient1->sendRequestWithResult(bpack,var_data,1);

        /*ushort offset = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset % 8;

        info = Global::getFerDeviceGroupInfo(tankLocation);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        address = Global::ferDeviceInfo.Runctr_Address +
                (info.offset + tankLocation - info.startIndex) * runctrlByteSize + offset;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_RealData,Bool,address,index,1,0,0,0};
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        connect(tcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showSetFerAuto(QByteArray)));
        tcpClient->sendRequestWithResult(bpack,var_data,1);*/
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void CustomFerSettingDialog::on_customFerButton_released()
{
    /*User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        StreamPack bpack;
        ushort offset = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("FER_Auto_BOOL").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankLocation);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankLocation - info.startIndex) * runctrlByteSize + offset;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_RealData,Bool,address,index,1,0,0,0};
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        disconnect(tcpClient,0,0,0);
        connect(tcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showSetFerAuto(QByteArray)));
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }*/
}

void CustomFerSettingDialog::setSpaceTime(int value)
{
    spaceTime = value;
    ui->aerationSpaceLineEdit->setText(QString::number(value));
}

bool CustomFerSettingDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->aerationSpaceLineEdit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->aerationSpaceLineEdit->selectionChanged();
        }
    }
    else if(watched == ui->aerationTimeLineEdit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            emit ui->aerationTimeLineEdit->selectionChanged();
        }
    }

    return QWidget::eventFilter(watched, event);
}

void CustomFerSettingDialog::setRunTime(int value)
{
    runTime = value;
    ui->aerationTimeLineEdit->setText(QString::number(value));
}

void CustomFerSettingDialog::on_customFerButton_2_clicked()
{
    close();
}

void CustomFerSettingDialog::updateFermentationData(QSet<int>, QMap<float, QString> dataMap)
{
    bool isAerationStart = Global::getFerRunctrValueByName(tankLocation,"Aeration_Start",dataMap);
    if(isAerationStart)
    {
        ui->customFerButton->setEnabled(false);
        ui->endCustomFerButton->setEnabled(true);
    }
    else
    {
        ui->customFerButton->setEnabled(true);
        ui->endCustomFerButton->setEnabled(false);
    }
}

void CustomFerSettingDialog::on_endCustomFerButton_pressed()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankLocation);
        ushort offset = Global::getFermenationNodeInfoByName("Aeration_Start").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("Aeration_Start").Offset % 8;

        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort addr = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankLocation - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Bool,addr,index,1,0,0,0};
        bool data = false;
        QVariant var_data = QVariant(data);

        tcpClient1->abort();
        disconnect(tcpClient1,0,0,0);
        connect(tcpClient1, SIGNAL(updateClients(QByteArray)),this,SLOT(showFerStart(QByteArray)));
        tcpClient1->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}
