#ifndef RENHUAITABWIDGET_H
#define RENHUAITABWIDGET_H

#include <QTabWidget>

#include "renhuaifercontroltabpagewidget.h"
#include "deocontroltabpagewidget.h"
#include "xinyifercontroltabwidget.h"

namespace Ui {
class RenhuaiTabWidget;
}

class RenhuaiTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit RenhuaiTabWidget(QWidget *parent = 0);
    ~RenhuaiTabWidget();

    //RenhuaiFerControlTabPageWidget *ferControlTabPageWidget;
    XinyiFerControlTabWidget *ferControlTabPageWidget;
    DeoControlTabPageWidget *deoControlTabPageWidget;

private:
    Ui::RenhuaiTabWidget *ui;

private slots:
    void handleDataChange();
    void handleServerConnectionChanged(bool isServerConnected);
    void handleFerDataUpdate(QSet<int>, QMap<float,QString> dataMap);
    void handleDeoDataUpdate(QSet<int>, QMap<float,QString> dataMap);
    void alertUpdate(QString alert);

signals:
    void dataChanged();
    void serverConnectionChanged(bool isServerConnected);
    void ferDataUpdate(QSet<int>, QMap<float,QString>);
    void deoDataUpdate(QSet<int>, QMap<float,QString> dataMap);
    void alertReceived(QString alert);
};

#endif // RENHUAITABWIDGET_H
