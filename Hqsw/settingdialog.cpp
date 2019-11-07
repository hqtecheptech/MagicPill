#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "global.h"
#include "dataformat.h"
#include "identity.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QVector>

//#include <winsock.h>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    //setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowTitle(QStringLiteral("发酵参数设置"));

    aerationRtSettingModel = new QStandardItemModel(Global::ferDeviceInfo.Aeration_Runtime_Setting_Category,3);
    connect(aerationRtSettingModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(updateAerationRtSetting(QStandardItem*)));
    ui->runtimeSettingTableView->setModel(aerationRtSettingModel);
    ui->runtimeSettingTableView->setColumnWidth(0, 180);
    //connect(ui->runtimeSettingTableView,SIGNAL(clicked(QModelIndex)),this,SLOT(showModelValue(QModelIndex)));

    ferTimeSettingModel = new QStandardItemModel(Global::ferDeviceInfo.Fermentation_Time_Setting_Category,3);
    connect(ferTimeSettingModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(updateFerTimeSetting(QStandardItem*)));
    ui->ferTimeSettingTableView->setModel(ferTimeSettingModel);
    ui->ferTimeSettingTableView->setColumnWidth(0, 150);

    ferTemptureSettingModel = new QStandardItemModel(Global::ferDeviceInfo.Tempture_Setting_Category,2);
    connect(ferTemptureSettingModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(updateFerTemptureSetting(QStandardItem*)));
    ui->ferTemptureSettingTableView->setModel(ferTemptureSettingModel);
    ui->ferTemptureSettingTableView->setColumnWidth(0, 150);

    //ferHzSettingModel = new QStandardItemModel(Global::ferDeviceInfo.Aeration_Hz_Setting_Category,2);
    //connect(ferHzSettingModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(updateFerHzSetting(QStandardItem*)));
    //ui->ferHzSettingTableView->setModel(ferHzSettingModel);
    //ui->ferHzSettingTableView->setColumnWidth(0, 150);

    minuteDelegate = new RuntimeSettingDelegate(Minute);
    ui->runtimeSettingTableView->setItemDelegateForColumn(1,minuteDelegate);
    secondDelegate = new RuntimeSettingDelegate(Second);
    ui->runtimeSettingTableView->setItemDelegateForColumn(2,minuteDelegate);

    dayDelegate = new RuntimeSettingDelegate(Day);
    ui->ferTimeSettingTableView->setItemDelegateForColumn(1,dayDelegate);
    hourDelegate = new RuntimeSettingDelegate(Hour);
    ui->ferTimeSettingTableView->setItemDelegateForColumn(2,hourDelegate);

    getAerationRtSettingTcpClient = new TcpClientSocket(this);
    connect(getAerationRtSettingTcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showAerationRtSetting(QByteArray)));

    getFerTimeSettingTcpClient = new TcpClientSocket(this);
    connect(getFerTimeSettingTcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showFerTimeSetting(QByteArray)));

    getFerTemptureSettingTcpClient = new TcpClientSocket(this);
    connect(getFerTemptureSettingTcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showFerTemptureSetting(QByteArray)));

    //getFerHzSettingTcpClient = new TcpClientSocket(this);
    //connect(getFerHzSettingTcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showFerHzSetting(QByteArray)));

    saveSettingTcpClient = new TcpClientSocket(this);
    connect(saveSettingTcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showSaveSetting(QByteArray)));
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::showEvent(QShowEvent *)
{
    if(!tankIndexInitialized)
    {
        for(int i=0; i < Global::ferDeviceInfo.Device_Number; i++)
        {
            ui->tankComboBox->addItem( QString::number(i+1) + QStringLiteral("#槽"));
        }
        tankIndexInitialized = true;
    }
}

void SettingDialog::showModelValue(QModelIndex index)
{
    uint value = aerationRtSettingModel->data(index).toUInt();
    qDebug() << "model value" << value;
}

