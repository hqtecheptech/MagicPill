#include "global.h"
#include <QDomDocument>
#include <QFile>
#include <QStandardItemModel>

Global::Global()
{
    alertsModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("时间"));
    alertsModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("设备号"));
    alertsModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("事件"));
    alertsModel->setHeaderData(3,Qt::Horizontal,QStringLiteral("用户"));
    alertsModel->removeRows(0,alertsModel->rowCount(QModelIndex()),QModelIndex());
}

ServerInfo Global::readServerInfo()
{
    QFile file("sysconfig.xml");
    ServerInfo retValue;
    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        return retValue;
    }
    if(document.isNull())
    {
        return retValue;
    }

    QDomElement root = document.documentElement();
    if(root.isNull())
    {
        return retValue;
    }

    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        if(node.toElement().tagName()=="server")
        {
            qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="ip")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.IP = leafNode.toText().data();
                    qDebug() << leafNode.toText().data();
                }
                if(childNode.toElement().tagName()=="port")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.Port = leafNode.toText().data().toInt();
                    qDebug() << leafNode.toText().data();
                }
                childNode = childNode.nextSibling();
            }
            break;
        }
        node = node.nextSibling();
    }
    return retValue;
}

YhcDeviceInfo Global::readYhcDeviceInfo()
{
    YhcDeviceInfo retValue;

    QFile file("yhc_devices.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return retValue;
    }

    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        return retValue;
    }
    if(document.isNull())
    {
        return retValue;
    }

    QDomElement root = document.documentElement();
    if(root.isNull())
    {
        return retValue;
    }

    QDomElement areaSection = root.firstChildElement();
    if(areaSection.isNull())
    {
        return retValue;
    }
    retValue.Device_Number = areaSection.attribute("devicenumber").toInt();
    retValue.RunCtr_Block_Size = areaSection.attribute("runctrnumber").toInt();

    QDomNodeList areaList = areaSection.elementsByTagName("area");
    int count = areaList.count();
    for(int i=0; i<count; i++)
    {
        QDomNode dom_node = areaList.item(i);
        QDomElement element = dom_node.toElement();
        QDomNodeList child_list = element.childNodes();
        int child_count = child_list.count();
        if(element.attribute("name")=="YHC")
        {
            retValue.Yhc_Type = element.attribute("datatype");
            retValue.Yhc_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Yhc_Num = varNum;
            retValue.Yhc_Category = categoryNum;
        }
        if(element.attribute("name")=="YHC_SETTING")
        {
            retValue.Yhc_Setting_Type = element.attribute("datatype");
            retValue.Yhc_Setting_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Yhc_Setting_Num = varNum;
            retValue.Yhc_Setting_Category = categoryNum;
        }
        if(element.attribute("name")=="RUNCTR")
        {
            retValue.Runctr_Type = element.attribute("datatype");
            retValue.Runctr_Address = element.attribute("start").toInt();
            retValue.Runctr_Num = retValue.Device_Number * retValue.RunCtr_Block_Size;
        }
    }
    return retValue;
}

FanDeviceInfo Global::readFanDeviceInfo()
{
    FanDeviceInfo retValue;

    QFile file("fan_devices.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return retValue;
    }

    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        return retValue;
    }
    if(document.isNull())
    {
        return retValue;
    }

    QDomElement root = document.documentElement();
    if(root.isNull())
    {
        return retValue;
    }

    QDomElement areaSection = root.firstChildElement();
    if(areaSection.isNull())
    {
        return retValue;
    }
    retValue.Device_Number = areaSection.attribute("devicenumber").toInt();
    retValue.RunCtr_Block_Size = areaSection.attribute("runctrnumber").toInt();

    QDomNodeList areaList = areaSection.elementsByTagName("area");
    int count = areaList.count();
    for(int i=0; i<count; i++)
    {
        QDomNode dom_node = areaList.item(i);
        QDomElement element = dom_node.toElement();
        QDomNodeList child_list = element.childNodes();
        int child_count = child_list.count();
        if(element.attribute("name")=="AERATION_FAN_HZ")
        {
            retValue.Aeration_Fan_Hz_Type = element.attribute("datatype");
            retValue.Aeration_Fan_Hz_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Aeration_Fan_Hz_Num = varNum;
            retValue.Aeration_Fan_Hz_Category = categoryNum;
        }
        if(element.attribute("name")=="AERATION_FAN_HZ_SETTING")
        {
            retValue.Aeration_Fan_Hz_SETTING_Type = element.attribute("datatype");
            retValue.Aeration_Fan_Hz_SETTING_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Aeration_Fan_Hz_SETTING_Num = varNum;
            retValue.Aeration_Fan_Hz_SETTING_Category = categoryNum;
        }
        if(element.attribute("name")=="RUNCTR")
        {
            retValue.Runctr_Type = element.attribute("datatype");
            retValue.Runctr_Address = element.attribute("start").toInt();
            retValue.Runctr_Num = retValue.Device_Number * retValue.RunCtr_Block_Size;
        }
    }
    return retValue;
}

