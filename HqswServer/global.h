#ifndef GLOBAL_H
#define GLOBAL_H

#include "dataformat.h"
#include <QMap>
#include <QVector>
#include <QDebug>

class Global
{
public:
    Global();

    static ServerInfo serverInfo;
    static SystemConfig systemConfig;

    static DeviceNode getFermenationNodeInfoByName(QString name);
    static DeviceNode getDeodorationNodeInfoByName(QString name);
    static DeviceNode getFanValveNodeInfoByName(QString name);
    static DeviceNode getFanNodeInfoByName(QString name);
    static DeviceNode getYhcNodeInfoByName(QString name);
    static DeviceNode getMixNodeInfoByName(QString name);

    static FermenationDeviceInfo ferDeviceInfo;
    static DeodorationDeviceInfo deoDeviceInfo;
    static FanValveDeviceInfo fanValveDeviceInfo;
    static FanDeviceInfo fanDeviceInfo;
    static YhcDeviceInfo yhcDeviceInfo;
    static MixDeviceInfo mixDeviceInfo;

    static QVector<DeviceNode> ferDeviceNodes;
    static QVector<DeviceNode> deoDeviceNodes;
    static QVector<DeviceNode> fanValveDeviceNodes;
    static QVector<DeviceNode> fanDeviceNodes;
    static QVector<DeviceNode> yhcDeviceNodes;
    static QVector<DeviceNode> mixDeviceNodes;

    static QVector<DeviceGroupInfo> deoDeviceGroupInfos;
    static QVector<DeviceGroupInfo> fanValveDeviceGroupInfos;
    static QVector<DeviceGroupInfo> fanDeviceGroupInfos;
    static QVector<DeviceGroupInfo> ferDeviceGroupInfos;
    static QVector<DeviceGroupInfo> yhcDeviceGroupInfos;
    static QVector<DeviceGroupInfo> mixDeviceGroupInfos;

    static QVector<DeviceNode> ferRunCtrDeviceNodes;
    static QVector<DeviceNode> deoRunCtrDeviceNodes;
    static QVector<DeviceNode> fanValveRunCtrDeviceNodes;
    static QVector<DeviceNode> fanRunCtrDeviceNodes;
    static QVector<DeviceNode> yhcRunCtrDeviceNodes;
    static QVector<DeviceNode> mixRunCtrDeviceNodes;

    static QString getNodeNameByAddress(ushort address, QVector<DeviceNode> deviceNodes);
    static QString getNodeCnameByAddress(ushort address, QVector<DeviceNode> deviceNodes);

    static int getFerDeviceIndexByAddress(ushort address);
    static int getDeoDeviceIndexByAddress(ushort address);
    static int getYhcDeviceIndexByAddress(ushort address);
    static int getMixDeviceIndexByAddress(ushort address);

    static int getFerDeviceIndexByRunctrAddress(float address);
    static int getDeoDeviceIndexByRunctrAddress(float address);
    static int getYhcDeviceIndexByRunctrAddress(float address);
    static int getMixDeviceIndexByRunctrAddress(float address);

    static DeviceNode getFermenationNodeInfoByCname(QString cname);
    static DeviceNode getDeodorationNodeInfoByCname(QString cname);
    static DeviceNode getFanValveNodeInfoByCname(QString cname);
    static DeviceNode getFanNodeInfoByCname(QString cname);
    static DeviceNode getYhcNodeInfoByCname(QString cname);
    static DeviceNode getMixNodeInfoByCname(QString cname);

    static DeviceNode getYhcNodeInfoByRunctrAddress(float address);
    static DeviceNode getMixNodeInfoByRunctrAddress(float address);

    static DeviceGroupInfo getDeoDeviceGroupInfo(int index);
    static DeviceGroupInfo getFanValveDeviceGroupInfo(int index);
    static DeviceGroupInfo getFanDeviceGroupInfo(int index);
    static DeviceGroupInfo getFerDeviceGroupInfo(int index);
    static DeviceGroupInfo getYhcDeviceGroupInfo(int index);
    static DeviceGroupInfo getMixDeviceGroupInfo(int index);

    static DeviceGroupInfo getFerDeviceGroupInfoByGroupId(int groupId);
    static DeviceGroupInfo getYhcDeviceGroupInfoByGroupId(int groupId);
    static DeviceGroupInfo getMixDeviceGroupInfoByGroupId(int groupId);
    static DeviceGroupInfo getDeoDeviceGroupInfoByGroupId(int groupId);

    static QMap<float,QString> currentFermenationDataMap;
    static QMap<float,QString> currentDeodorationDataMap;
    static QMap<float,QString> currentFanGroupDataMap;
    static QMap<float,QString> currentFanValveGroupDataMap;
    static QMap<float,QString> currentYhcDataMap;
    static QMap<float,QString> currentMixDataMap;

    static bool getFerRunctrValueByName(int deviceIndex, QString name, QMap<float,QString> dataMap);
    static bool getDeoRunctrValueByName(int deviceIndex, QString name, QMap<float,QString> dataMap);
    static bool getYhcRunctrValueByName(int deviceIndex, QString name, QMap<float,QString> dataMap);
    static bool getMixRunctrValueByName(int deviceIndex, QString name, QMap<float,QString> dataMap);

    static int getFerDeviceStartIndex(int deviceId, int deviceGroup);
    static int getFanDeviceStartIndex(int deviceId, int deviceGroup);
    static int getFanValveDeviceStartIndex(int deviceId, int deviceGroup);
    static int getDeoDeviceStartIndex(int deviceId, int deviceGroup);
    static int getYhcDeviceStartIndex(int deviceId, int deviceGroup);
    static int getMixDeviceStartIndex(int deviceId, int deviceGroup);

    static int getDataStartByType(QString type);
    static int convertAddressToIndex(float address, QString type);
    static float convertIndexToAddress(int index, QString type);

    static float getRunctrAddressByIndex(int index);
    static int getYhcDataIndexByName(QString name, int deviceIndex);
    static int getMixDataIndexByName(QString name, int deviceIndex);
    static int getFerDataIndexByName(QString name, int deviceIndex);

    static uint getLengthByDataType(QString dataType);
    static int getPruPid();

    static int alertIndex;
    static bool isPrint;
    static int ferGroupShow;
    static QStringList ferConfigStrContent;

private:
    static FermenationDeviceInfo readFermenationDeviceInfo();
    static DeodorationDeviceInfo readDeodorationDeviceInfo();
    static FanValveDeviceInfo readFanValveDeviceInfo();
    static FanDeviceInfo readFanDeviceInfo();
    static YhcDeviceInfo readYhcDeviceInfo();
    static MixDeviceInfo readMixDeviceInfo();

    static ServerInfo readServerInfo();
    static SystemConfig readSystemConfig();
    static int readFerGroupShowing();
};

#endif // GLOBAL_H
