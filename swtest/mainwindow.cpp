#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "data.h"
#include "sharehelper.h"
#include "global.h"

#include <QVariant>
#include <QTextDecoder>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);

    //从配置文件读取共享内存路径和键值，新建操作共享内存的对象.
    QString sharePath = Global::systemConfig.controlSharePath;
    int shareId = Global::systemConfig.controlShareKey;
    key_t shareKey = ShareHelper::GenKey(sharePath.toLatin1(), shareId);
    ctrShare = new ShareHelper(shareKey, sizeof(Ctr_Block));

    sharePath = Global::systemConfig.dataSharePath;
    shareId = Global::systemConfig.dataShareKey;
    shareKey = ShareHelper::GenKey(sharePath.toLatin1(), shareId);
    dbShare = new ShareHelper(shareKey, sizeof(Plc_Db));

    //创建定时器，连接定时回调函数，启动定时器（1秒1次）
    timer1 = new QTimer(this);
    connect(timer1, SIGNAL(timeout()), this, SLOT(refreshData()));
    timer1->start(1000);

    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(switchState()));
    timer2->start(1000);

    timer3 = new QTimer(this);
    connect(timer3, SIGNAL(timeout()), this, SLOT(autoUpdateValue()));
    timer3->start(5000);

    // 添加多个文本框到数组里
    dataVector.append(ui->arrayLineEdit_1);
    dataVector.append(ui->arrayLineEdit_2);
    dataVector.append(ui->arrayLineEdit_3);
    dataVector.append(ui->arrayLineEdit_4);
    dataVector.append(ui->arrayLineEdit_5);
    dataVector.append(ui->arrayLineEdit_6);
    dataVector.append(ui->arrayLineEdit_7);
    dataVector.append(ui->arrayLineEdit_8);
}

MainWindow::~MainWindow()
{

}

// 定时更新界面显示数据.
void MainWindow::refreshData()
{
    Plc_Db db;
    dbShare->GetShardMemory(&db, sizeof(Plc_Db));

    // ----------------------------------------------------
    // 显示数据，本函数其他地方不需要改动，只需要修改虚线内的代码即可。
    ui->dataLineEdit->setText(QString::number(db.b_data[ui->indexInputEdit->text().toInt()]));
    // 如果输入格式正确，显示修改后的值
    if(isFormatCorrect)
    {
        ui->outputEdit_1->setText(QString::number(db.b_data[ui->indexInputEdit->text().toInt()]));
    }
    // 使用循环显示多个值，值的个数为添加到数组的文本框数量
    for(int i=0; i < dataVector.length(); i++)
    {
        dataVector.at(i)->setText(QString::number(db.i_data[i]));
    }
    // 显示自动更新的值
    ui->autoUpdateLineEdit->setText(QString::number(db.f_data[4]));
    //ui->autoUpdateLineEdit->setText(QString::number(db.f_data[11]));
    ui->autoUpdateLineEdit_2->setText(QString::number(db.f_data[19]));

    Ctr_Block cb;
    ctrShare->GetShardMemory(&cb, sizeof(Ctr_Block));
    // 显示共享内存可以写状态值
    ui->statusLineEdit->setText(QString::number(cb.fromPru[0]));
    // ----------------------------------------------------
}

// 切换界面元素状态，改变颜色等
void MainWindow::switchState()
{
    if(stateFlag)
    {
        ui->dataLineEdit->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 255, 255)");
    }
    else
    {
        ui->dataLineEdit->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 255, 0)");
    }

    stateFlag = !stateFlag;
}

