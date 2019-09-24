#ifndef DATAFORMAT_H
#define DATAFORMAT_H

#include <QWidget>
#include <QtNetwork>

#define BCT_R_REALDATA      0x01    //读实时数据
#define BCT_R_HISDATA       0x02    //读历史数据
#define BCT_R_SETDATA       0x03    //读设置参数
#define BCT_R_USEREVENT     0x04    //读用户事件
#define BCT_R_SIGNINFOR     0x05    //读登录信息

#define BCT_W_REALDATA      0x21    //写实时数据
#define BCT_W_HISDATA       0x22    //写历史数据
#define BCT_W_SETDATA       0x23    //写设置参数
#define BCT_W_USEREVENT     0x24    //写用户事件
#define BCT_W_SIGNINFOR     0x25    //写登录信息

#define BDT_BOOL            0x01    //Bool 类型
#define BDT_INT             0x02    //int 类型
#define BDT_UINT            0x03    //unsigned Int 类型
#define BDT_USHORT          0x04    //unsigned short 类型
#define BDT_DOUBLE          0x05    //DOUBLE 类型
#define BDT_FLOAT           0x06    //float 类型
#define BDT_CHAR            0x07    //char 类型
#define BDT_UCHAR           0x08    //unsigned char 类型
#define BDT_STRING          0x09    //QString 类型

#define MODEL_COLUMNS       21    //槽数据列

enum DeviceIdType {
    FER = 1,
    DEO,
    FAN_VALVE,
    FAN,
    MIX,
    YHC,
    FPJ
};

enum msgname {
    fjsw  =  7,
    yhfpsw
};

enum CommandType
{
    r_RealData= 1,  //读实时数据
    r_HisData,      //读历史数据
    r_Report,       //读报告信息
    r_SignIn,       //读登录信息
    r_SetData,      //读设置参数
    r_UserEvent,    //读用户事件
    r_AllCacheData, //读所有当前数据
    r_DailyReport,  //读取日报

    w_RealData= 20, //写实时数据
    w_SetData,      //写设置参数
    w_UserEvent,    //写用户事件
    w_AddUser,    //写登录信息
    W_Update_PlcData, //更新PLC数据
    W_Send_Control  //发送控制命令
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
    String,
    Dt,
    Dtl
};