FanValveDeviceInfo Global::readFanValveDeviceInfo()
{
    FanValveDeviceInfo retValue;

    QFile file("fan_valve_devices.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return retValue;
    }

    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        return retValue;
    }
    if(document.isNull())
    {
        return retValue;
    }

    QDomElement root = document.documentElement();
    if(root.isNull())
    {
        return retValue;
    }

    QDomElement areaSection = root.firstChildElement();
    if(areaSection.isNull())
    {
        return retValue;
    }
    retValue.Device_Number = areaSection.attribute("devicenumber").toInt();
    retValue.RunCtr_Block_Size = areaSection.attribute("runctrnumber").toInt();

    QDomNodeList areaList = areaSection.elementsByTagName("area");
    int count = areaList.count();
    for(int i=0; i<count; i++)
    {
        QDomNode dom_node = areaList.item(i);
        QDomElement element = dom_node.toElement();
        QDomNodeList child_list = element.childNodes();
        int child_count = child_list.count();
        if(element.attribute("name")=="RUNCTR")
        {
            retValue.Runctr_Type = element.attribute("datatype");
            retValue.Runctr_Address = element.attribute("start").toInt();
            retValue.Runctr_Num = retValue.Device_Number * retValue.RunCtr_Block_Size;
        }
    }
    return retValue;
}

DeodorationDeviceInfo Global::readDeodorationDeviceInfo()
{
    DeodorationDeviceInfo retValue;

    QFile file("deo_devices.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return retValue;
    }

    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        return retValue;
    }
    if(document.isNull())
    {
        return retValue;
    }

    QDomElement root = document.documentElement();
    if(root.isNull())
    {
        return retValue;
    }

    QDomElement areaSection = root.firstChildElement();
    if(areaSection.isNull())
    {
        return retValue;
    }
    retValue.Device_Number = areaSection.attribute("devicenumber").toInt();
    retValue.RunCtr_Block_Size = areaSection.attribute("runctrnumber").toInt();

    QDomNodeList areaList = areaSection.elementsByTagName("area");
    int count = areaList.count();
    for(int i=0; i<count; i++)
    {
        QDomNode dom_node = areaList.item(i);
        QDomElement element = dom_node.toElement();
        QDomNodeList child_list = element.childNodes();
        int child_count = child_list.count();
        if(element.attribute("name")=="DEODORATION")
        {
            retValue.Deodoration_Type = element.attribute("datatype");
            retValue.Deodoration_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Deodoration_Num = varNum;
            retValue.Deodoration_Category = categoryNum;
        }
        if(element.attribute("name")=="DEODORATION_SETTING")
        {
            retValue.Deo_Setting_Type = element.attribute("datatype");
            retValue.Deo_Setting_Address = element.attribute("start").toInt();
            int varNum = 0;
            int displaySettingNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                if(!child_list.item(j).toElement().attribute("name").contains("check", Qt::CaseInsensitive))
                {
                    qDebug() << "Name = " << child_list.item(j).toElement().attribute("name");
                    displaySettingNum += 1;
                }
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Deo_Setting_Num = varNum;
            retValue.Deo_Setting_Display_Num = displaySettingNum;
            retValue.Deo_Setting_Category = categoryNum;
        }
        if(element.attribute("name")=="RUNCTR")
        {
            retValue.Runctr_Type = element.attribute("datatype");
            retValue.Runctr_Address = element.attribute("start").toInt();
            retValue.Runctr_Num = retValue.Device_Number * retValue.RunCtr_Block_Size;
        }
    }
    return retValue;
}

