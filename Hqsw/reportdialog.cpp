#include "reportdialog.h"
#include "ui_reportdialog.h"
#include "dataformat.h"
#include "requestworkthread.h"
#include "global.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDate>
#include <QFileDialog>
#include <QMessageBox>
//#include <QAxObject>

ReportDialog::ReportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportDialog)
{
    ui->setupUi(this);
    //setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowTitle(QStringLiteral("报表查询"));

    QVector<DeviceNode> nodes = Global::ferDeviceNodes;
    foreach (DeviceNode node, nodes) {
        if(node.DataType == "r" && node.Name.contains("FER"))
        {
            QListWidgetItem *elem = new QListWidgetItem(node.Cname);
            elem->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
            elem->setCheckState(Qt::Unchecked);
            ui->cbxListWidget->addItem(elem);
        }
    }

    for(int i=0; i < Global::ferDeviceInfo.Device_Number; i++)
    {
        ui->tankComboBox->addItem(QString::number(i+1) + QStringLiteral("#槽"));
    }

    reportModel = new QStandardItemModel(this);

    getReportTcpClient = new TcpClientSocket(this);
    connect(getReportTcpClient,SIGNAL(updateClients(QByteArray)),this,SLOT(showReport(QByteArray)));
    connect(this, SIGNAL(finished(int)), getReportTcpClient, SLOT(deleteLater()));

    getDailyReportTcpClient = new TcpClientSocket(this);
    connect(getDailyReportTcpClient,SIGNAL(updateClients(QByteArray)),this,SLOT(showDailyReport(QByteArray)));
    connect(this, SIGNAL(finished(int)), getDailyReportTcpClient, SLOT(deleteLater()));
}

ReportDialog::~ReportDialog()
{
    delete ui;
}

void ReportDialog::on_getReportButton_clicked()
{
    StreamPack bpack;

    QDate startDate = ui->startTimeEdit->dateTime().date();
    QDate endDate = ui->endTimeEdit->dateTime().date();

    uint stime = ui->startTimeEdit->dateTime().toTime_t();
    uint etime = ui->endTimeEdit->dateTime().toTime_t();

    if(etime > stime)
    {
        if((endDate.year() - startDate.year()) <= 1)
        {
            reportAddressList.clear();

            int deviceIndex = ui->tankComboBox->currentIndex();
            DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
            for(int row=0; row < ui->cbxListWidget->count(); row++)
            {
                QListWidgetItem *item = ui->cbxListWidget->item(row);
                if(item->checkState() == Qt::Checked)
                {
                    DeviceNode node = Global::getFermenationNodeInfoByCname(item->text());
                    reportAddressList.append(node.Offset + 4 * (info.offset + deviceIndex - info.startIndex));
                }
            }

            if(reportAddressList.length() > 0)
            {
                bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,r_Report,String,0,(quint16)(info.offset + deviceIndex - info.startIndex),(quint16)reportAddressList.length(),0,stime,etime};

                QVariant var_data = QVariant::fromValue(reportAddressList);
                getReportTcpClient->sendRequestWithResult(bpack, var_data, reportAddressList.length()*4);

                ui->getReportButton->setEnabled(false);
            }
        }
        else
        {
            QMessageBox messageBox;
            messageBox.setText(QStringLiteral("查询间隔不能超过2年！"));
            messageBox.exec();
        }
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText(QStringLiteral("查询结束时间必须大于查询开始时间！"));
        messageBox.exec();
    }
}

