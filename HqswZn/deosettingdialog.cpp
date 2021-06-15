#include "deosettingdialog.h"
#include "ui_deosettingdialog.h"
#include "global.h"
#include "identity.h"

//#include <winsock.h>

DeoSettingDialog::DeoSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeoSettingDialog)
{
    ui->setupUi(this);
    //setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);

    cbxDelegate = new CheckBoxDelegate(this);
    deoRtSettingModel = new QStandardItemModel(Global::deoDeviceInfo.Deo_Setting_Display_Num,3);
    connect(deoRtSettingModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(updateDeoRtSetting(QStandardItem*)));
    ui->deoSettingTableView->setModel(deoRtSettingModel);
    ui->deoSettingTableView->setColumnWidth(0, 200);

    ui->deoSettingTableView->setItemDelegateForColumn(2,cbxDelegate);

    getDeoSettingTcpClient = new TcpClientSocket(this);
    connect(getDeoSettingTcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showDeoRtSetting(QByteArray)));

    saveSettingTcpClient = new TcpClientSocket(this);
    connect(saveSettingTcpClient, SIGNAL(updateClients(QByteArray)),this,SLOT(showSaveSetting(QByteArray)));
}

DeoSettingDialog::~DeoSettingDialog()
{
    delete ui;
}

void DeoSettingDialog::showEvent(QShowEvent *)
{
    for(int i=0; i < Global::deoDeviceGroupInfos.length();i++)
    {
        QString itemName = QString::number(i + 1) + QStringLiteral("号生物除臭滤池");
        ui->deviceIndexComboBox->addItem(itemName);
    }
}

void DeoSettingDialog::closeEvent(QCloseEvent *)
{
    ui->deviceIndexComboBox->clear();
}

void DeoSettingDialog::getDeoRtSetting()
{
    StreamPack bpack;
    QDateTime todayTime = QDateTime::currentDateTime();
    uint stime = todayTime.toTime_t();
    uint etime = todayTime.toTime_t();

    int deviceIndex = ui->deviceIndexComboBox->currentIndex();
    if(deviceIndex >= 0)
    {
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);

        ushort address = Global::deoDeviceInfo.Deo_Setting_Address + (info.offset + deviceIndex - info.startIndex) * 4;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,r_SetData,Int,address,(quint16)deviceIndex,Global::deoDeviceInfo.Deo_Setting_Category,0,stime,etime};
        bpack.bStreamLength += 2 * Global::deoDeviceInfo.Deo_Setting_Category + 4;

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
        QVector<DeviceNode> nodes = Global::deoDeviceNodes;
        deoRtSettingAddress.clear();
        foreach (DeviceNode node, nodes) {
            if(node.DataType == "di" && node.Category.toLower() == "deodoration_setting")
            {
                out << (ushort)(node.Offset + 4 * ui->deviceIndexComboBox->currentIndex());
                deoRtSettingAddress.append((ushort)(node.Offset + 4 * ui->deviceIndexComboBox->currentIndex()));
            }
        }

        len = SData.length();
        uint scrc = getDeoSettingTcpClient->StreamLen_CRC32(SData);
        out << scrc;

        getDeoSettingTcpClient->sendRequestWithResults(SData);
    }
}