FermenationDeviceInfo Global::readFermenationDeviceInfo()
{
    FermenationDeviceInfo retValue;

    QFile file("devices.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return retValue;
    }

    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        return retValue;
    }
    if(document.isNull())
    {
        return retValue;
    }

    QDomElement root = document.documentElement();
    if(root.isNull())
    {
        return retValue;
    }

    QDomElement areaSection = root.firstChildElement();
    if(areaSection.isNull())
    {
        return retValue;
    }
    retValue.Device_Number = areaSection.attribute("devicenumber").toInt();
    retValue.RunCtr_Block_Size = areaSection.attribute("runctrnumber").toInt();

    QDomNodeList areaList = areaSection.elementsByTagName("area");
    int count = areaList.count();
    for(int i=0; i<count; i++)
    {

        QDomNode dom_node = areaList.item(i);
        QDomElement element = dom_node.toElement();
        QDomNodeList child_list = element.childNodes();
        int child_count = child_list.count();
        if(element.attribute("name")=="FERMENTATION")
        {
            retValue.Fermentation_Type = element.attribute("datatype");
            retValue.Fermentation_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Fermentation_Num = varNum;
            retValue.Fermentation_Category = categoryNum;
        }
        if(element.attribute("name")=="TEMPTURE_SETTING")
        {
            retValue.Tempture_Setting_Type = element.attribute("datatype");
            retValue.Tempture_Setting_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Tempture_Setting_Num = varNum;
            retValue.Tempture_Setting_Category = categoryNum;
        }
        if(element.attribute("name")=="ENVIRONMENT")
        {
            retValue.Enviroment_Type = element.attribute("datatype");
            retValue.Enviroment_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Enviroment_Num = varNum;
            retValue.Enviroment_Category = categoryNum;
        }
        if(element.attribute("name")=="RUNNING_TIME")
        {
            retValue.Runtime_Type = element.attribute("datatype");
            retValue.Runtime_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Runtime_Num = varNum;
            retValue.Runtime_Category = categoryNum;
        }
        if(element.attribute("name")=="FER_TIME_SETTING")
        {
            retValue.Fermentation_Time_Setting_Type = element.attribute("datatype");
            retValue.Fermentation_Time_Setting_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Fermentation_Time_Setting_Num = varNum;
            retValue.Fermentation_Time_Setting_Category = categoryNum;
        }
        if(element.attribute("name")=="AERATION_RUNTIME_SETTING")
        {
            retValue.Aeration_Runtime_Setting_Type = element.attribute("datatype");
            retValue.Aeration_Runtime_Setting_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Aeration_Runtime_Setting_Num = varNum;
            retValue.Aeration_Runtime_Setting_Category = categoryNum;
        }
        if(element.attribute("name")=="AERATION_HZ_SETTING")
        {
            retValue.Aeration_Hz_Setting_Type = element.attribute("datatype");
            retValue.Aeration_Hz_Setting_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Aeration_Hz_Setting_Num = varNum;
            retValue.Aeration_Hz_Setting_Category = categoryNum;
        }
        if(element.attribute("name")=="FER_START_END_TIME")
        {
            retValue.Fermentation_DateTime_Type = element.attribute("datatype");
            retValue.Fermentation_DateTime_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Fermentation_DateTime_Num = varNum;
            retValue.Fermentation_DateTime_Category = categoryNum;
        }
        if(element.attribute("name")=="TANK_STATE")
        {
            retValue.Tank_State_Type = element.attribute("datatype");
            retValue.Tank_State_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Tank_State_Num = varNum;
            retValue.Tank_State_Category = categoryNum;
        }
        if(element.attribute("name")=="RUNCTR")
        {
            retValue.Runctr_Type = element.attribute("datatype");
            retValue.Runctr_Address = element.attribute("start").toInt();
            retValue.Runctr_Num = retValue.Device_Number * retValue.RunCtr_Block_Size;
        }
    }
    return retValue;
}

