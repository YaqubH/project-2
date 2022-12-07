#include "Models.h"

MemberModel::MemberModel(QList<Member> memberList, QObject *parent):
    QAbstractTableModel(parent)
{
    modMemberList = memberList;
}

void MemberModel::setList(const QList<Member> memberList)
{
    beginResetModel();
    modMemberList = memberList;
    endResetModel();
}

int MemberModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return modMemberList.length();
}

int MemberModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant MemberModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    if(index.row() >= modMemberList.size() || index.row() < 0)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        Member member = modMemberList.at(index.row());

        switch(index.column())
        {
        case 0:
            return member.GetMemberName();
            break;
        case 1:
            return member.GetID();
            break;
        case 2:
            return member.GetMembershipTypeString();
            break;
        case 3:
            return member.GetExpirationDateString();
            break;
        case 4:
            return ("$" +QString::number(member.GetTotalSpent(), 'f', 2));
            break;
        case 5:
            return ("$" +QString::number(member.GetRebate(), 'f', 2));
            break;
        default:
            return QVariant();
            break;
        }
    }
    return QVariant();
}

QVariant MemberModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return tr("Name");
            break;
        case 1:
            return tr("Member ID");
            break;
        case 2:
            return tr("Member Type");
            break;
        case 3:
            return tr("Expiration Date");
            break;
        case 4:
            return tr("Total Spent");
            break;
        case 5:
            return tr("Rebate");
            break;
        }
    }
    return QVariant();
}

// ITEM MODEL
ItemModel::ItemModel(QList<Item> itemList, QObject *parent):
    QAbstractTableModel(parent)
{
    modItemList = itemList;
}

void ItemModel::setList(const QList<Item> itemList)
{
    beginResetModel();
    modItemList = itemList;
    endResetModel();
}

int ItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return modItemList.length();
}

int ItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant ItemModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    if(index.row() >= modItemList.size() || index.row() < 0)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        Item item = modItemList.at(index.row());

        switch(index.column())
        {
        case 0:
            return item.GetItemName();
            break;
        case 1:
            return ("$" +QString::number(item.GetItemPrice(), 'f', 2));
            break;
        default:
            return QVariant();
            break;
        }
    }
    return QVariant();
}

QVariant ItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return tr("Item Name");
            break;
        case 1:
            return tr("Item Price");
            break;
        }
    }
    return QVariant();
}

// Purchase MODEL
PurchaseModel::PurchaseModel(QList<Purchase> transList, QObject *parent):
    QAbstractTableModel(parent)
{
    modTransList = transList;
}

void PurchaseModel::setList(const QList<Purchase> transList)
{
    modTransList = transList;
}

int PurchaseModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return modTransList.length();
}

int PurchaseModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 8;
}

QVariant PurchaseModel::data(const QModelIndex &index, int role) const
{
    const Purchase& trans = modTransList.at(index.row());
    const Member& member = dbManager::instance().GetMember(trans.GetMemberID());

    if(!index.isValid())
    {
        return QVariant();
    }
    if(index.row() >= modTransList.size() || index.row() < 0)
    {
        return QVariant();
    }
    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 0:
            return trans.GetPurchaseDate().toString("MM/dd/yyyy");
            break;
        case 1:
            return trans.GetMemberID();
            break;
        case 2:
            return member.GetMemberName();
            break;
        case 3:
            return member.GetMembershipTypeString();
            break;
        case 4:
            return trans.GetItemName();
            break;
        case 5:
            return trans.GetQuantityPurchased();
            break;
        case 6:
            return ("$" +QString::number(trans.GetPurchaseSubTotal(), 'f', 2));
            break;
        case 7:
            return ("$" +QString::number(trans.GetPurchaseTotal(), 'f', 2));
            break;
        default:
            return QVariant();
            break;
        }
    }
    if(role == Qt::UserRole)
    {
        switch(index.column())
        {
        case 0:
            return trans.GetPurchaseDate();
            break;
        case 1:
            return trans.GetMemberID();
            break;
        case 2:
            return member.GetMemberName();
            break;
        case 3:
            return member.GetMembershipTypeString();
            break;
        case 4:
            return trans.GetItemName();
            break;
        case 5:
            return trans.GetQuantityPurchased();
            break;
        case 6:
            return trans.GetPurchaseSubTotal();
            break;
        case 7:
            return trans.GetPurchaseTotal();
            break;
        default:
            return QVariant();
            break;
        }
    }
    return QVariant();
}