// 定时自动更新值
void MainWindow::autoUpdateValue()
{
    Plc_Db db;
    Ctr_Block ctrBlock;

    // 修改数据共享内存前必须先读取状态共享内存，检查状态共享内存[0]位是否为1，否则不能修改数据共享内存
    ctrShare->LockShare();
    ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
    //ctrShare->UnlockShare();
    if(ctrBlock.fromPru[0] == 1)
    {
        dbShare->LockShare();
        dbShare->GetShardMemory(&db, sizeof(Plc_Db));

        // ------------------------------------------------------------------------------------------
        //修改数据，本函数其他地方不需要改动，只需要修改虚线内的代码即可。下面代码表示，每次点击按钮，在原值基础上加1.5。
        //db.dw_data[8] += 1;
        //db.i_data[x] += 1;
        //db.f_data[x] += 1.5;
        //db.w_data[x] += 1;
        if(db.b_data[32]==1)
        {
           db.b_data[32] = 0;
        }
        else
        {
           db.b_data[32] = 1;
        }

        /*if(db.b_data[13]==1)
        {
           db.b_data[13] = 0;
        }
        else
        {
           db.b_data[13] = 1;
        }

        if(db.b_data[17]==1)
        {
           db.b_data[17] = 0;
        }
        else
        {
           db.b_data[17] = 1;
        }

        if(db.b_data[37]==1)
        {
           db.b_data[37] = 0;
        }
        else
        {
           db.b_data[37] = 1;
        }

        if(db.b_data[114]==1)
        {
           db.b_data[114] = 0;
        }
        else
        {
           db.b_data[114] = 1;
        }*/

        db.b_data[8] = 1;
        db.b_data[11] = 1;

        //db.dw_data[0] = db.dw_data[0] - 1;
        //db.dw_data[1] = db.dw_data[1] - 1;

        db.w_data[0] = qrand() % 50;
        db.w_data[1] = qrand() % 100;

        db.f_data[0] = qrand() % 20 + 10;
        db.f_data[1] = qrand() % 450;
        db.f_data[2] = qrand() % 500;
        db.f_data[3] = qrand() % 1000;
        db.f_data[4] = qrand() % 2000;
        db.f_data[5] = qrand() % 3000;
        db.f_data[6] = qrand() % 4000;

        db.f_data[10] = qrand() % 150;
        db.f_data[21] = qrand() % 150;

        //db.f_data[0] = qrand() % 100;
        db.f_data[9] = qrand() % 300;

        db.f_data[15] = qrand() % 300;
        db.w_data[2] = qrand() % 180;

        db.f_data[16] = qrand() % 300;
        db.w_data[3] = qrand() % 180;

        db.f_data[13] = qrand() % 300;
        //db.dw_data[0] = qrand() % 25000;

        db.f_data[14] = qrand() % 300;
        //db.dw_data[1] = qrand() % 25000;

        //db.f_data[11] = qrand() % 150;
        db.f_data[4] = qrand() % 150;
        db.f_data[19] = qrand() % 150;

        db.f_data[12] = qrand() % 150;
        db.f_data[20] = qrand() % 150;

        db.f_data[87] = qrand() % 1000;
        db.f_data[88] = qrand() % 1000;
        db.f_data[89] = qrand() % 1000;
        db.f_data[90] = qrand() % 1000;
        // ------------------------------------------------------------------------------------------

        dbShare->SetSharedMemory(&db, sizeof(Plc_Db));
        dbShare->UnlockShare();
    }

    ctrBlock.toPru[0] = 1;
    ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
    ctrShare->UnlockShare();
}

