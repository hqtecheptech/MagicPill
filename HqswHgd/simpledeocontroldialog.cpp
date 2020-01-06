#include "simpledeocontroldialog.h"
#include "ui_simpledeocontroldialog.h"

#include "global.h"
#include "identity.h"

SimpleDeoControlDialog::SimpleDeoControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimpleDeoControlDialog)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("设备管理"));

    tcpClient = new TcpClientSocket(this);
}

SimpleDeoControlDialog::~SimpleDeoControlDialog()
{
    delete ui;
}

void SimpleDeoControlDialog::updateFermentationData(QSet<int> changedDataSet, QMap<float, QString> dataMap)
{
    if(changedDataSet.contains(0))
    {
        parseFermentationData(dataMap);
        parseFerRunCtrData(dataMap);
    }
}

void SimpleDeoControlDialog::parseFermentationData(QMap<float, QString> dataMap)
{
    int deviceIndex = 0;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("WT_PH");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    ui->waterPhValueLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("Level_Switch_Read");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    ui->waterLeverValueLabel->setText(dataMap[address]);

    deviceNode = Global::getFermenationNodeInfoByName("Temp_Read");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    ui->waterTemptureLabel->setText(dataMap[address]);
}

void SimpleDeoControlDialog::parseFerRunCtrData(QMap<float, QString> dataMap)
{
    int deviceIndex = 0;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("Pump_PL_Run_Signal");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    uint temp = dataMap[address].toUInt();
    if(temp == 1)
    {
       spRunState = true;
    }
    else
    {
       spRunState = false;
    }

    deviceNode = Global::getFermenationNodeInfoByName("Pump_PL_False_Signal");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    temp = dataMap[address].toUInt();
    if(temp == 1)
    {
       spFaultState = true;
    }
    else
    {
       spFaultState = false;
    }

    if(spFaultState)
    {
        ui->spary_pump_state_label->setText(QStringLiteral("故障"));
        ui->spary_pump_state_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 255, 0)");
    }
    else
    {
        if(spRunState)
        {
            ui->spary_pump_state_label->setText(QStringLiteral("运行中"));
            ui->spary_pump_state_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 170, 0)");
        }
        else
        {
            ui->spary_pump_state_label->setText(QStringLiteral("已停止"));
            ui->spary_pump_state_label->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
        }
    }

    deviceNode = Global::getFermenationNodeInfoByName("Start_Water_Valve");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    temp = dataMap[address].toUInt();
    if(temp == 1)
    {
        ui->startWaterValveButton->setEnabled(false);
        ui->stopWaterValveButton->setEnabled(true);
    }
    else
    {
        ui->startWaterValveButton->setEnabled(true);
        ui->stopWaterValveButton->setEnabled(false);
    }

    deviceNode = Global::getFermenationNodeInfoByName("Start_Pump_PL");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    temp = dataMap[address].toUInt();
    if(temp == 1)
    {
        ui->startSparyValveButton->setEnabled(false);
        ui->stopSparyValveButton->setEnabled(true);
    }
    else
    {
        ui->startSparyValveButton->setEnabled(true);
        ui->stopSparyValveButton->setEnabled(false);
    }

    deviceNode = Global::getFermenationNodeInfoByName("Set_Other_Devices_Auto");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
            * Global::getLengthByDataType(deviceNode.DataType);
    temp = dataMap[address].toUInt();
    if(temp == 1)
    {
        ui->otherDeviceAutoButton->setEnabled(false);
        ui->otherDeviceManualButton->setEnabled(true);
    }
    else
    {
        ui->otherDeviceAutoButton->setEnabled(true);
        ui->otherDeviceManualButton->setEnabled(false);
    }
}

