#include "global.h"
#include <QDomDocument>
#include <QFile>
#include <QDir>
#include <QVariant>
#include <math.h>
#include <QProcess>

#include "data.h"

Global::Global()
{

}

bool Global::modifyMixNodeValueRange()
{
    QFile file("mix_devices.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }

    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        file.close();
        return true;
    }

    file.close();

    if(document.isNull())
    {
        return true;
    }

    QDomElement root = document.documentElement();
    if(root.isNull())
    {
        return true;
    }

    QDomElement areaSection = root.firstChildElement();
    if(areaSection.isNull())
    {
        return true;
    }

    if(mixDeviceNodes.length() > 0)
    {
        int i = 0;
        QDomNodeList nodeList = areaSection.elementsByTagName("node");
        for(int j = 0; j < nodeList.length(); j++)
        {
            QDomNode node = nodeList.at(j);
            QDomElement e = node.toElement();
            if(e.attribute("name")==mixDeviceNodes.at(i).Name)
            {
                e.setAttribute("lr", mixDeviceNodes.at(i).leftRange);
                e.setAttribute("rr", mixDeviceNodes.at(i).rightRange);
                i++;
            }
        }
    }

    if (!file.open(QIODevice::WriteOnly | QFile::Truncate))
    {
        return false;
    }
    QTextStream xmlWrite(&file);
    document.save(xmlWrite,4);
    file.close();
    return true;
}

void Global::testMv()
{
    DeviceNode* head = Global::mixDeviceNodes.data();
    head = head + 5;
    head->leftRange = 3;
    head->rightRange = 4;
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
            //qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                //qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="ip")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.IP = leafNode.toText().data();
                    //qDebug() << leafNode.toText().data();
                }
                if(childNode.toElement().tagName()=="port")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.Port = leafNode.toText().data().toInt();
                    //qDebug() << leafNode.toText().data();
                }
                childNode = childNode.nextSibling();
            }
            break;
        }
        node = node.nextSibling();
    }
    return retValue;
}