void DeoSettingDialog::showDeoRtSetting(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QString msg;
    if(bDevice.bErrorCode==1)
    {
        deoRtSettingValues.clear();
        QByteArray byteValues = data.mid(sizeof(bDevice));
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            uint temp = 0;
            memcpy(&temp,value,4);
            deoRtSettingValues.append(temp);
            qDebug() << "deoRtSettingValues " << QString::number(i) << " = " << temp;
            if(!latestDeoRtSettingInitialized)
            {
                latestDeoRtSettingValues.append(temp);
            }
            msg.append(QString::number(temp));
            msg.append(",");
        }

        qDebug() << "setting datas: " << msg;

        deoRtSettingModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("参数名称"));
        deoRtSettingModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("值"));
        deoRtSettingModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("启动"));
        deoRtSettingModel->removeRows(0,deoRtSettingModel->rowCount(QModelIndex()),QModelIndex());

        uint row = 0;
        int index = 0;
        rowIndex.clear();

        QVector<DeviceNode> nodes = Global::deoDeviceNodes;
        foreach (DeviceNode node, nodes) {
            if(node.DataType == "di" && node.Name.contains("set",Qt::CaseInsensitive)
                    && node.Name.contains("hmi_",Qt::CaseInsensitive) && !node.Name.contains("check", Qt::CaseInsensitive))
            {
                index = (node.Offset - node.StartAddress)
                        / (Global::getLengthByDataType(node.DataType) * Global::deoDeviceInfo.Device_Number);
                rowIndex.append(index);
                deoRtSettingModel->insertRows(row,1,QModelIndex());
                deoRtSettingModel->setData(deoRtSettingModel->index(row,0,QModelIndex()),node.Cname);
                deoRtSettingModel->setData(deoRtSettingModel->index(row,1,QModelIndex()),deoRtSettingValues[index]);

                DeviceNode checkNode = Global::getDeodorationNodeInfoByName(node.Name + "_Check");
                uint state = 0;
                if(checkNode.Name == "NULL")
                {
                    state = 1;
                }
                else
                {
                    index = (checkNode.Offset - checkNode.StartAddress)
                            / (Global::getLengthByDataType(checkNode.DataType) * Global::deoDeviceInfo.Device_Number);
                    state = deoRtSettingValues[index];
                }
                if(state == 0)
                {
                   deoRtSettingModel->setData(deoRtSettingModel->index(row,2,QModelIndex()),false, Qt::UserRole);
                   deoRtSettingModel->item(row, 2)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
                   deoRtSettingModel->item(row, 2)->setCheckState(Qt::Unchecked);
                }
                else
                {
                   deoRtSettingModel->setData(deoRtSettingModel->index(row,2,QModelIndex()),true, Qt::UserRole);
                   deoRtSettingModel->item(row, 2)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
                   deoRtSettingModel->item(row, 2)->setCheckState(Qt::Checked);
                }

                row++;
            }
        }

        if(!latestDeoRtSettingInitialized)
        {
            latestDeoRtSettingInitialized = true;
        }
        deoRtSettingInitialized = true;
    }
}

void DeoSettingDialog::showSaveSetting(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    if(bDevice.bErrorCode==1)
    {
        msgBox.setText(QStringLiteral("设置更新成功！"));
        msgBox.show();
    }
}

void DeoSettingDialog::updateDeoRtSetting(QStandardItem *item)
{
    if(deoRtSettingInitialized)
    {        
        if(item->column() == 1)
        {
            uint newValue = item->data(Qt::EditRole).toUInt();
            deoRtSettingValues[rowIndex[item->row()]] = newValue;
        }
        else if(item->column() == 2)
        {
            qDebug() << "item row = " << item->row();
            bool newValue = item->data(Qt::UserRole).toBool();
            QString cname = deoRtSettingModel->item(item->row(), 0)->data(Qt::EditRole).toString();
            qDebug() << "cname = " << cname;
            DeviceNode checkNode = Global::getDeodorationNodeInfoByCname(cname);
            qDebug() << "name = " << checkNode.Name;
            try
            {
                checkNode = Global::getDeodorationNodeInfoByName(checkNode.Name + "_Check");
                if(checkNode.Name != "NULL")
                {
                    int index = (checkNode.Offset - checkNode.StartAddress)
                            / (Global::getLengthByDataType(checkNode.DataType) * Global::deoDeviceInfo.Device_Number);
                    if(newValue)
                    {
                        deoRtSettingValues[index] = 1;
                    }
                    else
                    {
                        deoRtSettingValues[index] = 0;
                    }
                }
            }
            catch(QException e)
            {

            }
        }
    }
}