QVector<DeviceNode> readRunctrDeviceNodes(QString filename)
{
    QFile file(filename);
    //QFile file("devices.xml");
    //QFile file(":/config/devices.xml");

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return QVector<DeviceNode>(0);
    }

    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        return QVector<DeviceNode>(0);
    }
    if(document.isNull())
    {
        return QVector<DeviceNode>(0);
    }

    QDomElement root = document.documentElement();
    if(root.isNull())
    {
        return QVector<DeviceNode>(0);
    }

    QDomElement areaSection = root.firstChildElement();
    if(areaSection.isNull())
    {
        return QVector<DeviceNode>(0);
    }

    int size = areaSection.attribute("runctrnumber").toInt();
    QVector<DeviceNode> nodes = QVector<DeviceNode>(size);

    QDomNodeList areaList = areaSection.elementsByTagName("area");
    int count = areaList.count();
    for(int i=0; i<count; i++)
    {
        QDomNode dom_node = areaList.item(i);
        QDomElement element = dom_node.toElement();
        int startAddress = element.attribute("start").toInt();
        if(element.attribute("name")=="RUNCTR")
        {
            QDomNodeList child_list = element.childNodes();
            int child_count = child_list.count();
            for(int j=0; j<child_count; j++)
            {
                DeviceNode node;
                node.DataType = element.attribute("datatype");
                node.Id = child_list.item(j).toElement().attribute("id").toInt();
                node.Name = child_list.item(j).toElement().attribute("name");
                node.Cname = child_list.item(j).toElement().attribute("cname");
                node.StartAddress = startAddress;
                node.Length = child_list.item(j).toElement().attribute("length").toInt();
                node.Diff = child_list.item(j).toElement().attribute("diff").toFloat();
                node.Offset = j;
                //node.Offset = child_list.item(j).toElement().attribute("offset").toUShort();
                node.Alert0 = child_list.item(j).toElement().attribute("alert0");
                node.Alert1 = child_list.item(j).toElement().attribute("alert1");
                nodes[j] = node;
            }
            return nodes;
        }
    }
    return nodes;
}

QVector<DeviceNode> readDeviceNodes(QString filename)
{
    QFile file(filename);
    //QFile file("devices.xml");
    //QFile file(":/config/devices.xml");

    QVector<DeviceNode> nodes;

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return nodes;
    }

    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        return nodes;
    }
    if(document.isNull())
    {
        return nodes;
    }

    QDomElement root = document.documentElement();
    if(root.isNull())
    {
        return nodes;
    }

    QDomElement areaSection = root.firstChildElement();
    if(areaSection.isNull())
    {
        return nodes;
    }

    QDomNodeList areaList = areaSection.elementsByTagName("area");
    int count = areaList.count();
    for(int i=0; i<count; i++)
    {
        QDomNode dom_node = areaList.item(i);
        QDomElement element = dom_node.toElement();
        QString dataType = element.attribute("datatype");
        ushort len = Global::getLengthByDataType(dataType);
        QDomNodeList child_list = element.childNodes();
        int child_count = child_list.count();
        ushort currentOffset = element.attribute("start").toUShort();
        for(int j=0; j<child_count; j++)
        {
            DeviceNode node;
            node.DataType = dataType;
            node.Id = child_list.item(j).toElement().attribute("id").toInt();
            node.Name = child_list.item(j).toElement().attribute("name");
            node.Cname = child_list.item(j).toElement().attribute("cname");
            node.Category = child_list.item(j).toElement().attribute("category");
            node.StartAddress = element.attribute("start").toInt();
            node.Length = child_list.item(j).toElement().attribute("length").toInt();
            node.Diff = child_list.item(j).toElement().attribute("diff").toFloat();
            if(node.DataType == "x0")
            {
                node.Offset = j;
                qDebug() << "j = " << j;
            }
            else
            {
                node.Offset = currentOffset;
                currentOffset += node.Length * len;
            }
            //node.Offset = child_list.item(j).toElement().attribute("offset").toUShort();
            nodes.append(node);
        }
    }
    return nodes;
}