void SettingDialog::showAerationRtSetting(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    aerationRtSettingValues.clear();

    QString msg;
    if(bDevice.bErrorCode==1)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice));
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            uint temp = 0;
            memcpy(&temp,value,4);
            aerationRtSettingValues.append(temp);
            if(!latestAerationRtSettingInitialized)
            {
                latestAerationRtSettingValues.append(temp);
            }
            msg.append(QString::number(temp));
            msg.append(",");
        }

        qDebug() << "setting datas: " << msg;
    }
    else
    {
        qDebug() << bDevice.bDataLength;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            aerationRtSettingValues.append(0);
            if(!latestAerationRtSettingInitialized)
            {
                latestAerationRtSettingValues.append(0);
            }
        }
    }

    aerationRtSettingModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("参数名称"));
    aerationRtSettingModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("分钟"));
    aerationRtSettingModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("秒"));
    aerationRtSettingModel->removeRows(0,aerationRtSettingModel->rowCount(QModelIndex()),QModelIndex());

    int row = 0;
    int index = 0;
    aerationRowIndex.clear();

    QVector<DeviceNode> nodes = Global::ferDeviceNodes;
    foreach (DeviceNode node, nodes) {
        if(node.DataType == "dw" && node.Name.contains("set",Qt::CaseInsensitive)
                && node.Name.contains("aeration",Qt::CaseInsensitive)
                && node.Name.contains("time",Qt::CaseInsensitive))
        {
            index = (node.Offset - node.StartAddress)
                    / (Global::getLengthByDataType(node.DataType) * Global::ferDeviceInfo.Device_Number);
            aerationRowIndex.append(index);

            aerationRtSettingModel->insertRows(row,1,QModelIndex());
            aerationRtSettingModel->setData(aerationRtSettingModel->index(row,0,QModelIndex()),node.Cname);
            aerationRtSettingModel->setData(aerationRtSettingModel->index(row,1,QModelIndex()),aerationRtSettingValues[index] / 60);
            aerationRtSettingModel->setData(aerationRtSettingModel->index(row,2,QModelIndex()),aerationRtSettingValues[index] % 60);

            row++;
        }
    }

    if(!latestAerationRtSettingInitialized)
    {
        latestAerationRtSettingInitialized = true;
    }
    aerationRtSettingInitialized = true;
}

void SettingDialog::showFerTimeSetting(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    ferTimeSettingValues.clear();
    QString msg;
    if(bDevice.bErrorCode==1)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice));
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            uint temp = 0;
            memcpy(&temp,value,4);
            ferTimeSettingValues.append(temp);
            if(!latestFerTimeSettingInitialized)
            {
                latestFerTimeSettingValues.append(temp);
            }
            msg.append(QString::number(temp));
            msg.append(",");
        }

        qDebug() << "setting datas: " << msg;
    }
    else
    {
        qDebug() << bDevice.bDataLength;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            ferTimeSettingValues.append(0);
            if(!latestFerTimeSettingInitialized)
            {
                latestFerTimeSettingValues.append(0);
            }
        }
    }

    ferTimeSettingModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("参数名称"));
    ferTimeSettingModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("天"));
    ferTimeSettingModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("小时"));
    ferTimeSettingModel->removeRows(0,ferTimeSettingModel->rowCount(QModelIndex()),QModelIndex());

    int row = 0;
    int index = 0;
    timeRowIndex.clear();

    QVector<DeviceNode> nodes = Global::ferDeviceNodes;
    foreach (DeviceNode node, nodes) {
        if(node.DataType == "dw" && node.Name.contains("fer_time",Qt::CaseInsensitive) && node.Name.contains("set",Qt::CaseInsensitive))
        {
            index = (node.Offset - node.StartAddress)
                    / (Global::getLengthByDataType(node.DataType) * Global::ferDeviceInfo.Device_Number);
            timeRowIndex.append(index);

            ferTimeSettingModel->insertRows(row,1,QModelIndex());
            ferTimeSettingModel->setData(ferTimeSettingModel->index(row,0,QModelIndex()),node.Cname);
            ferTimeSettingModel->setData(ferTimeSettingModel->index(row,1,QModelIndex()),ferTimeSettingValues[index] / 86400);
            ferTimeSettingModel->setData(ferTimeSettingModel->index(row,2,QModelIndex()),(ferTimeSettingValues[index] % 86400) / 3600);
            row++;
        }
    }

    if(!latestFerTimeSettingInitialized)
    {
        latestFerTimeSettingInitialized = true;
    }
    ferTimeSettingInitialized = true;
}

