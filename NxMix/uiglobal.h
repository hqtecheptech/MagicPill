#ifndef UIGLOBAL_H
#define UIGLOBAL_H

#include <QStandardItemModel>

class UiGlobal
{
public:
    UiGlobal();

    static QStandardItemModel *alertsModel;
    static QStandardItemModel *simpleAlertsModel;
    static QStandardItemModel *loginHistoryModel;
};

#endif // UIGLOBAL_H