// 点击修改数据
void MainWindow::on_modifyButton_clicked()
{
    Plc_Db db;
    Ctr_Block ctrBlock;

    // 修改数据共享内存前必须先读取状态共享内存，检查状态共享内存[0]位是否为1，否则不能修改数据共享内存
    ctrShare->LockShare();
    ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
    //ctrShare->UnlockShare();
    if(ctrBlock.fromPru[0] == 1)
    {
        dbShare->LockShare();
        dbShare->GetShardMemory(&db, sizeof(Plc_Db));

        // ------------------------------------------------------------------------------------------
        //修改数据，本函数其他地方不需要改动，只需要修改虚线内的代码即可。下面代码表示，每次点击按钮，在原值基础上加1.5。
        //db.f_data[5] += 1.5;
        //db.i_data[x] += 1;
        //db.dw_data[x] += 1;
        //db.w_data[x] += 1;
        if(db.b_data[ui->indexInputEdit->text().toInt()] == 1)
        {
            db.b_data[ui->indexInputEdit->text().toInt()] = 0;
        }
        else
        {
            db.b_data[ui->indexInputEdit->text().toInt()] = 1;
        }
        // ------------------------------------------------------------------------------------------

        dbShare->SetSharedMemory(&db, sizeof(Plc_Db));
        dbShare->UnlockShare();
    }

    ctrBlock.toPru[0] = 1;
    ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
    ctrShare->UnlockShare();
}

// 修改数据为输入的指定值
void MainWindow::on_setDataButton_clicked()
{
    // 取得输入文本(去掉收尾空格)，转换为相应数字类型
    QString temp = ui->inputEdit_1->text().trimmed();
    bool isOk;
    //float inputData = temp.toFloat(&isOk);

    // 文本转化为各种数据类型，参考一下注释掉的代码
    //int inputData = temp.toInt(&isOk);
    //uint32_t inputData = temp.toUInt(&isOk);
    //uint16_t inputData = temp.toUShort(&isOk);
    uint8_t inputData;
    if(temp == "0")
    {
        isOk = true;
        inputData = 0;
    }
    else if(temp == "1")
    {
        isOk = true;
        inputData = 1;
    }
    else
    {
        isOk = false;
    }

    if(isOk)
    {
        Plc_Db db;
        Ctr_Block ctrBlock;

        // 修改数据共享内存前必须先读取状态共享内存，检查状态共享内存[0]位是否为1，否则不能修改数据共享内存
        ctrShare->LockShare();
        ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
        //ctrShare->UnlockShare();
        if(ctrBlock.fromPru[0] == 1)
        {
            dbShare->LockShare();
            dbShare->GetShardMemory(&db, sizeof(Plc_Db));

            // ------------------------------------------------------------------------------------------
            //修改数据，本函数其他地方不需要改动，只需要修改虚线内的代码即可。下面代码表示，每次点击按钮，在原值基础上加1.5。
            //db.f_data[6] = inputData;
            //db.i_data[x] = inputData;
            //db.dw_data[ui->indexInputEdit->text().toInt()] = inputData;
            //db.w_data[x] = inputData;
            db.b_data[ui->indexInputEdit->text().toInt()] = inputData;
            // ------------------------------------------------------------------------------------------

            dbShare->SetSharedMemory(&db, sizeof(Plc_Db));
            dbShare->UnlockShare();
        }
        ctrBlock.toPru[0] = 1;
        ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
        ctrShare->UnlockShare();

        // 如果格式正确，设置背景颜色为蓝色。
        isFormatCorrect = true;
        ui->outputEdit_1->setStyleSheet("border: 0px;background: transparent;background-color: rgb(0, 255, 255)");
    }
    else
    {
        // 如果格式正确，设置背景颜色为红色。
        isFormatCorrect = false;
        ui->outputEdit_1->setStyleSheet("border: 0px;background: transparent;background-color: rgb(255, 0, 0)");
        ui->outputEdit_1->setText("格式错误!");
    }
}

// 批量修改多个数据
void MainWindow::on_batchModifyButton_clicked()
{
    Plc_Db db;
    Ctr_Block ctrBlock;

    // 修改数据共享内存前必须先读取状态共享内存，检查状态共享内存[0]位是否为1，否则不能修改数据共享内存
    ctrShare->LockShare();
    ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
    //ctrShare->UnlockShare();
    if(ctrBlock.fromPru[0] == 1)
    {
        dbShare->LockShare();
        dbShare->GetShardMemory(&db, sizeof(Plc_Db));

        // ------------------------------------------------------------------------------------------
        //修改数据，本函数其他地方不需要改动，只需要修改虚线内的代码即可。下面代码表示，每次点击按钮，在原值基础上加1.5。
        for(int i=0; i < dataVector.length(); i++)
        {
            db.i_data[i] += 3;
        }
        // ------------------------------------------------------------------------------------------

        dbShare->SetSharedMemory(&db, sizeof(Plc_Db));
        dbShare->UnlockShare();
    }
    ctrBlock.toPru[0] = 1;
    ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
    ctrShare->UnlockShare();
}

