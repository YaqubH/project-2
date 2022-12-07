#ifndef MANAGER_H
#define MANAGER_H

#include <QDialog>
#include "dbManager.h"
#include "Models.h"

namespace Ui {
class Manager;
}

class Manager : public QDialog
{
    Q_OBJECT

public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();

private slots:
    void on_PushButton_DisplaySalesInfo_clicked();
    void on_PushButton_DisplayMemberInfo_clicked();
    void on_PushButton_BackToMain_clicked();
    void on_PushButton_DateFilter_clicked();
    void on_PushButton_ItemFilter_clicked();
    void on_PushButton_MemberIDFilter_clicked();
    void on_PushButton_MemberName_clicked();
    void on_PushButton_ResetFilter_clicked();
    void updateSalesTotals(float);
    void updateMembersTotals(int, int);
    void updateQuantityTotals(int);

    void on_PushButton_MemberExpiration_clicked();

    void on_PushButton_MemberType_clicked();

    void on_PushButton_BackToHome_clicked();

    void on_PushButton_BackToHome2_clicked();

private:
    Ui::Manager *ui;
    PurchaseModel *tModel;
    ProxyModel *pModel;
    ProxyModel *pRModel;
    ExpModel *eModel;
    RebateModel *rModel;
    QDate minDate;
    QDate maxDate;
};

#endif // MANAGER_H