void SettingDialog::showFerTemptureSetting(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    ferTemptureSettingValues.clear();
    QString msg;
    if(bDevice.bErrorCode==1)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice));
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            float temp = 0;
            memcpy(&temp,value,4);
            ferTemptureSettingValues.append(temp);
            if(!latestFerTemptureSettingInitialized)
            {
                latestFerTemptureSettingValues.append(temp);
            }
            msg.append(QString::number(temp));
            msg.append(",");
        }

        qDebug() << "setting datas: " << msg;
    }
    else
    {
        qDebug() << bDevice.bDataLength;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            ferTemptureSettingValues.append(0);
            if(!latestFerTemptureSettingInitialized)
            {
                latestFerTemptureSettingValues.append(0);
            }
        }
    }

    ferTemptureSettingModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("参数名称"));
    ferTemptureSettingModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("℃"));
    ferTemptureSettingModel->removeRows(0,ferTemptureSettingModel->rowCount(QModelIndex()),QModelIndex());

    int row = 0;
    int index = 0;
    temptureRowIndex.clear();

    QVector<DeviceNode> nodes = Global::ferDeviceNodes;
    foreach (DeviceNode node, nodes) {
        if(node.DataType == "r" && node.Name.contains("fer_t_",Qt::CaseInsensitive) && node.Name.contains("set",Qt::CaseInsensitive))
        {
            index = (node.Offset - node.StartAddress)
                    / (Global::getLengthByDataType(node.DataType) * Global::ferDeviceInfo.Device_Number);

            temptureRowIndex.append(index);
            ferTemptureSettingModel->insertRows(row,1,QModelIndex());
            ferTemptureSettingModel->setData(ferTemptureSettingModel->index(row,0,QModelIndex()),node.Cname);
            ferTemptureSettingModel->setData(ferTemptureSettingModel->index(row,1,QModelIndex()),ferTemptureSettingValues[index]);
            row++;
        }
    }

    if(!latestFerTemptureSettingInitialized)
    {
        latestFerTemptureSettingInitialized = true;
    }
    ferTemptureSettingInitialized = true;
}

void SettingDialog::showFerHzSetting(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    ferHzSettingValues.clear();
    QString msg;
    if(bDevice.bErrorCode==1)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice));
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            float temp = 0;
            memcpy(&temp,value,4);
            ferHzSettingValues.append(temp);
            if(!latestFerHzSettingInitialized)
            {
                latestFerHzSettingValues.append(temp);
            }
            msg.append(QString::number(temp));
            msg.append(",");
        }

        qDebug() << "setting datas: " << msg;
    }
    else
    {
        qDebug() << bDevice.bDataLength;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            ferHzSettingValues.append(0);
            if(!latestFerHzSettingInitialized)
            {
                latestFerHzSettingValues.append(0);
            }
        }
    }

    ferHzSettingModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("参数名称"));
    ferHzSettingModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("HZ"));
    ferHzSettingModel->removeRows(0,ferHzSettingModel->rowCount(QModelIndex()),QModelIndex());

    int row = 0;
    int index = 0;
    hzRowIndex.clear();

    QVector<DeviceNode> nodes = Global::ferDeviceNodes;
    foreach (DeviceNode node, nodes) {
        if(node.DataType == "dw" && node.Name.contains("aerationhz_",Qt::CaseInsensitive) && node.Name.contains("set",Qt::CaseInsensitive))
        {
            index = (node.Offset - node.StartAddress)
                    / (Global::getLengthByDataType(node.DataType) * Global::ferDeviceInfo.Device_Number);
            hzRowIndex.append(index);

            ferHzSettingModel->insertRows(row,1,QModelIndex());
            ferHzSettingModel->setData(ferHzSettingModel->index(row,0,QModelIndex()),node.Cname);
            ferHzSettingModel->setData(ferHzSettingModel->index(row,1,QModelIndex()),ferHzSettingValues[index]);

            row++;
        }
    }

    if(!latestFerHzSettingInitialized)
    {
        latestFerHzSettingInitialized = true;
    }
    ferHzSettingInitialized = true;
}

