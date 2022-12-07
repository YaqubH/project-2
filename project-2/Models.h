#ifndef MODELS_H
#define MODELS_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QList>
#include <QDebug>
#include <QObject>
#include "Member.h"
#include "Item.h"
#include "Purchase.h"
#include "dbManager.h"

class MemberModel : public QAbstractTableModel
{
public:
    MemberModel(QList<Member> memberList, QObject *parent = 0);

    void setList(const QList<Member> memberList);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QList<Member> modMemberList;
};


class ItemModel : public QAbstractTableModel
{
public:
    ItemModel(QList<Item> itemList, QObject *parent = 0);

    void setList(const QList<Item> itemList);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    QList<Item> modItemList;
};

class PurchaseModel : public QAbstractTableModel
{
public:
    PurchaseModel(QList<Purchase> transList, QObject *parent = 0);

    void setList(const QList<Purchase> transList);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
private:
    QList<Purchase> modTransList;
};

enum Filter {NO_FILTER, DATE, ITEM, MEMBER_ID, MEMBER_NAME, MEMBERSHIP_TYPE};


class ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ProxyModel(QObject* parent = 0);
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public slots:
    void setPurchaseDate(QDate transDate);
    void setMemberID(int memberID);
    void setMemberName(QString memberName);
    void setItemName(QString itemName);
    void setMemberType(MemberType memberType);
    void resetFilter();

signals:
    void updateTotal(float price);

private:
    Filter filterType;
    QDate tDate;
    int bID;
    QString iName;
    QString bName;
    MemberType mType;
};


class UpgradeModel : public QAbstractTableModel
{
public:
    UpgradeModel(QList<Member> upgradeList, QObject *parent = 0);

    void setList(const QList<Member> upgradeList);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QList<Member> modUpgradeList;
};


class DowngradeModel : public QAbstractTableModel
{
public:
    DowngradeModel(QList<Member> downgradeList, QObject *parent = 0);

    void setList(const QList<Member> downgradeList);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QList<Member> modDowngradeList;
};


class ExpModel : public QAbstractTableModel
{
public:
    ExpModel(QList<Member> expList, QObject *parent = 0);

    void setList(const QList<Member> expList);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QList<Member> modExpList;
};


class RebateModel : public QAbstractTableModel
{
public:
    RebateModel(QList<Member> rebateList, QObject *parent = 0);

    void setList(const QList<Member> rebateList);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QList<Member> modRebateList;
};

#endif // MODELS_H
