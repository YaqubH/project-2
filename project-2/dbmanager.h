#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql>
#include <QList>
#include <QFile>
#include <QString>
#include <fstream>
#include <string>
#include "Credentials.h"
#include "Member.h"
#include "Purchase.h"
#include "Item.h"
using namespace std;


class dbManager
{
public:
    static dbManager& instance();
    dbManager(const dbManager&) = delete;
    void operator=(const dbManager&) = delete;

    bool VerifyLogin(const Credentials& credentials, QString& employeeType);

    bool AddMember(const Member& newMember);
    bool RemoveMember(const Member& member);
    bool MemberExists(const int memberID);
    Member GetMember(int memberID);
    QList<Member> GetAllMembers();
    QList<Member> GetUpgrades();
    QList<Member> GetDowngrades();
    QList<Member> GetExpirations(int month);
    QList<Member> GetRebates();

    bool AddPurchase(const Purchase& newPurchase);
    bool PurchaseExists(const Purchase& Purchase);
    bool PurchaseUpdateInventory(const Purchase newPurchase);
    bool PurchaseUpdateMemberTotalSpent(const Purchase newPurchase);
    bool PurchaseUpdateMemberRebate(const Purchase newPurchase);
    QList<Purchase> GetAllPurchases();

    float CalcGrossSales();
    float CalcGrossSalesByDate(QDate tDate);
    float CalcGrossSalesByMember(int buyersID);
    float CalcGrossSalesByMember(QString bName);
    float CalcGrossSalesByMember(MemberType mType);
    float CalcGrossSalesByItem(QString itemName);
    int GetTotalShoppers(MemberType mType);
    int GetTotalShoppersByDate(QDate, MemberType mType);
    int GetTotalShoppersByItem(QString itemName, MemberType mType);
    int GetTotalQuantity();
    int GetTotalQuantityByDate(QDate tDate);
    int GetTotalQuantityByMember(int buyersID);
    int GetTotalQuantityByMember(QString bName);
    int GetTotalQuantityByMember(MemberType mType);
    int GetTotalQuantityByItem(QString itemName);

    bool AddItem(const Item& newItem);
    bool RemoveItem(const Item& item);
    bool ItemExists(const Item& item) const;
    Item GetItem(QString itemName);
    float GetItemPrice(QString itemName);
    QList<Item> GetAllItems();
    QStringList GetAllItemNames();
    bool  modifyItem(const Item& item);

    bool isOpen() const;

    bool GetValidDates(QDate& earliestDate, QDate& latestDate);

private:
    dbManager();
    ~ dbManager();
    QSqlDatabase bulkdb;
};

#endif // DBMANAGER_H
