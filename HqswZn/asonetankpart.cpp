#include "asonetankpart.h"
#include "ui_asonetankpart.h"
#include "global.h"

#include <QPainter>
#include <QFontMetrics>
#include <QDebug>

#define TANK_H 220
#define HEIGHT_SPAN 5

AsOneTankPart::AsOneTankPart(QWidget *parent) :
    BaseTankWidget(parent),
    ui(new Ui::AsOneTankPart)
{
    ui->setupUi(this);

    path = new QPainterPath();

    switchStateTimer = new QTimer(this);
    connect(switchStateTimer, SIGNAL(timeout()), this, SLOT(switchState()));

    ui->ocValueLabel->setVisible(false);
    ui->oxygenContent->setVisible(false);
    ui->wtValueLabel_3->setVisible(false);

    updateDeviceState();
}

AsOneTankPart::~AsOneTankPart()
{
    delete ui;
}

int AsOneTankPart::getTankIndex() const
{
    return tankIndex;
}

void AsOneTankPart::setTankIndex(int value)
{
    tankIndex = value;
    if(value != 2 && value != 6 && value != 10 && value != 14)
    {
        ui->weightTempture->setVisible(false);
        ui->wtValueLabel->setVisible(false);
        ui->wtValueLabel_1->setVisible(false);
    }
}

int AsOneTankPart::getLocation() const
{
    return location;
}

void AsOneTankPart::setLocation(int value)
{
    location = value;
}

void AsOneTankPart::updateFermentationData(QSet<int> changedDeviceSet, QMap<float, QString> dataMap)
{
    if(changedDeviceSet.contains(tankIndex))
    {
        parseFermentationData(dataMap);
        parseFerRunTimeData(dataMap);
        parseFerStepData(dataMap);
        parseFerRunCtrData(dataMap);
        parseFerStartEndTime(dataMap);
        parseFerStuffHeight(dataMap);
        emit dataChanged(changedDeviceSet, dataMap);
    }
}

void AsOneTankPart::updateDeviceState()
{
    /*ui->tankFrame->setObjectName("tankframe");
    ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerAStop.bmp)}");

    if(isFerStart)
    {
        ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARun.bmp)}");

        if(isAeration)
        {
            ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARunBQ.bmp)}");
        }
    }*/

    if(!isFanOpen)
    {
        ui->fanButton->setIcon(QIcon(fanCloseBgImg));
    }
    else
    {
        ui->fanButton->setIcon(QIcon(fanOpenBgImg));
    }
}