void ReportDialog::showReportAtPage(int page)
{
    currentPage = page;
    ui->pageLineEdit->setText(QString::number(currentPage));

    QString msg;

    if(page == 1)
    {
        ui->prePushButton->setEnabled(false);
    }
    else
    {
        ui->prePushButton->setEnabled(true);
    }

    if(page == allPage)
    {
        ui->nextPushButton->setEnabled(false);
    }
    else
    {
        ui->nextPushButton->setEnabled(true);
    }

    QVector<ushort> valueAddresses;
    QVector<QDateTime> times;

    qDebug() << "page: " << page;

    if(page > 0 && page < allPage)
    {
        QByteArray byteValues = allAddressBytes.mid(2000 * (page - 1) * 2, 2000 * 2);
        qDebug() << "value bytes: " << byteValues.length();
        qDebug() << "value byteValues.length()/2: " << byteValues.length()/2;

        for(quint16 i=0; i<2000; ++i)
        {
            QByteArray value = byteValues.mid(i*2,2);
            ushort temp = 0;
            memcpy(&temp,value,2);
            valueAddresses.append(temp);
            msg.append(QString::number(temp));
            msg.append(",");
        }

        byteValues = allTimeBytes.mid(2000 * (page - 1) * 4, 2000 * 4);
        qDebug() << "time bytes: " << byteValues.length();
        qDebug() << "time byteValues.length()/4: " << byteValues.length()/4;

        for(quint16 i=0; i<2000; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            int temp = 0;
            memcpy(&temp,value,4);
            times.append(QDateTime::fromTime_t(temp));
            msg.append(QString::number(temp));
            msg.append(",");
        }
    }
    else if(page == allPage)
    {
        QByteArray byteValues = allAddressBytes.mid(2000 * (page - 1) * 2);
        qDebug() << "value bytes: " << byteValues.length();
        qDebug() << "value byteValues.length()/2: " << byteValues.length()/2;

        for(quint16 i=0; i<byteValues.length()/2; ++i)
        {
            QByteArray value = byteValues.mid(i*2,2);
            ushort temp = 0;
            memcpy(&temp,value,2);
            valueAddresses.append(temp);
            msg.append(QString::number(temp));
            msg.append(",");
        }

        byteValues = allTimeBytes.mid(2000 * (page - 1) * 4);
        qDebug() << "time bytes: " << byteValues.length();
        qDebug() << "time byteValues.length()/4: " << byteValues.length()/4;

        for(quint16 i=0; i<byteValues.length()/4; ++i)
        {
            QByteArray value = byteValues.mid(i*4,4);
            int temp = 0;
            memcpy(&temp,value,4);
            times.append(QDateTime::fromTime_t(temp));
            msg.append(QString::number(temp));
            msg.append(",");
        }
    }
    else
    {
        return;
    }

    if(valueAddresses.length() > 0)
    {
        reportModel->clear();
        reportModel->setRowCount(valueAddresses.length());
        reportModel->setColumnCount(3);
        reportModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("指标名称"));
        reportModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("指标值"));
        reportModel->setHeaderData(2,Qt::Horizontal,QStringLiteral("采集时间"));

        reportModel->removeRows(0,reportModel->rowCount(QModelIndex()),QModelIndex());

        for(int j=0; j<valueAddresses.length();++j)
        {
            reportModel->insertRows(j,1,QModelIndex());
            reportModel->setData(reportModel->index(j,0,QModelIndex()),Global::getNodeCnameByAddress(valueAddresses.at(j), Global::ferDeviceNodes));
            reportModel->setData(reportModel->index(j,1,QModelIndex()),reportValueArray.at(2000 * (page - 1) + j));
            reportModel->setData(reportModel->index(j,2,QModelIndex()),times.at(j));
        }

        ui->reportTableView->setModel(reportModel);
        ui->reportTableView->resizeColumnsToContents();
    }
}