int Global::readFerGroupShowing()
{
    QFile file("sysconfig.xml");
    int retValue = -1;
    QDomDocument document;
    QString error;
    int row = 0, column = 0;
    if(!document.setContent(&file, false, &error, &row, &column))
    {
        return -1;
    }
    if(document.isNull())
    {
        return -1;
    }

    QDomElement root = document.documentElement();
    if(root.isNull())
    {
        return -1;
    }

    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        if(node.toElement().tagName()=="fer")
        {
            qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="group")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue = leafNode.toText().data().toInt();
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

SystemConfig Global::readSystemConfig()
{
    //qDebug() << "Reading readSystemConfig ...!";

    QFile file("controller_sysconfig.xml");
    SystemConfig retValue;
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

    //qDebug() << "Reading readSystemConfig successful!";

    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        if(node.toElement().tagName()=="server")
        {
            //qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                //qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="ip")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.IP = leafNode.toText().data();
                    //qDebug() << retValue.IP;
                }
                if(childNode.toElement().tagName()=="port")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.Port = leafNode.toText().data().toInt();
                    //qDebug() << retValue.Port;
                }
                childNode = childNode.nextSibling();
            }
        }
        else if(node.toElement().tagName()=="data_sem")
        {
            //qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                //qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="pathname")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.dataSemPath = leafNode.toText().data();
                    //qDebug() << retValue.dataSemPath;
                }
                if(childNode.toElement().tagName()=="proj_id")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.dataSemKey = leafNode.toText().data().toInt();
                    //qDebug() << retValue.dataSemKey;
                }
                childNode = childNode.nextSibling();
            }
        }
        else if(node.toElement().tagName()=="data_msg")
        {
            //qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                //qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="pathname")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.dataMsgPath = leafNode.toText().data();
                    //qDebug() << retValue.dataMsgPath;
                }
                if(childNode.toElement().tagName()=="proj_id")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.dataMsgKey = leafNode.toText().data().toInt();
                    //qDebug() << retValue.dataMsgKey;
                }
                childNode = childNode.nextSibling();
            }
        }
        else if(node.toElement().tagName()=="data_share")
        {
            //qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                //qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="pathname")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.dataSharePath = leafNode.toText().data();
                    //qDebug() << retValue.dataSharePath;
                }
                if(childNode.toElement().tagName()=="proj_id")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.dataShareKey = leafNode.toText().data().toInt();
                    //qDebug() << retValue.dataShareKey;
                }
                childNode = childNode.nextSibling();
            }
        }
        else if(node.toElement().tagName()=="control_share")
        {
            //qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                //qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="pathname")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.controlSharePath = leafNode.toText().data();
                    //qDebug() << retValue.controlSharePath;
                }
                if(childNode.toElement().tagName()=="proj_id")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.controlShareKey = leafNode.toText().data().toInt();
                    //qDebug() << leafNode.toText().data();
                }
                childNode = childNode.nextSibling();
            }
        }
        else if(node.toElement().tagName()=="control_sem")
        {
            //qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                //qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="pathname")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.controlSemPath = leafNode.toText().data();
                    //qDebug() << retValue.controlSemPath;
                }
                if(childNode.toElement().tagName()=="proj_id")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.controlSemKey = leafNode.toText().data().toInt();
                    //qDebug() << retValue.controlSemKey;
                }
                childNode = childNode.nextSibling();
            }
        }
        else if(node.toElement().tagName()=="control_msg")
        {
            //qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                //qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="pathname")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.controlMsgPath = leafNode.toText().data();
                    //qDebug() << retValue.controlMsgPath;
                }
                if(childNode.toElement().tagName()=="proj_id")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.controlMsgKey = leafNode.toText().data().toInt();
                    //qDebug() << retValue.controlMsgKey;
                }
                childNode = childNode.nextSibling();
            }
        }
        else if(node.toElement().tagName()=="yhc_control_share")
        {
            //qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                //qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="pathname")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.yhcControlSharePath = leafNode.toText().data();
                    //qDebug() << retValue.yhcControlSharePath;
                }
                if(childNode.toElement().tagName()=="proj_id")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.yhcControlShareKey = leafNode.toText().data().toInt();
                    //qDebug() << leafNode.toText().data();
                }
                childNode = childNode.nextSibling();
            }
        }
        else if(node.toElement().tagName()=="yhc_control_sem")
        {
            //qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                //qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="pathname")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.yhcControlSemPath = leafNode.toText().data();
                    //qDebug() << retValue.yhcControlSemPath;
                }
                if(childNode.toElement().tagName()=="proj_id")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.yhcControlSemKey = leafNode.toText().data().toInt();
                    //qDebug() << retValue.yhcControlSemKey;
                }
                childNode = childNode.nextSibling();
            }
        }
        else if(node.toElement().tagName()=="yhc_control_msg")
        {
            //qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                //qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="pathname")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.yhcControlMsgPath = leafNode.toText().data();
                    //qDebug() << retValue.yhcControlMsgPath;
                }
                if(childNode.toElement().tagName()=="proj_id")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.yhcControlMsgKey = leafNode.toText().data().toInt();
                    //qDebug() << retValue.yhcControlMsgKey;
                }
                childNode = childNode.nextSibling();
            }
        }
        else if(node.toElement().tagName()=="ferconfig")
        {
            //qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                //qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="path")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.ferconfigPath = leafNode.toText().data();
                    //qDebug() << retValue.ferconfigPath;
                }
                if(childNode.toElement().tagName()=="prefix")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.ferconfigPrefix = leafNode.toText().data();
                    //qDebug() << retValue.ferconfigPrefix;
                }
                childNode = childNode.nextSibling();
            }
        }
        else if(node.toElement().tagName()=="device")
        {
            //qDebug() << node.toElement().tagName();
            QDomNode childNode = node.firstChild();
            while(!childNode.isNull())
            {
                //qDebug() << childNode.toElement().tagName();
                if(childNode.toElement().tagName()=="type")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.deviceType = DeviceType(leafNode.toText().data().toInt());
                    //qDebug() << retValue.deviceIdType;
                }
                if(childNode.toElement().tagName()=="group")
                {
                    QDomNode leafNode = childNode.firstChild();
                    retValue.deviceGroup = leafNode.toText().data().toInt();
                    //qDebug() << retValue.group;
                }
                childNode = childNode.nextSibling();
            }
        }

        node = node.nextSibling();
    }

    //qDebug() << "dataMsgPath:" <<retValue.dataMsgPath;
    //qDebug() << "dataMsgKey:" <<retValue.dataMsgKey;

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

    areaSection.firstChildElement("dfdfd");

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