void AsOneTankPart::switchState()
{
    //ui->tankFrame->setObjectName("tankframe");

    extraRuntimeAddition = extraRuntimeAddition + 2;

    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode;
    float address;
    uint runtime;

    if(!isFerStart)
    {
        deviceNode = Global::getFermenationNodeInfoByName("FER_TOT_UDI");
        address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
        runtime = Global::currentFermenationDataMap[address].toUInt() + extraRuntimeAddition;
        ui->totalTimeValueLabel->setText(formatLongDateString(runtime));

        if(isAeration)
        {
            deviceNode = Global::getFermenationNodeInfoByName("FER_AE_UDI");
            float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
            uint runtime = Global::currentFermenationDataMap[address].toUInt() + extraRuntimeAddition;
            ui->runTimeValueLabel->setText(QString::number(runtime));

            if(switchFlag)
            {
                //ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARunBQ.bmp)}");
            }
            else
            {
                //ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARun.bmp)}");
            }
        }
        else
        {
            deviceNode = Global::getFermenationNodeInfoByName("FER_STA_UDI");
            float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
            uint runtime = Global::currentFermenationDataMap[address].toUInt() + extraRuntimeAddition;
            ui->spacTimeValueLabel->setText(QString::number(runtime));
        }
    }

    if(isFanFault)
    {
        if(switchFlag)
        {
            ui->fanButton->setIcon(QIcon(fanFaultBgImg));
        }
        else
        {
            ui->fanButton->setIcon(QIcon(fanCloseBgImg));
        }
    }

    info = Global::getFerDeviceGroupInfo(deviceIndex);
    deviceNode = Global::getFermenationNodeInfoByName("Fer_Stuff_Height");
    address = deviceNode.Offset + (info.offset + deviceIndex * 5 - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    H1 = Global::currentFermenationDataMap[address].toFloat() / 10;
    H2 = Global::currentFermenationDataMap[address + HEIGHT_SPAN * Global::getLengthByDataType(deviceNode.DataType)].toFloat() / 10;

    QFile dataFile("height.txt");
    if(!dataFile.open(QIODevice::Append))
    {

    }

    QTextStream out(&dataFile);
    out << "H1: " << H1 << ", H2: " << H2 << "\r" << endl;

    if(H1 != old_h1 || H2 != old_h2)
    {
        delete path;
        path = new QPainterPath();
        path->moveTo(0.0, (float)(300.0 - H1));
        path->lineTo(0.0, 300.0);
        path->lineTo(100.0, 300.0);
        path->lineTo(100.0, (float)(300.0 - H2));

        path->closeSubpath();

        update();
    }

    old_h1 = H1;
    old_h2 = H2;

    switchFlag = !switchFlag;
}

void AsOneTankPart::showEvent(QShowEvent *event)
{
    if(!switchStateTimer->isActive())
    {
        switchStateTimer->start(2000);
    }
}

void AsOneTankPart::closeEvent(QCloseEvent *event)
{
    if(switchStateTimer->isActive())
    {
        switchStateTimer->stop();
    }
}

void AsOneTankPart::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush backBrush(QColor(215, 220, 251, 255));
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(rect(), backBrush);

    QLinearGradient gradient(0, 0, 0, 255);
    gradient.setColorAt(0.0, QColor(239, 241, 254));
    gradient.setColorAt(1.0, QColor(116, 135, 241));
    painter.setBrush(gradient);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.fillPath(*path, gradient);

    QPen pen(Qt::gray);
    if(tankIndex == 0)
    {
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(10);
    }
    else
    {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(3);
    }

    painter.setPen(pen);
    painter.drawLine(QPointF(0.0, 0.0), QPointF(0.0, 800.0));

    if(tankIndex == Global::ferDeviceInfo.Device_Number - 1)
    {
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(10);
        painter.setPen(pen);
        painter.drawLine(QPointF(200.0, 0.0), QPointF(200.0, 800.0));
    }

    QPen textPen(Qt::gray);
    painter.setPen(textPen);
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(10);
    font.setBold(true);
    painter.setFont(font);
    QFontMetrics fm = painter.fontMetrics();
    int textHeight = fm.ascent()+fm.descent();
    int textWidth = fm.width(QString::number(H2));
    painter.drawText(rect().width() - textWidth - 5,
                     300 - H2 - textHeight,
                     QString::number((int)floor(H2)));

    painter.end();
}

void AsOneTankPart::changePath(QList<QPointF> points)
{
    QPainterPath path1;
    path1.moveTo(0.0, 150.0);
    path1.lineTo(0.0, 300.0);
    path1.lineTo(400.0, 300.0);
    path1.lineTo(400.0, 50.0);
    path1.lineTo(300.0, 80.0);
    path1.lineTo(200.0, 60.0);
    path1.lineTo(100.0, 100.0);
    //path.arcTo(20.0, 30.0, 60.0, 40.0, 60.0, 240.0);
    path1.closeSubpath();

    *path = path1;
}

void AsOneTankPart::parseFermentationData(QMap<float, QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_WT_R");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    float wt = dataMap[address].toFloat() + 20;
    ui->wtValueLabel->setText(QString::number(wt));

    /*deviceNode = Global::getFermenationNodeInfoByName("FER_CO_R");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->ocValueLabel->setText(dataMap[address]);*/
}