void ReportDialog::showDailyReport(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    uint totalDevices = Global::ferDeviceInfo.Device_Number;

    reportModel->clear();
    reportModel->setRowCount(24*3);
    reportModel->setColumnCount(totalDevices + 1);

    QString msg;
    int index = 0;
    if(bDevice.bErrorCode==1)
    {
        dailyReportData = data;
        reportModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("时 间"));
        for(int i=0; i < totalDevices; i++)
        {
            reportModel->setHeaderData(i + 1,Qt::Horizontal, QString::number(i+1) + "#");
        }

        QDateTime dt = ui->startTimeEdit->dateTime();
        int ddLen = 24 * 3;
        for(int j=0; j < ddLen; j++)
        {
            //reportModel->insertRows(j,1,QModelIndex());
            QString hour = QString("%1").arg(j/3, 2, 10, QLatin1Char('0'));
            QString minu = QString("%1").arg(j%3*20, 2, 10, QLatin1Char('0'));

            reportModel->setData(reportModel->index(j,0,QModelIndex()),dt.toString("yyyy-MM-dd") + " "
                                 + hour + ":" + minu);
            for(int k=0; k<totalDevices;k++)
            {
                QByteArray byteValues = data.mid(sizeof(bDevice) + k * 4 * ddLen + 4 * j, 4);
                float value;
                memcpy(&value, byteValues.data(), 4);

                reportModel->setData(reportModel->index(j,k+1,QModelIndex()),QString::number(value, 'f', 2));
            }
        }

        ui->reportTableView->setModel(reportModel);
        ui->reportTableView->resizeColumnsToContents();
    }
    /*StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QString msg;
    int index = 0;
    if(bDevice.bErrorCode==1)
    {
        allPage = 0;
        allAddressBytes.clear();
        allTimeBytes.clear();

        QByteArray byteValues = data.mid(sizeof(bDevice),bDevice.bStreamLength - sizeof(bDevice)
                                         - bDevice.bDataLength*2 - bDevice.bDataLength*4 - 4);
        index += byteValues.length();
        QString reportValues = QString(byteValues);

        reportValueArray.clear();
        reportValueArray = reportValues.split(";");

        allAddressBytes = data.mid(sizeof(bDevice) + index, bDevice.bDataLength*2);
        index += allAddressBytes.length();
        allTimeBytes = data.mid(sizeof(bDevice) + index, bDevice.bDataLength*4);
        index += allTimeBytes.length();

        QVector<ushort> valueAddresses;
        QVector<QDateTime> times;

        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            msg.append(reportValueArray.at(i));
            msg.append(",");

            QByteArray value = allAddressBytes.mid(i*2,2);
            ushort tempAddr = 0;
            memcpy(&tempAddr,value,2);
            valueAddresses.append(tempAddr);
            msg.append(QString::number(tempAddr));
            msg.append(",");

            value = allTimeBytes.mid(i*4,4);
            int tempTime = 0;
            memcpy(&tempTime,value,4);
            times.append(QDateTime::fromTime_t(tempTime));
            msg.append(QString::number(tempTime));
            msg.append(";");

            float rawDataValue = QString(reportValueArray.at(i)).toFloat();
            if(rawDataValue != 0.0)
            {
                ((QMap<int, float> *)reportRawData.value(tempAddr))->insert(tempTime, rawDataValue);
            }
        }

        reportMap.clear();
        foreach (ushort addrKey, reportItemMap.keys())
        {
            float* dataInMapByHours = new float[24];
            reportMap.insert(addrKey, dataInMapByHours);

            QMap<int, float> *dataMap = reportRawData.value(addrKey);

            uint nextHourTime_t = ui->startTimeEdit->dateTime().toTime_t();
            int nextHour = 0;
            float preHourValue = 0.0;
            foreach (int timeKey, dataMap->keys()) {
                if(timeKey == nextHourTime_t)
                {
                    dataInMapByHours[nextHour] = dataMap->value(timeKey);
                    nextHour++;
                    nextHourTime_t += 3600;
                    preHourValue = dataMap->value(timeKey);
                }

                if(timeKey < nextHourTime_t)
                {
                    preHourValue = dataMap->value(timeKey);
                }

                while(timeKey > nextHourTime_t)
                {
                    dataInMapByHours[nextHour] = preHourValue;
                    nextHour++;
                    nextHourTime_t += 3600;
                    preHourValue = dataMap->value(timeKey);
                }
            }

            for(;nextHour < 24; nextHour++)
            {
                dataInMapByHours[nextHour] = preHourValue;
            }

            if(dataInMapByHours[0] == 0.0 && dataInMapByHours[1] != 0.0)
            {
                dataInMapByHours[0] = dataInMapByHours[1] - 0.6;
            }
        }

        reportModel->clear();
        reportModel->setRowCount(24);
        reportModel->setColumnCount(reportMap.keys().length() + 1);

        reportModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("时间"));
        for(int i=0; i < reportMap.keys().length(); i++)
        {
            reportModel->setHeaderData(i + 1,Qt::Horizontal, reportItemMap[reportMap.keys().at(i)]);
        }

        reportModel->removeRows(0,reportModel->rowCount(QModelIndex()),QModelIndex());

        QDateTime dt = ui->startTimeEdit->dateTime();
        for(int j=0; j<24;++j)
        {
            reportModel->insertRows(j,1,QModelIndex());
            reportModel->setData(reportModel->index(j,0,QModelIndex()),dt.toString("yyyy-MM-dd") + " " + QString::number(j) + QStringLiteral("时"));
            for(int k=0; k < reportMap.keys().length(); k++)
            {
                reportModel->setData(reportModel->index(j,k+1,QModelIndex()),reportMap.values().at(k)[j]);
            }
        }

        ui->reportTableView->setModel(reportModel);
        ui->reportTableView->resizeColumnsToContents();
    }*/

    ui->getDailyReportButton->setEnabled(true);
}

