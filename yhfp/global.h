#ifndef GLOBAL_H
#define GLOBAL_H

#include "dataformat.h"

#include <QStandardItemModel>
#include <QMap>

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

    static FermenationDeviceInfo ferDeviceInfo;
    static DeodorationDeviceInfo deoDeviceInfo;
    static FanValveDeviceInfo fanValveDeviceInfo;
    static FanDeviceInfo fanDeviceInfo;

    static QVector<DeviceNode> ferDeviceNodes;
    static QVector<DeviceNode> deoDeviceNodes;
    static QVector<DeviceNode> fanValveDeviceNodes;
    static QVector<DeviceNode> fanDeviceNodes;

    static QVector<DeviceGroupInfo> deoDeviceGroupInfos;
    static QVector<DeviceGroupInfo> fanValveDeviceGroupInfos;
    static QVector<DeviceGroupInfo> fanDeviceGroupInfos;
    static QVector<DeviceGroupInfo> ferDeviceGroupInfos;

    static QVector<DeviceNode> ferRunCtrDeviceNodes;
    static QVector<DeviceNode> deoRunCtrDeviceNodes;
    static QVector<DeviceNode> fanValveRunCtrDeviceNodes;
    static QVector<DeviceNode> fanRunCtrDeviceNodes;

    static QString getNodeNameByAddress(ushort address, QVector<DeviceNode> deviceNodes);
    static QString getNodeCnameByAddress(ushort address, QVector<DeviceNode> deviceNodes);

    static int getFerDeviceIndexByAddress(ushort address);
    static int getDeoDeviceIndexByAddress(ushort address);

    static int getFerDeviceIndexByRunctrAddress(float address);
    static int getDeoDeviceIndexByRunctrAddress(float address);

    static DeviceNode getFermenationNodeInfoByCname(QString cname);
    static DeviceNode getDeodorationNodeInfoByCname(QString cname);
    static DeviceNode getFanValveNodeInfoByCname(QString cname);
    static DeviceNode getFanNodeInfoByCname(QString cname);

    static DeviceGroupInfo getDeoDeviceGroupInfo(int index);
    static DeviceGroupInfo getFanValveDeviceGroupInfo(int index);
    static DeviceGroupInfo getFanDeviceGroupInfo(int index);
    static DeviceGroupInfo getFerDeviceGroupInfo(int index);

    static QStandardItemModel *alertsModel;
    static QStandardItemModel *simpleAlertsModel;
    static QStandardItemModel *loginHistoryModel;
    static QMap<float,QString> currentFermenationDataMap;
    static QMap<float,QString> currentDeodorationDataMap;
    static QMap<float,QString> currentFanGroupDataMap;
    static QMap<float,QString> currentFanValveGroupDataMap;

    static bool getFerRunctrValueByName(int deviceIndex, QString name, QMap<float,QString> dataMap);
    static bool getDeoRunctrValueByName(int deviceIndex, QString name, QMap<float,QString> dataMap);

    static int getFerDeviceStartIndex(int deviceId, int deviceGroup);
    static int getFanDeviceStartIndex(int deviceId, int deviceGroup);
    static int getFanValveDeviceStartIndex(int deviceId, int deviceGroup);
    static int getDeoDeviceStartIndex(int deviceId, int deviceGroup);

    static uint getLengthByDataType(QString dataType);

    static int alertIndex;

private:
    static FermenationDeviceInfo readFermenationDeviceInfo();
    static DeodorationDeviceInfo readDeodorationDeviceInfo();
    static FanValveDeviceInfo readFanValveDeviceInfo();
    static FanDeviceInfo readFanDeviceInfo();

    static ServerInfo readServerInfo();
    static SystemConfig readSystemConfig();
};

#endif // GLOBAL_H