QVector<DeviceGroupInfo> readDeviceGroupInfo(QString filename)
{
    QVector<DeviceGroupInfo> nodes;

    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return nodes;
    }

    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        return nodes;
    }
    if(document.isNull())
    {
        return nodes;
    }

    QDomElement root = document.documentElement();
    if(root.isNull())
    {
        return nodes;
    }

    QDomNodeList groupList = root.elementsByTagName("group");
    int count = groupList.count();
    for(int i=0; i<count; i++)
    {
        QDomNode dom_node = groupList.item(i);
        QDomElement element = dom_node.toElement();
        QDomNodeList child_list = element.childNodes();
        int child_count = child_list.count();
        for(int j=0; j<child_count; j++)
        {
            DeviceGroupInfo node;
            node.deviceId = child_list.item(j).toElement().attribute("deviceid").toInt();;
            node.deviceNumber = element.attribute("devicenumber").toInt();
            node.groupId = element.attribute("id").toInt();
            node.startIndex = child_list.item(j).toElement().attribute("index").toInt();
            node.offset = child_list.item(j).toElement().attribute("offset").toInt();
            nodes.append(node);
        }
    }
    return nodes;
}

DeviceNode Global::getFermenationNodeInfoByName(QString name)
{
    for(int i=0; i<ferDeviceNodes.length();i++)
    {
        if(ferDeviceNodes.at(i).Name==name)
        {
            return ferDeviceNodes.at(i);
        }
    }
}

DeviceNode Global::getDeodorationNodeInfoByName(QString name)
{
    DeviceNode node;
    for(int i=0; i<deoDeviceNodes.length();i++)
    {
        if(deoDeviceNodes.at(i).Name==name)
        {
            return deoDeviceNodes.at(i);
        }
    }
    return node;
}

DeviceNode Global::getFanValveNodeInfoByName(QString name)
{
    for(int i=0; i<fanValveDeviceNodes.length();i++)
    {
        if(fanValveDeviceNodes.at(i).Name==name)
        {
            return fanValveDeviceNodes.at(i);
        }
    }
}

DeviceNode Global::getFanNodeInfoByName(QString name)
{
    for(int i=0; i<fanDeviceNodes.length();i++)
    {
        if(fanDeviceNodes.at(i).Name==name)
        {
            return fanDeviceNodes.at(i);
        }
    }
}

DeviceNode Global::getYhcNodeInfoByName(QString name)
{
    for(int i=0; i<yhcDeviceNodes.length();i++)
    {
        if(yhcDeviceNodes.at(i).Name==name)
        {
            return yhcDeviceNodes.at(i);
        }
    }
}

DeviceNode Global::getFermenationNodeInfoByCname(QString cname)
{
    for(int i=0; i<ferDeviceNodes.length();i++)
    {
        if(ferDeviceNodes.at(i).Cname==cname)
        {
            return ferDeviceNodes.at(i);
        }
    }
}

DeviceNode Global::getDeodorationNodeInfoByCname(QString cname)
{
    for(int i=0; i<deoDeviceNodes.length();i++)
    {
        if(deoDeviceNodes.at(i).Cname==cname)
        {
            return deoDeviceNodes.at(i);
        }
    }
}

DeviceNode Global::getFanValveNodeInfoByCname(QString cname)
{
    for(int i=0; i<fanValveDeviceNodes.length();i++)
    {
        if(fanValveDeviceNodes.at(i).Cname==cname)
        {
            return fanValveDeviceNodes.at(i);
        }
    }
}

DeviceNode Global::getFanNodeInfoByCname(QString cname)
{
    for(int i=0; i<fanDeviceNodes.length();i++)
    {
        if(fanDeviceNodes.at(i).Cname==cname)
        {
            return fanDeviceNodes.at(i);
        }
    }
}

DeviceNode Global::getYhcNodeInfoByCname(QString cname)
{
    for(int i=0; i<yhcDeviceNodes.length();i++)
    {
        if(yhcDeviceNodes.at(i).Cname==cname)
        {
            return yhcDeviceNodes.at(i);
        }
    }
}