QVariant PurchaseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return tr("Purchase Date");
            break;
        case 1:
            return tr("Member's ID");
            break;
        case 2:
            return tr("Member's Name");
            break;
        case 3:
            return tr("Membership Type");
            break;
        case 4:
            return tr("Item Purchased");
            break;
        case 5:
            return tr("Qty Purchased");
            break;
        case 6:
            return tr("Subtotal");
            break;
        case 7:
            return tr("Total");
            break;
        default:
            return QVariant();
            break;
        }
    }
    return QVariant();
}

ProxyModel::ProxyModel(QObject *parent): QSortFilterProxyModel(parent),
    filterType(NO_FILTER), tDate(1900, 1, 1), bID(0), iName(""), bName(""), mType(REGULAR)
{

}

void ProxyModel::setPurchaseDate(QDate transDate)
{
    filterType = DATE;
    if(tDate != transDate)
    {
        tDate = transDate;
    }
    invalidateFilter();
}

void ProxyModel::setMemberID(int memberID)
{
    filterType = MEMBER_ID;
    if(bID != memberID)
    {
        bID = memberID;
    }
    invalidateFilter();
}

void ProxyModel::setMemberName(QString memberName)
{
    filterType = MEMBER_NAME;
    if(bName != memberName)
        bName = memberName;
    invalidateFilter();
}

void ProxyModel::setItemName(QString itemName)
{
    filterType = ITEM;
    if(iName != itemName)
    {
        iName = itemName;
    }
    invalidateFilter();
}

void ProxyModel::setMemberType(MemberType memberType)
{
    filterType = MEMBERSHIP_TYPE;
    if(mType != memberType)
        mType = memberType;
    invalidateFilter();
}

void ProxyModel::resetFilter()
{
    filterType = NO_FILTER;
    invalidateFilter();
}

bool ProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex indDate;
    QModelIndex indID;
    QModelIndex indBName;
    QModelIndex indIName;
    QModelIndex indMType;
    QString mTypeString;

    indDate = sourceModel()->index(source_row, 0, source_parent);
    indID = sourceModel()->index(source_row, 1, source_parent);
    indBName = sourceModel()->index(source_row, 2, source_parent);
    indMType = sourceModel()->index(source_row, 3, source_parent);
    indIName = sourceModel()->index(source_row, 4, source_parent);
    if(mType == REGULAR)
        mTypeString = "Regular";
    else
        mTypeString = "Executive";

    switch(filterType)
    {
    case DATE:
        if(QDate::fromString(sourceModel()->data(indDate).toString(), "MM/dd/yyyy") != tDate)
            return false;
        break;
    case MEMBER_ID:
        if(sourceModel()->data(indID).toInt() != bID)
            return false;
        break;
    case ITEM:
        if(sourceModel()->data(indIName).toString().toLower() != iName.toLower())
            return false;
        break;
    case MEMBER_NAME:
        if(sourceModel()->data(indBName).toString().toLower() != bName.toLower())
            return false;
        break;
    case MEMBERSHIP_TYPE:
        if(sourceModel()->data(indMType).toString() != mTypeString)
            return false;
        break;
    case NO_FILTER:
        return true;
        break;
    default:
        return true;
        break;
    }

    return true;
}

QVariant ProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return sourceModel()->headerData(section, orientation, role);
}

UpgradeModel::UpgradeModel(QList<Member> upgradeList, QObject *parent):
    QAbstractTableModel(parent)
{
    modUpgradeList = upgradeList;
}