void ReportDialog::showReport(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QString msg;
    int index = 0;
    if(bDevice.bErrorCode==1)
    {
        allPage = 0;
        allAddressBytes.clear();
        allTimeBytes.clear();

        QByteArray byteValues = data.mid(sizeof(bDevice),bDevice.bStreamLength - sizeof(bDevice)
                                         - bDevice.bDataLength*2 - bDevice.bDataLength*4 - 4);
        index += byteValues.length();
        QString reportValues = QString(byteValues);
        msg.append(reportValues);
        reportValueArray = reportValues.split(";");

        allAddressBytes = data.mid(sizeof(bDevice) + index, bDevice.bDataLength*2);
        index += allAddressBytes.length();
        qDebug() << "allAddressBytes: " << allAddressBytes.length();
        allTimeBytes = data.mid(sizeof(bDevice) + index, bDevice.bDataLength*4);
        index += allTimeBytes.length();
        qDebug() << "allTimeBytes: " << allTimeBytes.length();

        if((bDevice.bDataLength % 2000) > 0)
        {
            allPage = bDevice.bDataLength / 2000 + 1;
        }
        else
        {
            allPage = bDevice.bDataLength / 2000;
        }
        ui->allPageNumberLabel->setText(QString::number(allPage));

        showReportAtPage(1);
    }
    ui->getReportButton->setEnabled(true);
}

void ReportDialog::showEvent(QShowEvent *)
{

}

void ReportDialog::closeEvent(QCloseEvent *)
{
    //getReportWork->terminate();
}

void ReportDialog::on_importReportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("Excel file"),
                    qApp->applicationDirPath (),QStringLiteral("Excel Files (*.xlsx)"));

    if (fileName.isEmpty())
         return;

    //QMessageBox::about(this, QStringLiteral("您将要建立的Excel文件"), fileName );
    if (exportExcel(fileName)>0 )//调用导出Excel方法
    {
        QMessageBox::about(this, QStringLiteral("Excel文件 导出完成"), fileName );
        QSqlDatabase::removeDatabase("excelexport");
    }
}

