#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include "Models.h"
#include "dbManager.h"
#include "AddNewItem.h"
#include "AddNewMembers.h"

namespace Ui {
class Admin;
}

class Admin : public QDialog
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    ~Admin();

signals:
    void sendSelectedID(int id);
    void sendSelectedName(QString itemName);
    void sendModel(MemberModel *mod);
    void sendModel(ItemModel *mod);

private slots:
    void on_PushButton_DisplayMembers_clicked();
    void on_PushButton_DisplayInventory_clicked();
    void on_PushButton_DisplayUpAndDown_clicked();

    void on_PushButton_BackToMain_clicked();
    void on_PushButton_BackToHome_clicked();
    void on_PushButton_BackToHome2_clicked();

    void on_PushButton_AddMember_clicked();
    void on_PushButton_RemoveMember_clicked();
    void on_membersTable_clicked(const QModelIndex &index);

    void on_PushButton_AddItem_clicked();
    void on_PushButton_RemoveItem_clicked();
     void on_itemsTable_clicked(const QModelIndex &index);

    void removeMember();
    void removeItem();
    void updateItemView();
    void updateMemberView();

    void on_PushButton_BackToHome3_clicked();

    void on_PushButton_ModifyItem_clicked();

private:
    Ui::Admin *ui;
    MemberModel *mModel;
    ItemModel *iModel;
    UpgradeModel *uModel;
    DowngradeModel *dModel;
    int selectedID;
    QString selectedName;
};

#endif // ADMIN_H