void UpgradeModel::setList(const QList<Member> upgradeList)
{
    beginResetModel();
    modUpgradeList = upgradeList;
    endResetModel();
}

int UpgradeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return modUpgradeList.length();
}

int UpgradeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant UpgradeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    if(index.row() >= modUpgradeList.size() || index.row() < 0)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        Member member = modUpgradeList.at(index.row());

        switch(index.column())
        {
        case 0:
            return member.GetMemberName();
            break;
        case 1:
            return member.GetID();
            break;
        default:
            return QVariant();
            break;
        }
    }
    return QVariant();
}

QVariant UpgradeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return tr("Name");
            break;
        case 1:
            return tr("Member ID");
            break;
        }
    }
    return QVariant();
}

DowngradeModel::DowngradeModel(QList<Member> downgradeList, QObject *parent):
    QAbstractTableModel(parent)
{
    modDowngradeList = downgradeList;
}

void DowngradeModel::setList(const QList<Member> downgradeList)
{
    beginResetModel();
    modDowngradeList = downgradeList;
    endResetModel();
}

int DowngradeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return modDowngradeList.length();
}

int DowngradeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant DowngradeModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    if(index.row() >= modDowngradeList.size() || index.row() < 0)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        Member member = modDowngradeList.at(index.row());

        switch(index.column())
        {
        case 0:
            return member.GetMemberName();
            break;
        case 1:
            return member.GetID();
            break;
        default:
            return QVariant();
            break;
        }
    }
    return QVariant();
}

QVariant DowngradeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return tr("Name");
            break;
        case 1:
            return tr("Member ID");
            break;
        }
    }
    return QVariant();
}

ExpModel::ExpModel(QList<Member> expList, QObject *parent):
    QAbstractTableModel(parent)
{
    modExpList = expList;
}

void ExpModel::setList(const QList<Member> expList)
{
    beginResetModel();
    modExpList = expList;
    endResetModel();
}

int ExpModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return modExpList.length();
}

int ExpModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant ExpModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    if(index.row() >= modExpList.size() || index.row() < 0)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {
        Member member = modExpList.at(index.row());

        switch(index.column())
        {
        case 0:
            return member.GetMemberName();
            break;
        case 1:
            return member.GetID();
            break;
        case 2:
            return member.GetRenewCost();
            break;
        default:
            return QVariant();
            break;
        }
    }
    return QVariant();
}

QVariant ExpModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return tr("Name");
            break;
        case 1:
            return tr("Member ID");
            break;
        case 2:
            return tr("Renewal Cost");
            break;
        }
    }
    return QVariant();
}

RebateModel::RebateModel(QList<Member> rebateList, QObject *parent):
    QAbstractTableModel(parent)
{
    modRebateList = rebateList;
}

void RebateModel::setList(const QList<Member> rebateList)
{
    beginResetModel();
    modRebateList = rebateList;
    endResetModel();
}

int RebateModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return modRebateList.length();
}

int RebateModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant RebateModel::data(const QModelIndex &index, int role) const
{
    const Member& member = modRebateList.at(index.row());

    if(!index.isValid())
    {
        return QVariant();
    }
    if(index.row() >= modRebateList.size() || index.row() < 0)
    {
        return QVariant();
    }

    if(role == Qt::DisplayRole)
    {

        switch(index.column())
        {
        case 0:
            return member.GetMemberName();
            break;
        case 1:
            return member.GetID();
            break;
        case 2:
            return QString::number(member.GetRebate(), 'f', 2);
            break;
        default:
            return QVariant();
            break;
        }
    }
    if(role == Qt::UserRole)
    {
        switch(index.column())
        {
        case 0:
            return member.GetMemberName();
            break;
        case 1:
            return member.GetID();
            break;
        case 2:
            return member.GetRebate();
            break;
        default:
            return QVariant();
            break;
        }
    }
    return QVariant();
}

QVariant RebateModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return tr("Name");
            break;
        case 1:
            return tr("Member ID");
            break;
        case 2:
            return tr("Rebate");
            break;
        }
    }
    return QVariant();
}