MixDeviceInfo Global::readMixDeviceInfo()
{
    MixDeviceInfo retValue;

    QFile file("mix_devices.xml");
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
        if(element.attribute("name")=="RATE_SETTING")
        {
            retValue.Rate_Setting_Type = element.attribute("datatype");
            retValue.Rate_Setting_Address = element.attribute("start").toInt();
            int varNum = 0;
            int categoryNum = 0;
            for(int j=0; j<child_count; j++)
            {
                varNum += child_list.item(j).toElement().attribute("length").toInt();
                categoryNum += 1;
            }
            retValue.Rate_Setting_Num = varNum;
            retValue.Rate_Setting_Category = categoryNum;
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
                    //qDebug() << "Name = " << child_list.item(j).toElement().attribute("name");
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
    //QVector<DeviceNode> nodes = QVector<DeviceNode>();
    QList<DeviceNode> nodes;

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
                node.Priority = child_list.item(j).toElement().attribute("priority").toInt();
                //nodes[j] = node;
                nodes.append(node);
            }
            return nodes.toVector();
        }
    }
    return nodes.toVector();
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
            node.Alert0 = child_list.item(j).toElement().attribute("alert0");
            node.Alert1 = child_list.item(j).toElement().attribute("alert1");
            node.Priority = child_list.item(j).toElement().attribute("priority").toInt();
            node.leftRange = child_list.item(j).toElement().attribute("lr").toFloat();
            node.rightRange = child_list.item(j).toElement().attribute("rr").toFloat();
            if(node.DataType == "x0")
            {
                node.Offset = j;
                //qDebug() << "j = " << j;
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
    QString cp = QDir::currentPath();
    //qDebug()<<cp;

    QVector<DeviceGroupInfo> nodes;

    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return nodes;
    }

    //qDebug() << "Read " << filename << "succesfully!";

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
    //QVector<DeviceNode> temp = yhcDeviceNodes;
    for(int i=0; i<yhcDeviceNodes.length();i++)
    {
        if(yhcDeviceNodes.at(i).Name==name)
        {
            return yhcDeviceNodes.at(i);
        }
    }
}