QString Global::getNodeNameByAddress(ushort address, QVector<DeviceNode> deviceNodes)
{
    QString nodeName = deviceNodes.at(0).Name;
    foreach (DeviceNode node, deviceNodes) {
        if(node.Offset == address)
        {
            return node.Name;
        }
        else if(node.Offset < address)
        {
            nodeName = node.Name;
        }
        else if(node.Offset > address)
        {
            return nodeName;
        }
    }
}

QString Global::getNodeCnameByAddress(ushort address, QVector<DeviceNode> deviceNodes)
{
    QString nodeName = deviceNodes.at(0).Cname;
    foreach (DeviceNode node, deviceNodes) {
        if(node.Offset == address)
        {
            return node.Cname;
        }
        else if(node.Offset < address)
        {
            nodeName = node.Cname;
        }

        else if(node.Offset > address)
        {
            return nodeName;
        }
    }
}

int Global::getFerDeviceIndexByAddress(ushort address)
{
    int index = -1;
    foreach (DeviceNode node, ferDeviceNodes) {
        uint dataLen = getLengthByDataType(node.DataType);
        if(address >= node.Offset &&
                address < node.Offset + (ferDeviceInfo.Device_Number * dataLen))
        {
            index = (address - node.Offset) / dataLen;
        }
    }
    return index;
}

int Global::getDeoDeviceIndexByAddress(ushort address)
{
    int index = -1;
    foreach (DeviceNode node, deoDeviceNodes) {
        uint dataLen = getLengthByDataType(node.DataType);
        if(address >= node.Offset &&
                address < node.Offset + (deoDeviceInfo.Device_Number * dataLen))
        {
            index = (address - node.Offset) / dataLen;
        }
    }
    return index;
}

int Global::getYhcDeviceIndexByAddress(ushort address)
{
    int index = -1;
    if(yhcDeviceInfo.Device_Number == 1)
    {
        index = 0;
    }
    else
    {
        foreach (DeviceNode node, yhcDeviceNodes)
        {
            if(address >= node.Offset &&
                        address < node.Offset + yhcDeviceInfo.Device_Number)
            {
                index = address - node.Offset;
            }
        }
    }
    return index;
}

int Global::getFerDeviceIndexByRunctrAddress(float address)
{
    uint blockSize = ferDeviceInfo.RunCtr_Block_Size / 8;

    int index = -1;
    for(uint i=0; i < ferDeviceInfo.Device_Number; i++)
    {
        if((address >= ferDeviceInfo.Runctr_Address + i * blockSize) &&
                address < ferDeviceInfo.Runctr_Address + (i + 1) * blockSize)
        {
            index = i;
        }
    }
    return index;
}

int Global::getDeoDeviceIndexByRunctrAddress(float address)
{
    uint blockSize = deoDeviceInfo.RunCtr_Block_Size / 8;

    int index = -1;
    for(uint i=0; i < deoDeviceInfo.Device_Number; i++)
    {
        if((address >= deoDeviceInfo.Runctr_Address + i * blockSize) &&
                address < deoDeviceInfo.Runctr_Address + (i + 1) * blockSize)
        {
            index = i;
        }
    }
    return index;
}

int Global::getYhcDeviceIndexByRunctrAddress(float address)
{
    int index = -1;
    if(yhcDeviceInfo.Device_Number == 1)
    {
        index = 0;
    }
    else
    {
        uint blockSize = yhcDeviceInfo.RunCtr_Block_Size  / 8;

        int index = -1;
        for(uint i=0; i < yhcDeviceInfo.Device_Number; i++)
        {
            if((address >= yhcDeviceInfo.Runctr_Address + i * blockSize) &&
                    address < yhcDeviceInfo.Runctr_Address + (i + 1) * blockSize)
            {
                index = i;
            }
        }
    }
    return index;
}

DeviceGroupInfo Global::getDeoDeviceGroupInfo(int index)
{
    foreach(DeviceGroupInfo info, deoDeviceGroupInfos)
    {
        if(info.startIndex >= index && index < (info.startIndex + info.deviceNumber))
        {
            return info;
        }
    }
}

DeviceGroupInfo Global::getFanValveDeviceGroupInfo(int index)
{
    foreach(DeviceGroupInfo info, fanValveDeviceGroupInfos)
    {
        if(index >= info.startIndex && index < (info.startIndex + info.deviceNumber))
        {
            return info;
        }
    }
}