int ReportDialog::exportExcel(QString filePath)
{
//    QAxObject *work_book = NULL;
//    QAxObject excel("Excel.Application");
//    // Here is an error when visible is false
//    excel.setProperty("Visible", true);
//    excel.setProperty("DisplayAlerts", false);//不显示任何警告信息
//    QAxObject *work_books = excel.querySubObject("WorkBooks");
//    QFile xlsFile(filePath);
//    if (xlsFile.exists()) {
//        work_book = work_books->querySubObject("Open(const QString &)", filePath);
//        QAxObject *work_sheets = work_book->querySubObject("Sheets");
//        QAxObject *first_sheet = work_sheets->querySubObject("Item(int)", 1);
//        QAxObject *usedRange = first_sheet->querySubObject("UsedRange");
//        usedRange->dynamicCall("ClearContents()");
//    }
//    else {
//        work_books->dynamicCall("Add");
//        work_book = excel.querySubObject("ActiveWorkBook");
//    }

//    QAxObject *work_sheets = work_book->querySubObject("Sheets");
//    QAxObject *first_sheet = work_sheets->querySubObject("Item(int)", 1);
//    QAxObject *cell = first_sheet->querySubObject("Cells(int,int)", 1, 1);
//    cell->setProperty("Value", QStringLiteral("参数名称"));
//    cell = first_sheet->querySubObject("Cells(int,int)", 1, 1);
//    cell->setProperty("Value", QStringLiteral("参数名称"));
//    cell->setProperty("ColumnWidth", 50);  //设置单元格列宽
//    cell = first_sheet->querySubObject("Cells(int,int)", 1, 2);
//    cell->setProperty("Value", QStringLiteral("参数值"));
//    cell->setProperty("ColumnWidth", 30);  //设置单元格列宽
//    cell = first_sheet->querySubObject("Cells(int,int)", 1, 3);
//    cell->setProperty("Value", QStringLiteral("采集时间"));
//    cell->setProperty("ColumnWidth", 60);  //设置单元格列宽

//    QList<QList<QVariant>> mdatas;
//    for(int i=0; i<reportModel->rowCount(QModelIndex()); i++)
//    {
//        QList<QVariant> row;
//        QVariant vName = reportModel->data(reportModel->index(i,0));
//        QString name = vName.toString();
//        QVariant vValue = reportModel->data(reportModel->index(i,1));
//        QString value = vValue.toString();
//        QVariant vDateTime = reportModel->data(reportModel->index(i,2));
//        QDateTime dateTime = vDateTime.toDateTime();
//        QString strTime = dateTime.toString("yyyy/MM/dd hh:mm:ss");

//        row.append(name);
//        row.append(value);
//        row.append(strTime);

//        mdatas.append(row);
//    }

//    writeCurrentSheet(first_sheet, mdatas, 1);

//    work_book->dynamicCall("SaveAs(const QString &)", QDir::toNativeSeparators(filePath)); //转换路径不可少，否则会崩溃
//    //work_book->dynamicCall("Close(Boolean)", false);  //关闭文件
//    excel.dynamicCall("Quit(void)");  //退出

    return 1;
}