void SettingDialog::getAerationRtSetting()
{
    StreamPack bpack;
    QDateTime todayTime = QDateTime::currentDateTime();
    uint stime = todayTime.toTime_t();
    uint etime = todayTime.toTime_t();
    ushort address = Global::ferDeviceInfo.Aeration_Runtime_Setting_Address + 4 * ui->tankComboBox->currentIndex();
    qDebug() << "Global::ferDeviceInfo.Aeration_Runtime_Setting_Category:" << Global::ferDeviceInfo.Aeration_Runtime_Setting_Category;
    bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,r_SetData,UInt,address,(quint16)ui->tankComboBox->currentIndex(),Global::ferDeviceInfo.Aeration_Runtime_Setting_Category,0,stime,etime};

    bpack.bStreamLength += 2 * Global::ferDeviceInfo.Aeration_Runtime_Setting_Category + 4;

    quint16 strlen= sizeof(bpack);
    char *p = new char[strlen];
    QByteArray SData;
    memcpy(p,reinterpret_cast<char*>(&bpack),sizeof(bpack));

    QDataStream out(&SData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out.setByteOrder(QDataStream::LittleEndian);
    for(int i=0;i<strlen;i++) {out << *(p+i);}
    delete p;

    int len = SData.length();
    QVector<DeviceNode> nodes = Global::ferDeviceNodes;
    aerationRtSettingAddress.clear();
    foreach (DeviceNode node, nodes) {
        if(node.DataType == "dw" && node.Name.contains("set",Qt::CaseInsensitive)
                && node.Name.contains("aeration",Qt::CaseInsensitive)
                && node.Name.contains("time",Qt::CaseInsensitive))
        {
            out << (ushort)(node.Offset  + 4 * (ui->tankComboBox->currentIndex()));
            aerationRtSettingAddress.append(node.Offset + 4 * ui->tankComboBox->currentIndex());
        }
    }

    len = SData.length();
    uint scrc = getAerationRtSettingTcpClient->StreamLen_CRC32(SData);
    out << scrc;

    getAerationRtSettingTcpClient->sendRequestWithResults(SData);
}

void SettingDialog::getFerTimeSetting()
{
    StreamPack bpack;
    QDateTime todayTime = QDateTime::currentDateTime();
    uint stime = todayTime.toTime_t();
    uint etime = todayTime.toTime_t();
    ushort address = Global::ferDeviceInfo.Fermentation_Time_Setting_Address + 4 * ui->tankComboBox->currentIndex();
    qDebug() << "Current fermenation time setting address:" << address;
    bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,r_SetData,UInt,address,(quint16)ui->tankComboBox->currentIndex(),Global::ferDeviceInfo.Fermentation_Time_Setting_Category,0,stime,etime};

    bpack.bStreamLength += 2 * Global::ferDeviceInfo.Fermentation_Time_Setting_Category + 4;

    quint16 strlen= sizeof(bpack);
    char *p = new char[strlen];
    QByteArray SData;
    memcpy(p,reinterpret_cast<char*>(&bpack),sizeof(bpack));

    QDataStream out(&SData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out.setByteOrder(QDataStream::LittleEndian);
    for(int i=0;i<strlen;i++) {out << *(p+i);}
    delete p;

    int len = SData.length();
    QVector<DeviceNode> nodes = Global::ferDeviceNodes;
    ferTimeRtSettingAddress.clear();
    foreach (DeviceNode node, nodes) {
        if(node.DataType == "dw" && node.Name.contains("fer_time",Qt::CaseInsensitive) && node.Name.contains("set",Qt::CaseInsensitive))
        {
            out << (ushort)(node.Offset  + 4 * (ui->tankComboBox->currentIndex()));
            ferTimeRtSettingAddress.append(node.Offset + 4 * ui->tankComboBox->currentIndex());
        }
    }

    len = SData.length();
    uint scrc = getFerTimeSettingTcpClient->StreamLen_CRC32(SData);
    out << scrc;

    getFerTimeSettingTcpClient->sendRequestWithResults(SData);
}