enum TimeType
{
    Day,
    Hour,
    Minute,
    Second
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

struct YhcDeviceInfo
{
    quint32 Device_Number;                  //设备总数
    quint32 RunCtr_Block_Size;              //使能参数总数
    quint32 Yhc_Address;                    //变量起始地址
    quint32 Yhc_Num;                        //变量个数
    quint32 Yhc_Category;                   //变量种类
    QString Yhc_Type;                       //变量数据类型
    quint32 Yhc_Setting_Address;            //设定起始地址
    quint32 Yhc_Setting_Num;                //设定变量个数
    quint32 Yhc_Setting_Display_Num;        //发酵设定界面显示个数
    quint32 Yhc_Setting_Category;           //发酵设定变量种类
    QString Yhc_Setting_Type;               //发酵设定数据类型
    quint32 Runctr_Address;                 //使能变量起始地址
    quint32 Runctr_Num;                     //使能变量个数
    quint32 Runctr_Category;                //使能变量种类
    QString Runctr_Type;                    //使能变量数据类型
};

struct FermenationDeviceInfo
{
    quint32 Device_Number;                //设备总数
    quint32 RunCtr_Block_Size;             //使能参数总数
    quint32 Fermentation_Address;       //温度变量起始地址
    quint32 Fermentation_Num;           //温度变量个数
    quint32 Fermentation_Category;      //温度变量种类
    QString Fermentation_Type;          //温度变量数据类型
    quint32 Tempture_Setting_Address;       //温度设定起始地址
    quint32 Tempture_Setting_Num;           //温度设定个数
    quint32 Tempture_Setting_Category;      //温度设定种类
    QString Tempture_Setting_Type;          //温度设定数据类型
    quint32 Runtime_Address;    //运行时间起始地址
    quint32 Runtime_Num;        //运行时间变量个数
    quint32 Runtime_Category;   //运行时间变量种类
    QString Runtime_Type;       //运行时间数据类型
    quint32 Fermentation_Time_Setting_Address;    //发酵运行时间设定起始地址
    quint32 Fermentation_Time_Setting_Num;        //发酵运行时间设定变量个数
    quint32 Fermentation_Time_Setting_Category;   //发酵运行时间设定变量种类
    QString Fermentation_Time_Setting_Type;       //发酵运行时间设定数据类型
    quint32 Aeration_Runtime_Setting_Address;    //曝气运行时间设定起始地址
    quint32 Aeration_Runtime_Setting_Num;        //曝气运行时间设定变量个数
    quint32 Aeration_Runtime_Setting_Category;   //曝气运行时间设定变量种类
    QString Aeration_Runtime_Setting_Type;       //曝气运行时间设定数据类型
    quint32 Aeration_Hz_Setting_Address;    //曝气频率设定起始地址
    quint32 Aeration_Hz_Setting_Num;        //曝气频率设定变量个数
    quint32 Aeration_Hz_Setting_Category;   //曝气频率设定变量种类
    QString Aeration_Hz_Setting_Type;       //曝气频率设定数据类型
    quint32 Fermentation_DateTime_Address;       //发酵起始时间起始地址
    quint32 Fermentation_DateTime_Num;           //发酵起始变量个数
    quint32 Fermentation_DateTime_Category;      //发酵起始变量种类
    QString Fermentation_DateTime_Type;          //发酵起始数据类型
    quint32 Tank_State_Address;       //运行步骤起始地址
    quint32 Tank_State_Num;           //运行步骤变量个数
    quint32 Tank_State_Category;      //运行步骤变量种类
    QString Tank_State_Type;          //运行时间数据类型
    quint32 Enviroment_Address; //环境变量起始地址
    quint32 Enviroment_Num;     //环境变量个数
    quint32 Enviroment_Category;//环境变量种类
    QString Enviroment_Type;    //环境变量数据类型
    quint32 Runctr_Address;     //使能变量起始地址
    quint32 Runctr_Num;         //使能变量个数
    quint32 Runctr_Category;    //使能变量种类
    QString Runctr_Type;        //使能变量数据类型
};

struct DeodorationDeviceInfo
{
    quint32 Device_Number;                //设备总数
    quint32 RunCtr_Block_Size;             //使能参数总数
    quint32 Deodoration_Address;       //发酵变量起始地址
    quint32 Deodoration_Num;           //发酵变量个数
    quint32 Deodoration_Category;      //发酵变量种类
    QString Deodoration_Type;          //发酵变量数据类型
    quint32 Deo_Setting_Address;    //发酵设定起始地址
    quint32 Deo_Setting_Num;        //发酵设定变量个数
    quint32 Deo_Setting_Display_Num;//发酵设定界面显示个数
    quint32 Deo_Setting_Category;   //发酵设定变量种类
    QString Deo_Setting_Type;       //发酵设定数据类型
    quint32 Enviroment_Address; //环境变量起始地址
    quint32 Enviroment_Num;     //环境变量个数
    quint32 Enviroment_Category;//环境变量种类
    QString Enviroment_Type;    //环境变量数据类型
    quint32 Runctr_Address;     //使能变量起始地址
    quint32 Runctr_Num;         //使能变量个数
    quint32 Runctr_Category;    //使能变量种类
    QString Runctr_Type;        //使能变量数据类型
};

struct FanValveDeviceInfo
{
    quint32 Device_Number;      //设备总数
    quint32 RunCtr_Block_Size;  //使能参数总数
    quint32 Runctr_Address;     //使能变量起始地址
    quint32 Runctr_Num;         //使能变量个数
    quint32 Runctr_Category;    //使能变量种类
    QString Runctr_Type;        //使能变量数据类型
};

struct FanDeviceInfo
{
    quint32 Device_Number;      //设备总数
    quint32 RunCtr_Block_Size;  //使能参数总数
    quint32 Aeration_Fan_Hz_Address;    //曝气频率反馈起始地址
    quint32 Aeration_Fan_Hz_Num;        //曝气频率反馈变量个数
    quint32 Aeration_Fan_Hz_Category;   //曝气频率反馈变量种类
    QString Aeration_Fan_Hz_Type;       //曝气频率反馈数据类型
    quint32 Aeration_Fan_Hz_SETTING_Address;    //曝气频率设定起始地址
    quint32 Aeration_Fan_Hz_SETTING_Num;        //曝气频率设定变量个数
    quint32 Aeration_Fan_Hz_SETTING_Category;   //曝气频率设定变量种类
    QString Aeration_Fan_Hz_SETTING_Type;       //曝气频率设定数据类型
    quint32 Runctr_Address;     //使能变量起始地址
    quint32 Runctr_Num;         //使能变量个数
    quint32 Runctr_Category;    //使能变量种类
    QString Runctr_Type;        //使能变量数据类型
};

struct DeviceNode
{
    int Id = 0;
    QString Name ="NULL";
    QString Cname = "NULL";
    int StartAddress = -1;
    ushort Offset = 0;
    float Diff = 0.0;
    int Length = 0;
    QString Category = "NULL";
    QString DataType = "NULL";
    QString Alert0 = "NULL";
    QString Alert1 = "NULL";
};

struct DeviceGroupInfo
{
    int deviceId;
    int startIndex;
    int groupId;
    int offset;
    int deviceNumber;
};

struct ServerInfo
{
    int Port;
    QString IP;
};

struct SystemConfig
{
    int Port;
    QString IP;
    QString dataSharePath;
    QString dataSemPath;
    QString dataMsgPath;
    QString controlSharePath;
    QString controlSemPath;
    QString controlMsgPath;
    QString yhcControlSharePath;
    QString yhcControlSemPath;
    QString yhcControlMsgPath;
    int dataShareKey;
    int dataSemKey;
    int dataMsgKey;
    int controlShareKey;
    int controlSemKey;
    int controlMsgKey;
    int yhcControlShareKey;
    int yhcControlSemKey;
    int yhcControlMsgKey;
};

#endif // DATAFORMAT_H