int ReportDialog::exportDailyReportExcel(QString filePath)
{
//    QAxObject *work_book = NULL;
//    QAxObject excel("Excel.Application");
//    // Here is an error when visible is false
//    excel.setProperty("Visible", true);
//    excel.setProperty("DisplayAlerts", false);//不显示任何警告信息
//    QAxObject *work_books = excel.querySubObject("WorkBooks");
//    QFile xlsFile(filePath);
//    if (xlsFile.exists()) {
//        work_book = work_books->querySubObject("Open(const QString &)", filePath);
//        QAxObject *work_sheets = work_book->querySubObject("Sheets");
//        QAxObject *first_sheet = work_sheets->querySubObject("Item(int)", 1);
//        QAxObject *usedRange = first_sheet->querySubObject("UsedRange");
//        usedRange->dynamicCall("ClearContents()");
//    }
//    else {
//        work_books->dynamicCall("Add");
//        work_book = excel.querySubObject("ActiveWorkBook");
//    }

//    QAxObject *work_sheets = work_book->querySubObject("Sheets");
//    QAxObject *first_sheet = work_sheets->querySubObject("Item(int)", 1);
//    QAxObject *cell = first_sheet->querySubObject("Cells(int,int)", 1, 1);
//    cell = first_sheet->querySubObject("Cells(int,int)", 1, 1);
//    cell->setProperty("Value", QStringLiteral("时间 | 权重温度"));
//    cell->setProperty("ColumnWidth", 20);  //设置单元格列宽
//    cell->setProperty("HorizontalAlignment", -4108);//水平居中

//    for(int j=0; j < Global::ferDeviceInfo.Device_Number; j ++)
//    {
//        cell = first_sheet->querySubObject("Cells(int,int)", 1, j+2);
//        cell->setProperty("Value", QString::number(j+1) + "#");
//        cell->setProperty("ColumnWidth", 10);  //设置单元格列宽
//        cell->setProperty("HorizontalAlignment", -4108);//水平居中
//    }

//    StreamPack bDevice;
//    QDateTime dt = ui->startTimeEdit->dateTime();
//    int ddLen = 24 * 3;
//    QList<QList<QVariant>> mdatas;
//    for(int j=0; j < ddLen; j++)
//    {
//        QList<QVariant> row;

//        QString hour = QString("%1").arg(j/3, 2, 10, QLatin1Char('0'));
//        QString minu = QString("%1").arg(j%3*20, 2, 10, QLatin1Char('0'));
//        row.append(dt.toString("yyyy-MM-dd") + " " + hour + ":" + minu);

//        for(int k=0; k<Global::ferDeviceInfo.Device_Number;k++)
//        {
//            QByteArray byteValues = dailyReportData.mid(sizeof(bDevice) + k * 4 * ddLen + 4 * j, 4);
//            float value;
//            memcpy(&value, byteValues.data(), 4);

//            row.append(QString::number(value, 'f', 2));
//        }

//        mdatas.append(row);
//    }

//    /*for(int j=0; j < reportMap.keys().length(); j++)
//    {
//        cell = first_sheet->querySubObject("Cells(int,int)", 1, j+2);
//        cell->setProperty("Value", reportItemMap.value(reportMap.keys().at(j)));
//        cell->setProperty("ColumnWidth", 20);  //设置单元格列宽
//    }

//    QList<QList<QVariant>> mdatas;
//    for(int i=0; i<reportModel->rowCount(QModelIndex()); i++)
//    {
//        QList<QVariant> row;

//        QVariant temp = reportModel->data(reportModel->index(i,0));
//        row.append(temp.toString());

//        for(int j=0; j < reportMap.keys().length(); j++)
//        {
//            QVariant temp = reportModel->data(reportModel->index(i,j+1));
//            row.append(QString::number(temp.toFloat(), 'f', 1));
//        }

//        mdatas.append(row);
//    }*/

//    writeCurrentSheet(first_sheet, mdatas, 1);

//    work_book->dynamicCall("SaveAs(const QString &)", QDir::toNativeSeparators(filePath)); //转换路径不可少，否则会崩溃
//    //work_book->dynamicCall("Close(Boolean)", false);  //关闭文件
//    excel.dynamicCall("Quit(void)");  //退出

    return 1;
}

//bool ReportDialog::writeCurrentSheet(QAxObject *sheet, const QList<QList<QVariant> > &cells, int rowOffset, int colOffSet)
//{
//    if(cells.size() <= 0)
//        return false;
//    if(NULL == sheet || sheet->isNull())
//        return false;
//    int row = cells.size();
//    int col = cells.at(0).size();
//    QString rangStr;
//    convertToColName(col,rangStr);
//    rangStr += QString::number(row + rowOffset);
//    rangStr = "A2:" + rangStr;
//    //qDebug()<<rangStr;
//    QAxObject *range = sheet->querySubObject("Range(const QString&)",rangStr);
//    if(NULL == range || range->isNull())
//    {
//        return false;
//    }
//    bool succ = false;
//    QVariant var;
//    castListListVariant2Variant(cells,var);
//    succ = range->setProperty("Value", var);
//    delete range;
//    return succ;
//}

void ReportDialog::castListListVariant2Variant(const QList<QList<QVariant> > &res, QVariant &var)
{
    QList<QVariant> datas;
    foreach (QList<QVariant> rowData, res) {
        QVariant vData = QVariant(rowData);
        datas.append(vData);
    }
    var = QVariant(datas);
}

