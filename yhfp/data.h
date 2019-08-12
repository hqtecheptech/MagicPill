#ifndef DATA_H
#define DATA_H

#include <QMetaType>

enum CommandType
{
    r_RealData= 1,  //读实时数据
    r_HisData,      //读历史数据
    r_Report,       //读报告信息
    r_SignIn,       //读登录信息
    r_SetData,      //读设置参数
    r_UserEvent,    //读用户事件
    r_AllCacheData, //读所有当前数据

    w_RealData= 20, //写实时数据
    w_SetData,      //写设置参数
    w_UserEvent,    //写用户事件
    w_AddUser,    //写登录信息
    w_Update_Plcdata //update plc data
};

enum DataType
{
    Bool = 1,
    Int,
    UInt,
    UShort,
    Double,
    Float,
    Char,
    UChar,
    String
};

struct StreamPack
{
    quint32 bStreamLength;  //Pack_0
    quint16 bDeviceId;      //Pack_1
    quint16 bDeviceGroup;   //Pack_2
    quint16 bCommandType;   //Pack_3
    quint16 bDataType;      //Pack_4
    quint16 bAddress;       //Pack_5
    quint16 bIndex;         //Pack_6
    quint32 bDataLength;    //Pack_7
    quint16 bErrorCode;     //Pack_8
    quint32 bStartTime;     //Pack_9
    quint32 bEndTime;       //Pack_10
};

struct PlcData
{
    int values[20];
};

Q_DECLARE_METATYPE(PlcData)

#endif // DATA_H
