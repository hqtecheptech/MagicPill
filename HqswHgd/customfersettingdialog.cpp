#include "customfersettingdialog.h"
#include "ui_customfersettingdialog.h"
#include "dataformat.h"
#include "global.h"
#include "identity.h"

CustomFerSettingDialog::CustomFerSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomFerSettingDialog)
{
    ui->setupUi(this);

    setWindowTitle(QStringLiteral("发酵控制"));

    tcpClient = new TcpClientSocket(this);
    tcpClient1 = new TcpClientSocket(this);
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
        close();
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
    int aerationTimeValue = ui->aerationTimeLineEdit->text().toInt(&ok);
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

    int aerationSpaceValue = ui->aerationSpaceLineEdit->text().toInt(&ok);
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

    int frequency = ui->fanFrequencyLineEdit->text().toInt(&ok);
    if(!ok)
    {
        msgBox.setText(QStringLiteral("风机频率格式不正确"));
        msgBox.show();
        return;
    }
    else
    {
        if(frequency >50 || frequency < 1)
        {
            msgBox.setText(QStringLiteral("风机频率超出允许范围"));
            msgBox.show();
            return;
        }
    }

    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankLocation);

        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Int,0,0,3,0,0,0};
        //Length of ushort address and value, plus length of scrc.
        bpack.bDataLength = 3;
        bpack.bStreamLength += (4+2)*bpack.bDataLength + 4;

        QList<ushort> addrs;
        QList<int> values;
        DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_Hand_RunTime");
        ushort addr = deviceNode.Offset + (info.offset + tankLocation - info.startIndex)
                * Global::getLengthByDataType(deviceNode.DataType);
        addrs.append(addr);
        int intData = aerationTimeValue * 60;
        values.append(intData);

        deviceNode = Global::getFermenationNodeInfoByName("FER_Hand_SpaceTime");
        addr = deviceNode.Offset + (info.offset + tankLocation - info.startIndex)
                * Global::getLengthByDataType(deviceNode.DataType);
        addrs.append(addr);
        intData = aerationSpaceValue * 60;
        values.append(intData);

        deviceNode = Global::getFermenationNodeInfoByName("FER_HAND_FSP");
        addr = deviceNode.Offset + (info.offset + tankLocation - info.startIndex)
                * Global::getLengthByDataType(deviceNode.DataType);
        addrs.append(addr);
        intData = frequency;
        values.append(intData);

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

        foreach(int item, values)
        {
            out << item;
        }

        SData.insert(0, allPackData);

        uint scrc = tcpClient->StreamLen_CRC32(SData);

        QDataStream out1(&crcData,QIODevice::WriteOnly);
        out1.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out1.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out1.setByteOrder(QDataStream::LittleEndian);
        out1 << scrc;

        SData.append(crcData);
        // Send manual fermentation paramters firstly.
        tcpClient->sendRequestWithResults(SData);

        ushort offset = Global::getFermenationNodeInfoByName("Aeration_Start").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("Aeration_Start").Offset % 8;

        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        addr = Global::ferDeviceInfo.Runctr_Address + (info.offset + tankLocation - info.startIndex) * runctrlByteSize + offset;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,Bool,addr,index,2,0,0,0};
        bool data = true;
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

void CustomFerSettingDialog::setFrequency(int value)
{
    frequency = value;
}

void CustomFerSettingDialog::setSpaceTime(int value)
{
    spaceTime = value;
    ui->aerationSpaceLineEdit->setText(QString::number(value));
}

void CustomFerSettingDialog::setRunTime(int value)
{
    runTime = value;
    ui->aerationTimeLineEdit->setText(QString::number(value));
}