void MainWindow::on_setCtrlButton_clicked()
{
    Ctr_Block ctrBlock;

    // 修改状态共享内存
    ctrShare->LockShare();
    ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
    if(ctrBlock.fromPru[0] == 1)
    {
        ctrBlock.fromPru[0] = 0;
    }
    else
    {
        ctrBlock.fromPru[0] = 1;
    }
    ctrShare->SetSharedMemory(&ctrBlock, sizeof(Ctr_Block));
    ctrShare->UnlockShare();
}

void MainWindow::on_pushButton_clicked()
{
    Plc_Db db;
    Ctr_Block ctrBlock;

    // 修改数据共享内存前必须先读取状态共享内存，检查状态共享内存[0]位是否为1，否则不能修改数据共享内存
    ctrShare->LockShare();
    ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
    //ctrShare->UnlockShare();
    if(ctrBlock.fromPru[0] == 1)
    {
        dbShare->LockShare();
        dbShare->GetShardMemory(&db, sizeof(Plc_Db));

        // ------------------------------------------------------------------------------------------
        //修改数据，本函数其他地方不需要改动，只需要修改虚线内的代码即可。下面代码表示，每次点击按钮，在原值基础上加1.5。
        //db.dw_data[8] += 1;
        //db.i_data[x] += 1;
        //db.f_data[x] += 1.5;
        //db.w_data[x] += 1;
        db.b_data[0] = 0;
        db.dw_data[0] = -1;
        db.dw_data[1] = 500;
        // ------------------------------------------------------------------------------------------

        dbShare->SetSharedMemory(&db, sizeof(Plc_Db));
        dbShare->UnlockShare();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    Plc_Db db;
    Ctr_Block ctrBlock;

    // 修改数据共享内存前必须先读取状态共享内存，检查状态共享内存[0]位是否为1，否则不能修改数据共享内存
    ctrShare->LockShare();
    ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
    ctrShare->UnlockShare();
    if(ctrBlock.fromPru[0] == 1)
    {
        dbShare->LockShare();
        dbShare->GetShardMemory(&db, sizeof(Plc_Db));

        // ------------------------------------------------------------------------------------------
        //修改数据，本函数其他地方不需要改动，只需要修改虚线内的代码即可。下面代码表示，每次点击按钮，在原值基础上加1.5。
        //db.dw_data[8] += 1;
        //db.i_data[x] += 1;
        //db.f_data[x] += 1.5;
        //db.w_data[x] += 1;
        db.b_data[0] = 1;
        db.dw_data[0] = 600;
        db.dw_data[1] = -1;
        // ------------------------------------------------------------------------------------------

        dbShare->SetSharedMemory(&db, sizeof(Plc_Db));
        dbShare->UnlockShare();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    Plc_Db db;
    Ctr_Block ctrBlock;

    // 修改数据共享内存前必须先读取状态共享内存，检查状态共享内存[0]位是否为1，否则不能修改数据共享内存
    ctrShare->LockShare();
    ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
    ctrShare->UnlockShare();
    if(ctrBlock.fromPru[0] == 1)
    {
        dbShare->LockShare();
        dbShare->GetShardMemory(&db, sizeof(Plc_Db));

        // ------------------------------------------------------------------------------------------
        //修改数据，本函数其他地方不需要改动，只需要修改虚线内的代码即可。下面代码表示，每次点击按钮，在原值基础上加1.5。
        //db.dw_data[8] += 1;
        //db.i_data[x] += 1;
        //db.f_data[x] += 1.5;
        //db.w_data[x] += 1;
        //db.b_data[0] = 1;
        //db.b_data[88] = 0;
        //db.b_data[89] = 1;
        db.dw_data[0] = -1;
        // ------------------------------------------------------------------------------------------

        dbShare->SetSharedMemory(&db, sizeof(Plc_Db));
        dbShare->UnlockShare();
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    Plc_Db db;
    Ctr_Block ctrBlock;

    // 修改数据共享内存前必须先读取状态共享内存，检查状态共享内存[0]位是否为1，否则不能修改数据共享内存
    ctrShare->LockShare();
    ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
    ctrShare->UnlockShare();
    if(ctrBlock.fromPru[0] == 1)
    {
        dbShare->LockShare();
        dbShare->GetShardMemory(&db, sizeof(Plc_Db));

        // ------------------------------------------------------------------------------------------
        //修改数据，本函数其他地方不需要改动，只需要修改虚线内的代码即可。下面代码表示，每次点击按钮，在原值基础上加1.5。
        //db.dw_data[8] += 1;
        //db.i_data[x] += 1;
        //db.f_data[x] += 1.5;
        //db.w_data[x] += 1;
        //db.b_data[0] = 0;
        //db.b_data[88] = 1;
        //db.b_data[89] = 1;
        db.dw_data[1] = -1;
        // ------------------------------------------------------------------------------------------

        dbShare->SetSharedMemory(&db, sizeof(Plc_Db));
        dbShare->UnlockShare();
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    Plc_Db db;
    Ctr_Block ctrBlock;

    // 修改数据共享内存前必须先读取状态共享内存，检查状态共享内存[0]位是否为1，否则不能修改数据共享内存
    ctrShare->LockShare();
    ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
    ctrShare->UnlockShare();
    if(ctrBlock.fromPru[0] == 1)
    {
        dbShare->LockShare();
        dbShare->GetShardMemory(&db, sizeof(Plc_Db));

        // ------------------------------------------------------------------------------------------
        //修改数据，本函数其他地方不需要改动，只需要修改虚线内的代码即可。下面代码表示，每次点击按钮，在原值基础上加1.5。
        //db.dw_data[8] += 1;
        //db.i_data[x] += 1;
        //db.f_data[x] += 1.5;
        //db.w_data[x] += 1;
        db.b_data[0] = 0;
        db.b_data[88] = 0;
        db.b_data[89] = 1;
        // ------------------------------------------------------------------------------------------

        dbShare->SetSharedMemory(&db, sizeof(Plc_Db));
        dbShare->UnlockShare();
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    Plc_Db db;
    Ctr_Block ctrBlock;

    // 修改数据共享内存前必须先读取状态共享内存，检查状态共享内存[0]位是否为1，否则不能修改数据共享内存
    ctrShare->LockShare();
    ctrShare->GetShardMemory(&ctrBlock, sizeof(Ctr_Block));
    ctrShare->UnlockShare();
    if(ctrBlock.fromPru[0] == 1)
    {
        dbShare->LockShare();
        dbShare->GetShardMemory(&db, sizeof(Plc_Db));

        // ------------------------------------------------------------------------------------------
        //修改数据，本函数其他地方不需要改动，只需要修改虚线内的代码即可。下面代码表示，每次点击按钮，在原值基础上加1.5。
        //db.dw_data[8] += 1;
        //db.i_data[x] += 1;
        //db.f_data[x] += 1.5;
        //db.w_data[x] += 1;
        db.b_data[0] = 0;
        db.b_data[88] = 0;
        db.b_data[89] = 0;
        // ------------------------------------------------------------------------------------------

        dbShare->SetSharedMemory(&db, sizeof(Plc_Db));
        dbShare->UnlockShare();
    }
}