DeviceNode Global::getMixNodeInfoByName(QString name)
{
    for(int i=0; i<mixDeviceNodes.length();i++)
    {
        if(mixDeviceNodes.at(i).Name==name)
        {
            return mixDeviceNodes.at(i);
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

DeviceNode Global::getMixNodeInfoByCname(QString cname)
{
    for(int i=0; i<mixDeviceNodes.length();i++)
    {
        if(mixDeviceNodes.at(i).Cname==cname)
        {
            return mixDeviceNodes.at(i);
        }
    }
}

DeviceNode Global::getYhcNodeInfoByRunctrAddress(float address)
{
    uint blockSize = yhcDeviceInfo.RunCtr_Block_Size  / 8;

    int temp = (int)floor(address);
    int blockOffset = (temp - yhcDeviceInfo.Runctr_Address) % blockSize;
    int offset = blockOffset * 8 + (int)(address * 10.0 - temp * 10.0 + 0.5);

    for(int i=0; i<yhcDeviceNodes.length();i++)
    {
        if(yhcDeviceNodes.at(i).DataType == "x0" && yhcDeviceNodes.at(i).Offset == offset)
        {
            return yhcDeviceNodes.at(i);
        }
    }
}

DeviceNode Global::getMixNodeInfoByRunctrAddress(float address)
{
    uint blockSize = mixDeviceInfo.RunCtr_Block_Size  / 8;
    DeviceNode node;

    int temp = (int)floor(address);
    int blockOffset = (temp - mixDeviceInfo.Runctr_Address) % blockSize;
    int offset = blockOffset * 8 + (int)(address * 10.0 - temp * 10.0 + 0.5);

    for(int i=0; i<mixDeviceNodes.length();i++)
    {
        if(mixDeviceNodes.at(i).DataType == "x0" && mixDeviceNodes.at(i).Offset == offset)
        {
            node = mixDeviceNodes.at(i);
            return node;
        }
    }

    return node;
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

int Global::getMixDeviceIndexByAddress(ushort address)
{
    int index = -1;
    if(mixDeviceInfo.Device_Number == 1)
    {
        index = 0;
    }
    else
    {
        foreach (DeviceNode node, mixDeviceNodes)
        {
            if(address >= node.Offset &&
                        address < node.Offset + mixDeviceInfo.Device_Number)
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

int Global::getMixDeviceIndexByRunctrAddress(float address)
{
    int index = -1;
    if(mixDeviceInfo.Device_Number == 1)
    {
        index = 0;
    }
    else
    {
        uint blockSize = mixDeviceInfo.RunCtr_Block_Size  / 8;

        int index = -1;
        for(uint i=0; i < mixDeviceInfo.Device_Number; i++)
        {
            if((address >= mixDeviceInfo.Runctr_Address + i * blockSize) &&
                    address < mixDeviceInfo.Runctr_Address + (i + 1) * blockSize)
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

DeviceGroupInfo Global::getMixDeviceGroupInfo(int index)
{
    foreach(DeviceGroupInfo info, mixDeviceGroupInfos)
    {
        if(index >= info.startIndex && index < (info.startIndex + info.deviceNumber))
        {
            return info;
        }
    }
}

DeviceGroupInfo Global::getFerDeviceGroupInfoByGroupId(int groupId)
{
    foreach(DeviceGroupInfo info, ferDeviceGroupInfos)
    {
        if(groupId == info.groupId)
        {
            return info;
        }
    }
}

DeviceGroupInfo Global::getYhcDeviceGroupInfoByGroupId(int groupId)
{
    foreach(DeviceGroupInfo info, yhcDeviceGroupInfos)
    {
        if(groupId == info.groupId)
        {
            return info;
        }
    }
}

DeviceGroupInfo Global::getMixDeviceGroupInfoByGroupId(int groupId)
{
    foreach(DeviceGroupInfo info, mixDeviceGroupInfos)
    {
        if(groupId == info.groupId)
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
    QVariant tempValue = dataMap.value(dictAddress);
    return tempValue.toBool();
}

bool Global::getMixRunctrValueByName(int deviceIndex, QString name, QMap<float, QString> dataMap)
{
    DeviceGroupInfo groupInfo = getMixDeviceGroupInfo(deviceIndex);
    int startAddrss = mixDeviceInfo.Runctr_Address +
            mixDeviceInfo.RunCtr_Block_Size / 8 * (deviceIndex - groupInfo.startIndex);

    DeviceNode deviceNode = getMixNodeInfoByName(name);
    int offset = deviceNode.Offset;
    uint step = offset / 8;
    uint temp = offset % 8;
    float index = float(temp) / 10;
    float dictAddress = index + startAddrss + step;
    QVariant tempValue = dataMap.value(dictAddress);
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

int Global::getMixDeviceStartIndex(int deviceId, int deviceGroup)
{
    foreach(DeviceGroupInfo info, mixDeviceGroupInfos)
    {
        if(info.deviceId == deviceId && info.groupId == deviceGroup)
        {
            return info.startIndex;
        }
    }
    return -1;
}

int Global::getDataStartByType(QString type)
{
    if(type == "r")
    {
        return 0;
    }
    else if(type == "di")
    {
        return getDataStartByType("r") + DB_FLOAT_LEN * getLengthByDataType("r");
    }
    else if(type == "dw")
    {
        return getDataStartByType("di") + DB_INT_LEN * getLengthByDataType("di");
    }
    else if(type == "w")
    {
        return getDataStartByType("dw") + DB_UINT32_LEN * getLengthByDataType("dw");
    }
    else if(type == "x0")
    {
        return getDataStartByType("w") + DB_UINT16_LEN * getLengthByDataType("w");
    }
    else
    {
        return -1;
    }
}

int Global::convertAddressToIndex(float address, QString type)
{
    int start = getDataStartByType(type);
    if(type != "x0")
    {
        return ((int)address - start) / (int)getLengthByDataType(type);
    }
    else
    {
        int temp = (int)floor(address);
        int offset = (int)(address * 10.0 - temp * 10.0 + 0.5);
        return (temp - start) * 8 + offset;
    }
}

float Global::convertIndexToAddress(int index, QString type)
{
    if(type != "x0")
    {
        int start = getDataStartByType(type);
        return start + index * getLengthByDataType(type);
    }
    else
    {
        int address;
        int startAddress = getDataStartByType("x0");

        int step = index / 8;
        address = startAddress + step;
        int offset = index % 8;

        return (float)offset / 10 + address;
    }
}

float Global::getRunctrAddressByIndex(int index)
{
    int address;
    int startAddress = getDataStartByType("x0");;

    int step = index / 8;
    address = startAddress + step;
    int offset = index % 8;

    return (float)offset / 10 + address;
}

int Global::getYhcDataIndexByName(QString name, int deviceIndex)
{
    DeviceGroupInfo info = Global::getYhcDeviceGroupInfo(deviceIndex);
    // To do: using a test name temporary.
    DeviceNode deviceNode = Global::getYhcNodeInfoByName(name);
    int offset = (info.offset + deviceIndex - info.startIndex) * Global::yhcDeviceInfo.RunCtr_Block_Size;
    return deviceNode.Offset + offset;
}

int Global::getMixDataIndexByName(QString name, int deviceIndex)
{
    DeviceGroupInfo info = Global::getMixDeviceGroupInfo(deviceIndex);
    // To do: using a test name temporary.
    DeviceNode deviceNode = Global::getMixNodeInfoByName(name);
    int offset = (info.offset + deviceIndex - info.startIndex) * Global::mixDeviceInfo.RunCtr_Block_Size;
    return deviceNode.Offset + offset;
}

int Global::getFerDataIndexByName(QString name, int deviceIndex)
{
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
    // To do: using a test name temporary.
    DeviceNode deviceNode = Global::getFermenationNodeInfoByName(name);
    int offset = (info.offset + deviceIndex - info.startIndex) * Global::ferDeviceInfo.RunCtr_Block_Size;
    return deviceNode.Offset + offset;
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

int Global::getPruPid()
{
    QProcess *cmd = new QProcess;
    QString strArg;
    strArg = "ps -e";
    cmd->start(strArg);
    cmd->waitForReadyRead();
    cmd->waitForFinished();
    QString retStr = "";
    while(cmd->canReadLine())
    {
        retStr = cmd->readLine();
        if(retStr.contains("dev_app"))
        {
            retStr.trimmed();
            QStringList strList = retStr.split(" ", QString::SkipEmptyParts);
            QByteArray retArray = strList.at(0).toLatin1();
            int pid = retArray.toInt();
            return pid;
        }
    }

    return -1;
}

ServerInfo Global::serverInfo = readServerInfo();

SystemConfig Global::systemConfig = readSystemConfig();

int Global::ferGroupShow = readFerGroupShowing();

FermenationDeviceInfo Global::ferDeviceInfo = readFermenationDeviceInfo();

DeodorationDeviceInfo Global::deoDeviceInfo = readDeodorationDeviceInfo();

FanValveDeviceInfo Global::fanValveDeviceInfo = readFanValveDeviceInfo();

FanDeviceInfo Global::fanDeviceInfo = readFanDeviceInfo();

YhcDeviceInfo Global::yhcDeviceInfo = readYhcDeviceInfo();

MixDeviceInfo Global::mixDeviceInfo = readMixDeviceInfo();

QVector<DeviceNode> Global::ferDeviceNodes = readDeviceNodes("devices.xml");

QVector<DeviceNode> Global::deoDeviceNodes = readDeviceNodes("deo_devices.xml");

QVector<DeviceNode> Global::fanValveDeviceNodes = readDeviceNodes("fan_valve_devices.xml");

QVector<DeviceNode> Global::fanDeviceNodes = readDeviceNodes("fan_devices.xml");

QVector<DeviceNode> Global::yhcDeviceNodes = readDeviceNodes("yhc_devices.xml");

QVector<DeviceNode> Global::mixDeviceNodes = readDeviceNodes("mix_devices.xml");

QVector<DeviceNode> Global::ferRunCtrDeviceNodes = readRunctrDeviceNodes("devices.xml");

QVector<DeviceNode> Global::deoRunCtrDeviceNodes = readRunctrDeviceNodes("deo_devices.xml");

QVector<DeviceNode> Global::fanValveRunCtrDeviceNodes = readRunctrDeviceNodes("fan_valve_devices.xml");

QVector<DeviceNode> Global::fanRunCtrDeviceNodes = readRunctrDeviceNodes("fan_devices.xml");

QVector<DeviceNode> Global::yhcRunCtrDeviceNodes = readRunctrDeviceNodes("yhc_devices.xml");

QVector<DeviceNode> Global::mixRunCtrDeviceNodes = readRunctrDeviceNodes("mix_devices.xml");

QVector<DeviceGroupInfo> Global::deoDeviceGroupInfos = readDeviceGroupInfo("deo_device_group.xml");

QVector<DeviceGroupInfo> Global::fanValveDeviceGroupInfos = readDeviceGroupInfo("fan_Valve_device_group.xml");

QVector<DeviceGroupInfo> Global::fanDeviceGroupInfos = readDeviceGroupInfo("fan_device_group.xml");

QVector<DeviceGroupInfo> Global::ferDeviceGroupInfos = readDeviceGroupInfo("fer_device_group.xml");

QVector<DeviceGroupInfo> Global::yhcDeviceGroupInfos = readDeviceGroupInfo("yhc_devices_group.xml");

QVector<DeviceGroupInfo> Global::mixDeviceGroupInfos = readDeviceGroupInfo("mix_device_group.xml");

QMap<float,QString> Global::currentFermenationDataMap;

QMap<float,QString> Global::currentDeodorationDataMap;

QMap<float,QString> Global::currentFanGroupDataMap;

QMap<float,QString> Global::currentFanValveGroupDataMap;

QMap<float,QString> Global::currentYhcDataMap;

QMap<float,QString> Global::currentMixDataMap;

int Global::alertIndex = 0;
bool Global::isPrint = false;
QStringList Global::ferConfigStrContent;