void AsOneTankPart::parseFerRunTimeData(QMap<float, QString> dataMap)
{
    extraRuntimeAddition = 0;

    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode;
    float address;
    uint runtime;
    deviceNode = Global::getFermenationNodeInfoByName("FER_TOT_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    runtime = dataMap[address].toUInt();
    ui->totalTimeValueLabel->setText(formatLongDateString(runtime));

    /*deviceNode = Global::getFermenationNodeInfoByName("FER_START_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    runtime = dataMap[address].toUInt();
    QDateTime dt = QDateTime::fromTime_t(runtime);
    ui->startTimeValueLabel->setText(dt.toString("yyyy-MM-dd hh:mm"));

    deviceNode = Global::getFermenationNodeInfoByName("FER_END_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    runtime = dataMap[address].toUInt();
    qDebug() << "fercontrol endtime = " << runtime;
    dt = QDateTime::fromTime_t(runtime);
    ui->endTimeValueLabel->setText(dt.toString("yyyy-MM-dd hh:mm"));*/

    deviceNode = Global::getFermenationNodeInfoByName("FER_AE_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    runtime = dataMap[address].toUInt();
    ui->runTimeValueLabel->setText(QString::number(runtime));

    deviceNode = Global::getFermenationNodeInfoByName("FER_STA_UDI");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    runtime = dataMap[address].toUInt();
    ui->spacTimeValueLabel->setText(QString::number(runtime));
}

void AsOneTankPart::parseFerStepData(QMap<float, QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_STEPCTR_UI");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    int step = dataMap[address].toInt();

    deviceNode = Global::getFermenationNodeInfoByName("FER_Stuff_ID");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    ui->stuffIdLabel->setText(dataMap[address]);
}

void AsOneTankPart::parseFerRunCtrData(QMap<float, QString> dataMap)
{
    isFanRemote = Global::getFerRunctrValueByName(tankIndex,"FAN_Remote_BOOL", dataMap);
    isFanAuto = Global::getFerRunctrValueByName(tankIndex,"FAN_Auto_BOOL", dataMap);
    isFanOpen = Global::getFerRunctrValueByName(tankIndex,"FAN_Run_BOOL", dataMap);
    isFerStart = Global::getFerRunctrValueByName(tankIndex,"FER_State_BOOL", dataMap);
    isAeration = Global::getFerRunctrValueByName(tankIndex,"FER_AERATION_BOOL", dataMap);
    isFerAuto = Global::getFerRunctrValueByName(tankIndex,"FER_Auto_BOOL", dataMap);
    isFanFault = Global::getFerRunctrValueByName(tankIndex,"FAN_Fault_BOOL", dataMap);

    //ui->tankFrame->setObjectName("tankframe");
    //ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerAStop.bmp)}");

    if(isFerStart)
    {
        //ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARun.bmp)}");

        if(isAeration)
        {
            //ui->tankFrame->setStyleSheet("QFrame#tankframe{border-image:url(:/image/old/nerFer/FerARunBQ.bmp)}");
        }
    }

    if(!isFanOpen)
    {
        ui->fanButton->setIcon(QIcon(fanCloseBgImg));
    }
    else
    {
        ui->fanButton->setIcon(QIcon(fanOpenBgImg));
    }

    if(isFanFault)
    {
        ui->fanButton->setIcon(QIcon(fanFaultBgImg));
    }
}

void AsOneTankPart::parseFerStartEndTime(QMap<float, QString> dataMap)
{
    int deviceIndex = tankIndex;
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);

    DeviceNode deviceNode = Global::getFermenationNodeInfoByName("FER_Start_Time");
    float address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "Start time = " << dataMap[address];
    uint runtime = dataMap[address].toUInt();
    QDateTime dt = QDateTime::fromTime_t(runtime);
    ui->startTimeValueLabel->setText(dt.toString("yyyy-MM-dd hh:mm"));

    deviceNode = Global::getFermenationNodeInfoByName("FER_End_Time");
    address = deviceNode.Offset + (info.offset + deviceIndex - info.startIndex) * Global::getLengthByDataType(deviceNode.DataType);
    qDebug() << "End time = " << dataMap[address];
    runtime = dataMap[address].toUInt();
    dt = QDateTime::fromTime_t(runtime);
    ui->endTimeValueLabel->setText(dt.toString("yyyy-MM-dd hh:mm"));
}

void AsOneTankPart::parseFerStuffHeight(QMap<float, QString> dataMap)
{

}

QString AsOneTankPart::formatLongDateString(uint value)
{
    int day = value / 86400;
    int hour = (value - day*86400) / 3600;
    int minute = (value - day*86400 - hour*3600) / 60;
    int second = (value - day*86400 - hour*3600 - minute * 60);
    return QStringLiteral("%1天%2时%3分%4秒").arg(QString::number(day)).arg(QString::number(hour)).arg(QString::number(minute)).arg(QString::number(second));
}