void SettingDialog::getFerTemptureSetting()
{
    StreamPack bpack;
    QDateTime todayTime = QDateTime::currentDateTime();
    uint stime = todayTime.toTime_t();
    uint etime = todayTime.toTime_t();
    ushort address = Global::ferDeviceInfo.Tempture_Setting_Address + 4 * ui->tankComboBox->currentIndex();
    qDebug() << "Current tempture setting address:" << address;
    bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,r_SetData,Float,address,(quint16)ui->tankComboBox->currentIndex(),Global::ferDeviceInfo.Tempture_Setting_Category,0,stime,etime};

    bpack.bStreamLength += 2 * Global::ferDeviceInfo.Tempture_Setting_Category + 4;

    quint16 strlen= sizeof(bpack);
    char *p = new char[strlen];
    QByteArray SData;
    memcpy(p,reinterpret_cast<char*>(&bpack),sizeof(bpack));

    QDataStream out(&SData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out.setByteOrder(QDataStream::LittleEndian);
    for(int i=0;i<strlen;i++) {out << *(p+i);}
    delete p;

    int len = SData.length();
    QVector<DeviceNode> nodes = Global::ferDeviceNodes;
    ferTemptureSettingAddress.clear();
    foreach (DeviceNode node, nodes) {
        if(node.DataType == "r" && node.Name.contains("fer_t_",Qt::CaseInsensitive) && node.Name.contains("set",Qt::CaseInsensitive))
        {
            out << (ushort)(node.Offset  + 4 * (ui->tankComboBox->currentIndex()));
            ferTemptureSettingAddress.append(node.Offset + 4 * ui->tankComboBox->currentIndex());
            qDebug() << "Tempture Address: " << node.Offset + 4 * ui->tankComboBox->currentIndex();
        }
    }

    len = SData.length();
    uint scrc = getFerTemptureSettingTcpClient->StreamLen_CRC32(SData);
    out << scrc;

    getFerTemptureSettingTcpClient->sendRequestWithResults(SData);
}

void SettingDialog::getFerHzSetting()
{
    StreamPack bpack;
    QDateTime todayTime = QDateTime::currentDateTime();
    uint stime = todayTime.toTime_t();
    uint etime = todayTime.toTime_t();
    ushort address = Global::ferDeviceInfo.Aeration_Hz_Setting_Address + 4 * ui->tankComboBox->currentIndex();
    qDebug() << "Current tempture setting address:" << address;
    bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,r_SetData,UInt,address,(quint16)ui->tankComboBox->currentIndex(),Global::ferDeviceInfo.Aeration_Hz_Setting_Category,0,stime,etime};

    bpack.bStreamLength += 2 * Global::ferDeviceInfo.Aeration_Hz_Setting_Category + 4;

    quint16 strlen= sizeof(bpack);
    char *p = new char[strlen];
    QByteArray SData;
    memcpy(p,reinterpret_cast<char*>(&bpack),sizeof(bpack));

    QDataStream out(&SData,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);
    out.setByteOrder(QDataStream::LittleEndian);
    for(int i=0;i<strlen;i++) {out << *(p+i);}
    delete p;

    int len = SData.length();
    QVector<DeviceNode> nodes = Global::ferDeviceNodes;
    ferHzSettingRtSettingAddress.clear();
    foreach (DeviceNode node, nodes) {
        if(node.DataType == "dw" && node.Name.contains("aerationhz_",Qt::CaseInsensitive) && node.Name.contains("set",Qt::CaseInsensitive))
        {
            out << (ushort)(node.Offset  + 4 * (ui->tankComboBox->currentIndex()));
            ferHzSettingRtSettingAddress.append(node.Offset + 4 * ui->tankComboBox->currentIndex());
        }
    }

    len = SData.length();
    uint scrc = getFerHzSettingTcpClient->StreamLen_CRC32(SData);
    out << scrc;

    getFerHzSettingTcpClient->sendRequestWithResults(SData);
}

void SettingDialog::updateAerationRtSetting(QStandardItem *item)
{
    if(aerationRtSettingInitialized)
    {
        uint newValue = item->data(Qt::EditRole).toUInt();
        if(item->column() == 1)
        {
            aerationRtSettingValues[aerationRowIndex[item->row()]] = newValue * 60 + aerationRtSettingValues[aerationRowIndex[item->row()]] % 60;
        }
        if(item->column() == 2)
        {
            aerationRtSettingValues[aerationRowIndex[item->row()]] = aerationRtSettingValues[aerationRowIndex[item->row()]]
                    - aerationRtSettingValues[aerationRowIndex[item->row()]] % 60 + newValue;
        }
    }
}

void SettingDialog::updateFerTimeSetting(QStandardItem *item)
{
    if(ferTimeSettingInitialized)
    {
        uint newValue = item->data(Qt::EditRole).toUInt();
        if(item->column() == 1)
        {
            ferTimeSettingValues[timeRowIndex[item->row()]] = newValue * 86400 + ferTimeSettingValues[timeRowIndex[item->row()]] % 86400;
        }
        if(item->column() == 2)
        {
            ferTimeSettingValues[timeRowIndex[item->row()]] = ferTimeSettingValues[timeRowIndex[item->row()]]
                    - ferTimeSettingValues[timeRowIndex[item->row()]] % 86400 + newValue * 3600;
        }
    }
}