void DeoSettingDialog::on_saveSettingPushButton_clicked()
{
    User *user = Identity::getInstance()->getUser();
    if(user != Q_NULLPTR)
    {
        QDateTime currentdt = QDateTime::currentDateTime();
        StreamPack bpack;
        uint stime =currentdt.toTime_t();
        uint etime =currentdt.toTime_t();

        int deviceIndex = ui->deviceIndexComboBox->currentIndex();
        DeviceGroupInfo info = Global::getDeoDeviceGroupInfo(deviceIndex);

        ushort address = Global::deoDeviceInfo.Deo_Setting_Address + (info.offset + deviceIndex - info.startIndex) * 4;
        bpack = {sizeof(StreamPack),(quint16)info.deviceId,(quint16)info.groupId,w_SetData,UInt,address,(quint16)deviceIndex,Global::deoDeviceInfo.Deo_Setting_Category,0,stime,etime};
        bpack.bStreamLength += Global::deoDeviceInfo.Deo_Setting_Category * (2 + 4) + 4;

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
        foreach(ushort value, deoRtSettingAddress)
        {
            out << value;
        }

        len = SData.length();
        foreach(int value, deoRtSettingValues)
        {
            out << value;
        }

        len = SData.length();
        uint scrc = saveSettingTcpClient->StreamLen_CRC32(SData);
        out << scrc;

        saveSettingTcpClient->sendRequestWithResults(SData);

        //QVariant var_data = QVariant::fromValue(deoRtSettingValues.toList());
        //saveSettingTcpClient->sendRequestWithResult(bpack,var_data,Global::deoDeviceInfo.Deo_Setting_Category * 4);

        latestDeoRtSettingValues.clear();
        foreach(int value, deoRtSettingValues)
        {
            latestDeoRtSettingValues.append(value);
            qDebug() << "latestDeoRtSettingValues" << value;
        }
    }
    else
    {
        msgBox.setText(QStringLiteral("请先登录后再进行操作！"));
        msgBox.show();
    }
}

void DeoSettingDialog::on_deviceIndexComboBox_currentIndexChanged(int index)
{
    deoRtSettingInitialized = false;
    getDeoRtSetting();
}

void DeoSettingDialog::on_copySettingButton_clicked()
{
    int index = 0;
    uint row = 0;
    QVector<DeviceNode> nodes = Global::deoDeviceNodes;

    deoRtSettingModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("参数名称"));
    deoRtSettingModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("值"));
    deoRtSettingModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("启动"));
    deoRtSettingModel->removeRows(0,deoRtSettingModel->rowCount(QModelIndex()),QModelIndex());

    foreach (DeviceNode node, nodes) {
        if(node.DataType == "di" && node.Name.contains("set",Qt::CaseInsensitive)
                && node.Name.contains("hmi_",Qt::CaseInsensitive) && !node.Name.contains("check",Qt::CaseInsensitive))
        {
            index = (node.Offset - node.StartAddress)
                    / Global::getLengthByDataType(node.DataType) * Global::deoDeviceInfo.Device_Number;

            deoRtSettingModel->insertRows(row,1,QModelIndex());
            deoRtSettingModel->setData(deoRtSettingModel->index(row,0,QModelIndex()),node.Cname);
            deoRtSettingModel->setData(deoRtSettingModel->index(row,1,QModelIndex()),latestDeoRtSettingValues[index]);

            DeviceNode checkNode = Global::getDeodorationNodeInfoByName(node.Name + "_Check");
            uint state = 0;
            if(checkNode.Name == "NULL")
            {
                state = 1;
            }
            else
            {
                state = latestDeoRtSettingValues[(checkNode.Offset - checkNode.StartAddress)
                        / Global::getLengthByDataType(node.DataType) * Global::deoDeviceInfo.Device_Number];

            }
            if(state == 0)
            {
                deoRtSettingModel->setData(deoRtSettingModel->index(row,2,QModelIndex()),false, Qt::UserRole);
                deoRtSettingModel->item(row, 2)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
                deoRtSettingModel->item(row, 2)->setCheckState(Qt::Unchecked);
            }
            else if(state == 1)
            {
                deoRtSettingModel->setData(deoRtSettingModel->index(row,2,QModelIndex()),true, Qt::UserRole);
                deoRtSettingModel->item(row, 2)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
                deoRtSettingModel->item(row, 2)->setCheckState(Qt::Checked);
            }

            row++;
        }
    }

     for(int i=0; i < latestDeoRtSettingValues.length(); i++)
     {
         deoRtSettingValues[i] = latestDeoRtSettingValues[i];
     }

    row = 0;
}