DeviceGroupInfo Global::getFanDeviceGroupInfo(int index)
{
    foreach(DeviceGroupInfo info, fanDeviceGroupInfos)
    {
        if(index >= info.startIndex && index < (info.startIndex + info.deviceNumber))
        {
            return info;
        }
    }
}

DeviceGroupInfo Global::getFerDeviceGroupInfo(int index)
{
    foreach(DeviceGroupInfo info, ferDeviceGroupInfos)
    {
        if(index >= info.startIndex && index < (info.startIndex + info.deviceNumber))
        {
            return info;
        }
    }
}

DeviceGroupInfo Global::getYhcDeviceGroupInfo(int index)
{
    foreach(DeviceGroupInfo info, yhcDeviceGroupInfos)
    {
        if(index >= info.startIndex && index < (info.startIndex + info.deviceNumber))
        {
            return info;
        }
    }
}

bool Global::getFerRunctrValueByName(int deviceIndex, QString name, QMap<float, QString> dataMap)
{
    DeviceGroupInfo groupInfo = getFerDeviceGroupInfo(deviceIndex);
    int startAddrss = ferDeviceInfo.Runctr_Address +
            ferDeviceInfo.RunCtr_Block_Size / 8 * (deviceIndex - groupInfo.startIndex);

    DeviceNode deviceNode = getFermenationNodeInfoByName(name);
    int offset = deviceNode.Offset;
    uint step = offset / 8;
    uint temp = offset % 8;
    float index = float(temp) / 10;
    float dictAddress = index + startAddrss + step;
    QVariant tempValue = dataMap[dictAddress];
    return tempValue.toBool();
}

bool Global::getDeoRunctrValueByName(int deviceIndex, QString name, QMap<float, QString> dataMap)
{
    DeviceGroupInfo groupInfo = getDeoDeviceGroupInfo(deviceIndex);
    int startAddrss = deoDeviceInfo.Runctr_Address +
            deoDeviceInfo.RunCtr_Block_Size / 8 * (deviceIndex - groupInfo.startIndex);

    DeviceNode deviceNode = getDeodorationNodeInfoByName(name);
    int offset = deviceNode.Offset;
    uint step = offset / 8;
    uint temp = offset % 8;
    float index = float(temp) / 10;
    float dictAddress = index + startAddrss + step;
    QVariant tempValue = dataMap[dictAddress];
    return tempValue.toBool();
}

bool Global::getYhcRunctrValueByName(int deviceIndex, QString name, QMap<float, QString> dataMap)
{
    DeviceGroupInfo groupInfo = getYhcDeviceGroupInfo(deviceIndex);
    int startAddrss = yhcDeviceInfo.Runctr_Address +
            yhcDeviceInfo.RunCtr_Block_Size / 8 * (deviceIndex - groupInfo.startIndex);

    DeviceNode deviceNode = getYhcNodeInfoByName(name);
    int offset = deviceNode.Offset;
    uint step = offset / 8;
    uint temp = offset % 8;
    float index = float(temp) / 10;
    float dictAddress = index + startAddrss + step;
    QVariant tempValue = dataMap[dictAddress];
    return tempValue.toBool();
}

int Global::getFerDeviceStartIndex(int deviceId, int deviceGroup)
{
    foreach(DeviceGroupInfo info, ferDeviceGroupInfos)
    {
        if(info.deviceId == deviceId && info.groupId == deviceGroup)
        {
            return info.startIndex;
        }
    }
    return -1;
}

int Global::getFanDeviceStartIndex(int deviceId, int deviceGroup)
{
    foreach(DeviceGroupInfo info, fanDeviceGroupInfos)
    {
        if(info.deviceId == deviceId && info.groupId == deviceGroup)
        {
            return info.startIndex;
        }
    }
    return -1;
}

int Global::getFanValveDeviceStartIndex(int deviceId, int deviceGroup)
{
    foreach(DeviceGroupInfo info, fanValveDeviceGroupInfos)
    {
        if(info.deviceId == deviceId && info.groupId == deviceGroup)
        {
            return info.startIndex;
        }
    }
    return -1;
}