void SettingDialog::updateFerTemptureSetting(QStandardItem *item)
{
    if(ferTemptureSettingInitialized)
    {
        float newValue = item->data(Qt::EditRole).toFloat();
        if(item->column() == 1)
        {
            ferTemptureSettingValues[temptureRowIndex[item->row()]] = newValue;
        }
    }
}

void SettingDialog::updateFerHzSetting(QStandardItem *item)
{
    if(ferHzSettingInitialized)
    {
        float newValue = item->data(Qt::EditRole).toFloat();
        if(item->column() == 1)
        {
            ferHzSettingValues[hzRowIndex[item->row()]] = newValue;
        }
    }
}

void SettingDialog::showSaveSetting(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {
        msgBox.setText(QStringLiteral("设置更新成功！"));
        msgBox.show();
    }
}

void SettingDialog::on_saveAerationRtSettingButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort address = Global::ferDeviceInfo.Aeration_Runtime_Setting_Address + 4 * ui->tankComboBox->currentIndex();
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_SetData,UInt,address,(quint16)ui->tankComboBox->currentIndex(),Global::ferDeviceInfo.Aeration_Runtime_Setting_Category,0,stime,etime};
        //bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_SetData,UInt,address,0,Global::ferDeviceInfo.Aeration_Runtime_Setting_Category,0,stime,etime};

        bpack.bStreamLength += Global::ferDeviceInfo.Aeration_Runtime_Setting_Category * (2 + 4) + 4;

        quint16 strlen= sizeof(bpack);
        char *p = new char[strlen];
        QByteArray SData;
        memcpy(p,reinterpret_cast<char*>(&bpack),sizeof(bpack));
        QDataStream out(&SData,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out.setByteOrder(QDataStream::LittleEndian);
        for(int i=0;i<strlen;i++) {out << *(p+i);}
        delete p;

        int len = SData.length();
        foreach(ushort value, aerationRtSettingAddress)
        {
            out << value;
        }

        len = SData.length();
        foreach(int value, aerationRtSettingValues)
        {
            out << value;
        }

        len = SData.length();
        uint scrc = saveSettingTcpClient->StreamLen_CRC32(SData);
        out << scrc;

        saveSettingTcpClient->sendRequestWithResults(SData);

        //QVariant var_data = QVariant::fromValue(aerationRtSettingValues.toList());
        //saveSettingTcpClient->sendRequestWithResult(bpack,var_data,Global::ferDeviceInfo.Aeration_Runtime_Setting_Category * 4);

        latestAerationRtSettingValues.clear();
        foreach(uint value, aerationRtSettingValues)
        {
            latestAerationRtSettingValues.append(value);
        }
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SettingDialog::on_tankComboBox_currentIndexChanged(int index)
{
    ferTemptureSettingInitialized = false;
    ferTimeSettingInitialized = false;
    aerationRtSettingInitialized = false;
    //ferHzSettingInitialized = false;
    getAerationRtSetting();
    getFerTimeSetting();
    getFerTemptureSetting();
    //getFerHzSetting();
}

void SettingDialog::on_saveFerTimeSettingButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort address = Global::ferDeviceInfo.Fermentation_Time_Setting_Address + 4 * ui->tankComboBox->currentIndex();
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_SetData,UInt,address,(quint16)ui->tankComboBox->currentIndex(),Global::ferDeviceInfo.Fermentation_Time_Setting_Category,0,stime,etime};

        bpack.bStreamLength += Global::ferDeviceInfo.Fermentation_Time_Setting_Category * (2 + 4) + 4;

        quint16 strlen= sizeof(bpack);
        char *p = new char[strlen];
        QByteArray SData;
        memcpy(p,reinterpret_cast<char*>(&bpack),sizeof(bpack));
        QDataStream out(&SData,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out.setByteOrder(QDataStream::LittleEndian);
        for(int i=0;i<strlen;i++) {out << *(p+i);}
        delete p;

        int len = SData.length();
        foreach(ushort value, ferTimeRtSettingAddress)
        {
            out << value;
        }

        len = SData.length();
        foreach(int value, ferTimeSettingValues)
        {
            out << value;
        }

        len = SData.length();
        uint scrc = saveSettingTcpClient->StreamLen_CRC32(SData);
        out << scrc;

        saveSettingTcpClient->sendRequestWithResults(SData);

        //QVariant var_data = QVariant::fromValue(ferTimeSettingValues.toList());
        //saveSettingTcpClient->sendRequestWithResult(bpack,var_data,Global::ferDeviceInfo.Fermentation_Time_Setting_Category * 4);

        latestFerTimeSettingValues.clear();
        qDebug() << "original: " << latestFerTimeSettingValues.length();
        foreach(uint value, ferTimeSettingValues)
        {
            latestFerTimeSettingValues.append(value);
        }
        qDebug() << "latest: " << latestFerTimeSettingValues.length();
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SettingDialog::on_saveFerTemptureSettingButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort address = Global::ferDeviceInfo.Tempture_Setting_Address + 4 * ui->tankComboBox->currentIndex();
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_SetData,Float,address,(quint16)ui->tankComboBox->currentIndex(),Global::ferDeviceInfo.Tempture_Setting_Category,0,stime,etime};

        bpack.bStreamLength += Global::ferDeviceInfo.Tempture_Setting_Category * (2 + 4) + 4;

        quint16 strlen= sizeof(bpack);
        char *p = new char[strlen];
        QByteArray SData;
        memcpy(p,reinterpret_cast<char*>(&bpack),sizeof(bpack));
        QDataStream out(&SData,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out.setByteOrder(QDataStream::LittleEndian);
        for(int i=0;i<strlen;i++) {out << *(p+i);}
        delete p;

        int len = SData.length();
        foreach(ushort value, ferTemptureSettingAddress)
        {
            out << value;
        }

        len = SData.length();
        foreach(float value, ferTemptureSettingValues)
        {
            out << value;
        }

        len = SData.length();
        uint scrc = saveSettingTcpClient->StreamLen_CRC32(SData);
        out << scrc;

        saveSettingTcpClient->sendRequestWithResults(SData);

        //QVariant var_data = QVariant::fromValue(ferTemptureSettingValues.toList());
        //saveSettingTcpClient->sendRequestWithResult(bpack,var_data,Global::ferDeviceInfo.Tempture_Setting_Category * 4);

        latestFerTemptureSettingValues.clear();
        foreach(float value, ferTemptureSettingValues)
        {
            latestFerTemptureSettingValues.append(value);
        }
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void SettingDialog::on_copySettingButton_clicked()
{
    int row = 0;
    QVector<DeviceNode> nodes = Global::ferDeviceNodes;

    aerationRtSettingModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("参数名称"));
    aerationRtSettingModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("分钟"));
    aerationRtSettingModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("秒"));
    aerationRtSettingModel->removeRows(0,aerationRtSettingModel->rowCount(QModelIndex()),QModelIndex());

    foreach (DeviceNode node, nodes) {
        if(node.DataType == "dw" && node.Name.contains("set",Qt::CaseInsensitive) && node.Name.contains("aeration",Qt::CaseInsensitive))
        {
            aerationRtSettingModel->insertRows(row,1,QModelIndex());
            aerationRtSettingModel->setData(aerationRtSettingModel->index(row,0,QModelIndex()),node.Cname);
            aerationRtSettingModel->setData(aerationRtSettingModel->index(row,1,QModelIndex()),latestAerationRtSettingValues[row] / 60);
            aerationRtSettingModel->setData(aerationRtSettingModel->index(row,2,QModelIndex()),latestAerationRtSettingValues[row] % 60);
            row++;
        }
    }

     for(int i=0; i < latestAerationRtSettingValues.length(); i++)
     {
         aerationRtSettingValues[i] = latestAerationRtSettingValues[i];
     }

    row = 0;

    ferTimeSettingModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("参数名称"));
    ferTimeSettingModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("天"));
    ferTimeSettingModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("小时"));
    ferTimeSettingModel->removeRows(0,ferTimeSettingModel->rowCount(QModelIndex()),QModelIndex());

    foreach (DeviceNode node, nodes) {
        if(node.DataType == "dw" && node.Name.contains("fer_time",Qt::CaseInsensitive) && node.Name.contains("set",Qt::CaseInsensitive))
        {
            ferTimeSettingModel->insertRows(row,1,QModelIndex());
            ferTimeSettingModel->setData(ferTimeSettingModel->index(row,0,QModelIndex()),node.Cname);
            ferTimeSettingModel->setData(ferTimeSettingModel->index(row,1,QModelIndex()),latestFerTimeSettingValues[row] / 86400);
            ferTimeSettingModel->setData(ferTimeSettingModel->index(row,2,QModelIndex()),(latestFerTimeSettingValues[row] % 86400) / 3600);
            row++;
        }
    }

    for(int i=0; i < latestFerTimeSettingValues.length(); i++)
    {
        ferTimeSettingValues[i] = latestFerTimeSettingValues[i];
    }

    row = 0;

    ferTemptureSettingModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("参数名称"));
    ferTemptureSettingModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("℃"));
    ferTemptureSettingModel->removeRows(0,ferTemptureSettingModel->rowCount(QModelIndex()),QModelIndex());

    foreach (DeviceNode node, nodes) {
        if(node.DataType == "r" && node.Name.contains("fer_t_",Qt::CaseInsensitive) && node.Name.contains("set",Qt::CaseInsensitive))
        {
            ferTemptureSettingModel->insertRows(row,1,QModelIndex());
            ferTemptureSettingModel->setData(ferTemptureSettingModel->index(row,0,QModelIndex()),node.Cname);
            ferTemptureSettingModel->setData(ferTemptureSettingModel->index(row,1,QModelIndex()),latestFerTemptureSettingValues[row]);
            row++;
        }
    }

    for(int i=0; i < latestFerTemptureSettingValues.length(); i++)
    {
        ferTemptureSettingValues[i] = latestFerTemptureSettingValues[i];
    }

    /*row = 0;

    ferHzSettingModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("参数名称"));
    ferHzSettingModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("HZ"));
    ferHzSettingModel->removeRows(0,ferHzSettingModel->rowCount(QModelIndex()),QModelIndex());

    foreach (DeviceNode node, nodes) {
        if(node.DataType == "dw" && node.Name.contains("aerationhz_",Qt::CaseInsensitive) && node.Name.contains("set",Qt::CaseInsensitive))
        {
            ferHzSettingModel->insertRows(row,1,QModelIndex());
            ferHzSettingModel->setData(ferHzSettingModel->index(row,0,QModelIndex()),node.Cname);
            ferHzSettingModel->setData(ferHzSettingModel->index(row,1,QModelIndex()),latestFerHzSettingValues[row]);
            row++;
        }
    }

    for(int i=0; i < latestFerHzSettingValues.length(); i++)
    {
        ferHzSettingValues[i] = latestFerHzSettingValues[i];
    }*/
}

