#include "environment.h"
#include "ui_environment.h"
#include "dataformat.h"
#include "global.h"
#include "tcpclientsocket.h"

Environment::Environment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Environment)
{
    ui->setupUi(this);
}

Environment::~Environment()
{
    delete ui;
}

void Environment::requestEnvironmentData(StreamPack requestPack)
{
    TcpClientSocket *tcpClient = new TcpClientSocket(this);
    QObject::connect(tcpClient,SIGNAL(updateClients(QByteArray)),this,SLOT(showEnviromentData(QByteArray)));
    tcpClient->sendRequest(requestPack);
}

void Environment::showEnviromentData(QByteArray data)
{
    StreamPack bDevice;
    memcpy(&bDevice,data,sizeof(bDevice));

    QString msg;
    if(bDevice.bErrorCode==1)
    {
        QByteArray byteValues = data.mid(sizeof(bDevice));
        QVector<float> realValues;
        for(quint16 i=0; i<bDevice.bDataLength; ++i)
        {
            float temp = 0;
            QByteArray value = byteValues.mid(i*sizeof(temp),sizeof(temp));
            memcpy(&temp,value,sizeof(temp));
            realValues.append(temp);
            msg.append(QString::number(temp));
            msg.append(",");
        }
        setupEnvironmentalMapper(realValues);
    }

    ui->textEdit->setText(msg);
}

// Todo
void Environment::setupEnvironmentalMapper(QVector<float> values)
{
    /*int index=0;
    for (int row=0; row < envModel->rowCount(); ++row) {
        for(int tn = 0; tn<Global::deviceInfo.Enviroment_Num; ++tn)
        {
            if(tn % Global::deviceInfo.Enviroment_Params_Number == varIndex)
            {
                QStandardItem *item = new QStandardItem(QString::number(values[tn]));
                envModel->setItem(row, index++, item);
            }
        }
    }
    envMapper = new QDataWidgetMapper(this);
    envMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    envMapper->toFirst();*/
}

void Environment::on_pushButton_clicked()
{
    QDateTime currentdt = QDateTime::currentDateTime();
    StreamPack bpack;
    uint stime =currentdt.toTime_t();
    uint etime =currentdt.toTime_t();
    bpack = {sizeof(StreamPack),1,(quint16)Global::ferGroupShow,1,6,(quint16)Global::ferDeviceInfo.Enviroment_Address,0,(quint16)Global::ferDeviceInfo.Enviroment_Num,0,stime,etime};
    bpack.bStartTime =stime;
    bpack.bEndTime =etime;
    requestEnvironmentData(bpack);
}