int Global::getDeoDeviceStartIndex(int deviceId, int deviceGroup)
{
    foreach(DeviceGroupInfo info, deoDeviceGroupInfos)
    {
        if(info.deviceId == deviceId && info.groupId == deviceGroup)
        {
            return info.startIndex;
        }
    }
    return -1;
}

int Global::getYhcDeviceStartIndex(int deviceId, int deviceGroup)
{
    foreach(DeviceGroupInfo info, yhcDeviceGroupInfos)
    {
        if(info.deviceId == deviceId && info.groupId == deviceGroup)
        {
            return info.startIndex;
        }
    }
    return -1;
}

uint Global::getLengthByDataType(QString dataType)
{
    uint length = 0;

    if(dataType == "r" || dataType == "dw" || dataType == "di")
    {
        return 4;
    }
    else if(dataType == "w" || dataType == "i")
    {
        return 2;
    }
    else if(dataType == "dt")
    {
        return 8;
    }
    else if(dataType == "dtl")
    {
        return 12;
    }

    return length;
}

ServerInfo Global::serverInfo = readServerInfo();

FermenationDeviceInfo Global::ferDeviceInfo = readFermenationDeviceInfo();

DeodorationDeviceInfo Global::deoDeviceInfo = readDeodorationDeviceInfo();

FanValveDeviceInfo Global::fanValveDeviceInfo = readFanValveDeviceInfo();

FanDeviceInfo Global::fanDeviceInfo = readFanDeviceInfo();

YhcDeviceInfo Global::yhcDeviceInfo = readYhcDeviceInfo();

QVector<DeviceNode> Global::ferDeviceNodes = readDeviceNodes("devices.xml");

QVector<DeviceNode> Global::deoDeviceNodes = readDeviceNodes("deo_devices.xml");

QVector<DeviceNode> Global::fanValveDeviceNodes = readDeviceNodes("fan_valve_devices.xml");

QVector<DeviceNode> Global::fanDeviceNodes = readDeviceNodes("fan_devices.xml");

QVector<DeviceNode> Global::yhcDeviceNodes = readDeviceNodes("yhc_devices.xml");

QVector<DeviceNode> Global::ferRunCtrDeviceNodes = readRunctrDeviceNodes("devices.xml");

QVector<DeviceNode> Global::deoRunCtrDeviceNodes = readRunctrDeviceNodes("deo_devices.xml");

QVector<DeviceNode> Global::fanValveRunCtrDeviceNodes = readRunctrDeviceNodes("fan_valve_devices.xml");

QVector<DeviceNode> Global::fanRunCtrDeviceNodes = readRunctrDeviceNodes("fan_devices.xml");

QVector<DeviceNode> Global::yhcRunCtrDeviceNodes = readRunctrDeviceNodes("yhc_devices.xml");

QVector<DeviceGroupInfo> Global::deoDeviceGroupInfos = readDeviceGroupInfo("deo_device_group.xml");

QVector<DeviceGroupInfo> Global::fanValveDeviceGroupInfos = readDeviceGroupInfo("fan_Valve_device_group.xml");

QVector<DeviceGroupInfo> Global::fanDeviceGroupInfos = readDeviceGroupInfo("fan_device_group.xml");

QVector<DeviceGroupInfo> Global::ferDeviceGroupInfos = readDeviceGroupInfo("fer_device_group.xml");

QVector<DeviceGroupInfo> Global::yhcDeviceGroupInfos = readDeviceGroupInfo("./yhc_devices_group.xml");

QStandardItemModel* Global::alertsModel = new QStandardItemModel(0,4);

QStandardItemModel* Global::simpleAlertsModel = new QStandardItemModel(0,1);

QStandardItemModel* Global::loginHistoryModel = new QStandardItemModel(0,1);

QMap<float,QString> Global::currentFermenationDataMap;

QMap<float,QString> Global::currentDeodorationDataMap;

QMap<float,QString> Global::currentFanGroupDataMap;

QMap<float,QString> Global::currentFanValveGroupDataMap;

QMap<float,QString> Global::currentYhcDataMap;

int Global::alertIndex = 0;