void SettingDialog::on_saveFerHzSettingButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();
        ushort address = Global::ferDeviceInfo.Aeration_Hz_Setting_Address + 4 * ui->tankComboBox->currentIndex();
        bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,w_SetData,UInt,address,(quint16)ui->tankComboBox->currentIndex(),Global::ferDeviceInfo.Aeration_Hz_Setting_Category,0,stime,etime};

        bpack.bStreamLength += Global::ferDeviceInfo.Aeration_Hz_Setting_Category * (2 + 4) + 4;

        quint16 strlen= sizeof(bpack);
        char *p = new char[strlen];
        QByteArray SData;
        memcpy(p,reinterpret_cast<char*>(&bpack),sizeof(bpack));
        QDataStream out(&SData,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_6); //设计数据流版本
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);
        //QDataStream::BigEndian或QDataStream::LittleEndian
        out.setByteOrder(QDataStream::LittleEndian);
        for(int i=0;i<strlen;i++) {out << *(p+i);}
        delete p;

        int len = SData.length();
        foreach(ushort value, ferHzSettingRtSettingAddress)
        {
            out << value;
        }

        len = SData.length();
        foreach(int value, ferHzSettingValues)
        {
            out << value;
        }

        len = SData.length();
        uint scrc = saveSettingTcpClient->StreamLen_CRC32(SData);
        out << scrc;

        saveSettingTcpClient->sendRequestWithResults(SData);

        //QVariant var_data = QVariant::fromValue(ferHzSettingValues.toList());
        //saveSettingTcpClient->sendRequestWithResult(bpack,var_data,Global::ferDeviceInfo.Aeration_Hz_Setting_Category * 4);

        latestFerHzSettingValues.clear();
        foreach(uint value, ferHzSettingValues)
        {
            latestFerHzSettingValues.append(value);
        }
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}
