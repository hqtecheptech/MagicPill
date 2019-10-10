#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QWidget>
#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include "dataformat.h"
#include "tcpclientsocket.h"

namespace Ui {
class Environment;
}

class Environment : public QWidget
{
    Q_OBJECT

public:
    explicit Environment(QWidget *parent = 0);
    ~Environment();

private:
    Ui::Environment *ui;
    QStandardItemModel *envModel;
    QDataWidgetMapper *envMapper = Q_NULLPTR;
    void requestEnvironmentData(StreamPack requestPack);
    void setupEnvironmentalMapper(QVector<float> values);

public slots:
    void showEnviromentData(QByteArray);
private slots:
    void on_pushButton_clicked();
};

#endif // ENVIRONMENT_H