///
/// \brief 把QVariant转为QList<QList<QVariant> >
/// \param var
/// \param res
///
void ReportDialog::castVariant2ListListVariant(const QVariant &var, QList<QList<QVariant> > &res)
{
    QVariantList varRows = var.toList();
    if(varRows.isEmpty())
    {
        return;
    }
    const int rowCount = varRows.size();
    QVariantList rowData;
    for(int i=0;i<rowCount;++i)
    {
        rowData = varRows[i].toList();
        res.push_back(rowData);
    }
}

///
/// \brief 把列数转换为excel的字母列号
/// \param data 大于0的数
/// \return 字母列号，如1->A 26->Z 27 AA
///
void ReportDialog::convertToColName(int data, QString &res)
{
    Q_ASSERT(data>0 && data<65535);
    int tempData = data / 26;
    if(tempData > 0)
    {
        int mode = data % 26;
        convertToColName(mode,res);
        convertToColName(tempData,res);
    }
    else
    {
        res=(to26AlphabetString(data)+res);
    }
}
///
/// \brief 数字转换为26字母
///
/// 1->A 26->Z
/// \param data
/// \return
///
QString ReportDialog::to26AlphabetString(int data)
{
    QChar ch = data + 0x40;//A对应0x41
    return QString(ch);
}

void ReportDialog::on_prePushButton_clicked()
{
    showReportAtPage(currentPage - 1);
}

void ReportDialog::on_nextPushButton_clicked()
{
    showReportAtPage(currentPage + 1);
}

void ReportDialog::on_gotoPushButton_clicked()
{
    int page = ui->pageLineEdit->text().toInt();
    if(page > 0 && page <= allPage)
    {
        showReportAtPage(page);
    }
}

void ReportDialog::on_getDailyReportButton_clicked()
{
    ui->getDailyReportButton->setEnabled(false);

    uint stime = ui->startTimeEdit->dateTime().toTime_t();
    uint etime = stime + 86400;

    DeviceNode node = Global::getFermenationNodeInfoByCname(QStringLiteral("发酵池权重温度"));
    StreamPack bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,r_DailyReport,Float,node.Offset,0,1,0,stime,etime};

    getDailyReportTcpClient->sendRequest(bpack);

    /*ui->getDailyReportButton->setEnabled(false);

    uint stime = ui->startTimeEdit->dateTime().toTime_t();
    uint etime = stime + 86400;

    reportItemMap.clear();
    reportRawData.clear();

    int deviceIndex = ui->tankComboBox->currentIndex();
    DeviceGroupInfo info = Global::getFerDeviceGroupInfo(deviceIndex);
    for(int row=0; row < ui->cbxListWidget->count(); row++)
    {
        QListWidgetItem *item = ui->cbxListWidget->item(row);
        if(item->checkState() == Qt::Checked)
        {
            DeviceNode node = Global::getFermenationNodeInfoByCname(item->text());
            reportItemMap.insert(node.Offset + 4 * (info.offset + deviceIndex - info.startIndex), item->text());
            QMap<int, float> *tv = new QMap<int, float>();
            reportRawData.insert(node.Offset + 4 * (info.offset + deviceIndex - info.startIndex),tv);
        }
    }

    if(reportItemMap.keys().length() > 0)
    {
        StreamPack bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,r_Report,String,0,(quint16)(info.offset + deviceIndex - info.startIndex),(quint16)reportItemMap.keys().length(),0,stime,etime};
        QVariant var_data = QVariant::fromValue(reportItemMap.keys());
        getDailyReportTcpClient->sendRequestWithResult(bpack, var_data, reportItemMap.keys().length()*4);
    }
    else
    {
        ui->getDailyReportButton->setEnabled(true);
    }*/
}

void ReportDialog::on_exmportDailyReportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("Excel file"),
                    qApp->applicationDirPath (),QStringLiteral("Excel Files (*.xlsx)"));

    if (fileName.isEmpty())
         return;

    //QMessageBox::about(this, QStringLiteral("您将要建立的Excel文件"), fileName );
    if (exportDailyReportExcel(fileName)>0 )//调用导出Excel方法
    {
        QMessageBox::about(this, QStringLiteral("Excel文件 导出完成"), fileName );
        QSqlDatabase::removeDatabase("excelexport");
    }
}
