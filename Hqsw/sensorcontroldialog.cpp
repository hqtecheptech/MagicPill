#include "sensorcontroldialog.h"
#include "ui_sensorcontroldialog.h"
#include "global.h"
#include "identity.h"

SensorControlDialog::SensorControlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SensorControlDialog)
{
    ui->setupUi(this);

    setWindowTitle(QStringLiteral("传感器控制"));

    tcpClient = new TcpClientSocket(this);

    for(int i=0;i<Global::ferDeviceInfo.Device_Number;i++)
    {
        ui->sensorIndexComboBox->addItem(QStringLiteral("%1#传感器").arg(QString::number(i+1)));
    }
}

SensorControlDialog::~SensorControlDialog()
{
    delete ui;
}

int SensorControlDialog::getTankIndex() const
{
    return tankIndex;
}

void SensorControlDialog::setTankIndex(int value)
{
    tankIndex = value;
}

void SensorControlDialog::updateFermentationData(QSet<int> changedDeviceSet, QMap<float,QString> dataMap)
{
    if(changedDeviceSet.contains(tankIndex))
    {
        parseFerRunCtrData(dataMap);
    }
}

void SensorControlDialog::parseFerRunCtrData(QMap<float,QString> dataMap)
{
    isSensorRaised = Global::getFerRunctrValueByName(tankIndex,"sensor_rise_inplace", dataMap);
    isSensorFall = Global::getFerRunctrValueByName(tankIndex,"sensor_fall_inplace", dataMap);

    if(isSensorRaised)
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::green)));
    }
    else
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::red)));
    }
    ui->raiseStateLabel->setPalette(p);

    if(isSensorFall)
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::green)));
    }
    else
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::red)));
    }
    ui->fallStateLabel->setPalette(p);

    if(!isSensorFall && !isSensorRaised)
    {
        p.setBrush(QPalette::Background,QBrush(QColor(Qt::white)));
        ui->fallStateLabel->setPalette(p);
        ui->raiseStateLabel->setPalette(p);
    }
}

void SensorControlDialog::on_sensorFallPushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getFermenationNodeInfoByName("sensor_fall").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("sensor_fall").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (tankIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SensorControlDialog::on_sensorRaisePushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        ushort offset = Global::getFermenationNodeInfoByName("sensor_rise").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("sensor_rise").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (tankIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = true;
        QVariant var_data = QVariant(data);

        tcpClient->abort();
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SensorControlDialog::on_sensorIndexComboBox_currentIndexChanged(int index)
{
    tankIndex = index;
    ui->sensorIndexLabel->setText(QString::number(tankIndex+1));
    parseFerRunCtrData(Global::currentFermenationDataMap);
}

void SensorControlDialog::on_sensorStopPushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort offset = Global::getFermenationNodeInfoByName("sensor_fall").Offset / 8;
        ushort index = Global::getFermenationNodeInfoByName("sensor_fall").Offset % 8;

        DeviceGroupInfo info = Global::getFerDeviceGroupInfo(tankIndex);
        ushort runctrlByteSize = Global::ferDeviceInfo.RunCtr_Block_Size / 8;
        ushort address = Global::ferDeviceInfo.Runctr_Address + (tankIndex - info.startIndex) * runctrlByteSize + offset;

        StreamPack bpack;
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        bool data = false;
        QVariant var_data = QVariant(data);
        tcpClient->abort();
        tcpClient->sendRequestWithResult(bpack,var_data,1);

        offset = Global::getFermenationNodeInfoByName("sensor_rise").Offset / 8;
        index = Global::getFermenationNodeInfoByName("sensor_rise").Offset % 8;        
        address = Global::ferDeviceInfo.Runctr_Address + (tankIndex - info.startIndex) * runctrlByteSize + offset;

        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_RealData,Bool,address,index,1,0,stime,etime};
        bpack.bStartTime =stime;
        bpack.bEndTime =etime;
        data = false;
        var_data = QVariant(data);
        tcpClient->abort();
        tcpClient->sendRequestWithResult(bpack,var_data,1);
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}