void SimpleDeoControlDialog::on_startWaterValveButton_clicked()
{
    int deviceIndex = 0;

    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
        StreamPack bpack;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,UInt,0,0,1,0,0,0};
        //Length of ushort address and value, plus length of scrc.
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bpack.bDataLength = 1;
        bpack.bStreamLength += (4+2)*bpack.bDataLength + 4;

        QList<ushort> addrs;
        QList<uint> values;
        DeviceNode deviceNode = Global::getFermenationNodeInfoByName("Start_Water_Valve");
        ushort addr = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
                * Global::getLengthByDataType(deviceNode.DataType);
        addrs.append(addr);
        values.append(1);

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

        foreach(uint item, values)
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

        tcpClient->sendRequestWithResults(SData);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SimpleDeoControlDialog::on_stopWaterValveButton_clicked()
{
    int deviceIndex = 0;
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
        StreamPack bpack;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,UInt,0,0,1,0,0,0};
        //Length of ushort address and value, plus length of scrc.
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bpack.bDataLength = 1;
        bpack.bStreamLength += (4+2)*bpack.bDataLength + 4;

        QList<ushort> addrs;
        QList<uint> values;
        DeviceNode deviceNode = Global::getFermenationNodeInfoByName("Start_Water_Valve");
        ushort addr = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
                * Global::getLengthByDataType(deviceNode.DataType);
        addrs.append(addr);
        values.append(0);

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

        foreach(uint item, values)
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

        tcpClient->sendRequestWithResults(SData);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SimpleDeoControlDialog::on_startSparyValveButton_clicked()
{
    int deviceIndex = 0;
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
        StreamPack bpack;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,UInt,0,0,1,0,0,0};
        //Length of ushort address and value, plus length of scrc.
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bpack.bDataLength = 1;
        bpack.bStreamLength += (4+2)*bpack.bDataLength + 4;

        QList<ushort> addrs;
        QList<uint> values;
        DeviceNode deviceNode = Global::getFermenationNodeInfoByName("Start_Pump_PL");
        ushort addr = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
                * Global::getLengthByDataType(deviceNode.DataType);
        addrs.append(addr);
        values.append(1);

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

        foreach(uint item, values)
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

        tcpClient->sendRequestWithResults(SData);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SimpleDeoControlDialog::on_stopSparyValveButton_clicked()
{
    int deviceIndex = 0;
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
        StreamPack bpack;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,UInt,0,0,1,0,0,0};
        //Length of ushort address and value, plus length of scrc.
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bpack.bDataLength = 1;
        bpack.bStreamLength += (4+2)*bpack.bDataLength + 4;

        QList<ushort> addrs;
        QList<uint> values;
        DeviceNode deviceNode = Global::getFermenationNodeInfoByName("Start_Pump_PL");
        ushort addr = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
                * Global::getLengthByDataType(deviceNode.DataType);
        addrs.append(addr);
        values.append(0);

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

        foreach(uint item, values)
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

        tcpClient->sendRequestWithResults(SData);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SimpleDeoControlDialog::on_otherDeviceAutoButton_clicked()
{
    int deviceIndex = 0;
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
        StreamPack bpack;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,UInt,0,0,1,0,0,0};
        //Length of ushort address and value, plus length of scrc.
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bpack.bDataLength = 1;
        bpack.bStreamLength += (4+2)*bpack.bDataLength + 4;

        QList<ushort> addrs;
        QList<uint> values;
        DeviceNode deviceNode = Global::getFermenationNodeInfoByName("Set_Other_Devices_Auto");
        ushort addr = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
                * Global::getLengthByDataType(deviceNode.DataType);
        addrs.append(addr);
        values.append(1);

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

        foreach(uint item, values)
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

        tcpClient->sendRequestWithResults(SData);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SimpleDeoControlDialog::on_otherDeviceManualButton_clicked()
{
    int deviceIndex = 0;
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
        StreamPack bpack;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,W_Send_Control,UInt,0,0,1,0,0,0};
        //Length of ushort address and value, plus length of scrc.
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bpack.bDataLength = 1;
        bpack.bStreamLength += (4+2)*bpack.bDataLength + 4;

        QList<ushort> addrs;
        QList<uint> values;
        DeviceNode deviceNode = Global::getFermenationNodeInfoByName("Set_Other_Devices_Auto");
        ushort addr = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex)
                * Global::getLengthByDataType(deviceNode.DataType);
        addrs.append(addr);
        values.append(0);

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

        foreach(uint item, values)
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

        tcpClient->sendRequestWithResults(SData);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SimpleDeoControlDialog::on_pushButton_clicked()
{
    close();
}
