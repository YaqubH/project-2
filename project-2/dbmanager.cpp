#include "dbManager.h"
#include <QDebug>

dbManager::dbManager()
{
    bulkdb = QSqlDatabase::addDatabase("QSQLITE");
    bulkdb.setDatabaseName("C:/Users/Yaqub/OneDrive/Desktop/CS1C/Projects/project-2/project-2/HiChew - Copy.db");

    if(QFile::exists("C:/Users/Yaqub/OneDrive/Desktop/CS1C/Projects/project-2/project-2/HiChew - Copy.db"))
    {
        if(!bulkdb.open())
        {
            qDebug() << "Error connecting to database";
        }
        else
        {
            qDebug() << "Connected to database";
        }
    }
    else
    {
        qDebug() << "Database file path does not exist";
    }
}

dbManager::~dbManager()
{
    if(bulkdb.isOpen())
    {
        bulkdb.close();
    }
    qDebug() << "Destroying dbManager";
}

dbManager& dbManager::instance()
{
    static dbManager bulkdbInstance;

    return bulkdbInstance;
}

bool dbManager::AddItem(const Item& newItem)
{
    bool success;
    QString itemName;
    float price;

    success = false;
    itemName = newItem.GetItemName();
    price = newItem.GetItemPrice();
    qDebug() << "Item " << itemName << " Price " << price;
    QSqlQuery query;
    query.prepare("INSERT INTO InventoryInfo (ItemName, ItemPrice) VALUES (:ItemName, :ItemPrice)");
    query.bindValue(":ItemName", itemName);
    query.bindValue(":ItemPrice", price);

    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "addItem error:"
                 << query.lastError();
    }

    return success;
}

bool dbManager::RemoveItem(const Item& item)
{
    QSqlQuery query;
    bool success;
    QString itemName;

    success = false;
    itemName = item.GetItemName();

    if(ItemExists(item))
    {
        query.prepare("DELETE FROM InventoryInfo WHERE ItemName = (:ItemName)");
        query.bindValue(":ItemName", itemName);


        if(query.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "RemoveItem error:"
                     << query.lastError();
        }
    }
    return success;
}

bool dbManager::modifyItem(const Item& item)
{
    QSqlQuery query;
    bool success;
    QString itemName;
    double itemPrice;

    success = false;
    itemName = item.GetItemName();
    itemPrice = item.GetItemPrice();
    qDebug() << "Item " << itemName << " Price " << itemPrice;
    query.prepare("UPDATE InventoryInfo SET ItemPrice=:ItemPrice WHERE ItemName=:ItemName");
    query.bindValue(":ItemName", itemName);
    query.bindValue(":ItemPrice", itemPrice);

    if(query.exec())
    {
        success = true;
    }else{
        qDebug() << "Modify Item error:"
                 << query.lastError();
    }
    return success;
}
bool dbManager::ItemExists(const Item& item) const
{
    bool exists;
    QSqlQuery query;

    query.prepare("SELECT ItemName FROM InventoryInfo WHERE ItemName = :name");
    query.bindValue(":name", item.GetItemName());

    exists = false;
    if(query.exec())
    {
        if(query.next())
        {
            exists = true;
        }
    }
    return exists;
}

bool dbManager::VerifyLogin(const Credentials &credentials, QString& employeeType)
{
    QSqlQuery query;
    bool success;

    success = false;
    query.prepare("SELECT Username, Password, EmployeeType FROM LoginInfo WHERE Username = :user AND Password = :passw");
    query.bindValue(":user", credentials.GetUsername());
    query.bindValue(":passw", credentials.GetPassword());

    if(query.exec())
    {
        if(query.next())
        {
            employeeType = query.value(2).toString();
            success = true;
        }
    }
    else
    {
        qDebug() << "Login Error: " << query.lastError();
    }

    return success;
}
bool dbManager::AddMember(const Member& newMember)
{
    QSqlQuery query;
    bool success;

    success = false;
    query.prepare("INSERT INTO MembersInfo (MemberName, MemberID, MemberStatus, MemberExpire)"
                  " VALUES (:name, :id, :member_type, :expiration_date)");
    query.bindValue(":name", newMember.GetMemberName());
    query.bindValue(":id", newMember.GetID());
    query.bindValue(":member_type", newMember.GetMembershipTypeString());
    query.bindValue(":expiration_date", newMember.GetExpirationDateString());

    if(query.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "Add Member Error: " << query.lastError();
    }

    return success;
}

bool dbManager::RemoveMember(const Member& member)
{
    bool success;
    QSqlQuery deleteQueryMember;
    QSqlQuery deleteQueryPurchase;

    success = false;
    if(MemberExists(member.GetID()))
    {
        deleteQueryPurchase.prepare("DELETE FROM SalesInfo WHERE ID = (:MemberID)");
        deleteQueryPurchase.bindValue(":MemberID", member.GetID());
        if(deleteQueryPurchase.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "Remove Purchase Error: " << deleteQueryPurchase.lastError();
        }

        deleteQueryMember.prepare("DELETE FROM MembersInfo WHERE MemberName = (:MemberName) AND MemberID = (:MemberID)");
        deleteQueryMember.bindValue(":MemberName", member.GetMemberName());
        deleteQueryMember.bindValue(":MemberID", member.GetID());

        if(deleteQueryMember.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "Remove Member Error: " << deleteQueryMember.lastError();
        }
    }
    else
    {
        qDebug() << "Member doesn't exist";
    }
    return success;
}

bool dbManager::MemberExists(const int memberID)
{
    bool memberExists;
    QSqlQuery checkQuery;

    memberExists = false;

    checkQuery.prepare("SELECT MemberID FROM MembersInfo WHERE MemberID = (:MemberID)");
    checkQuery.bindValue(":MemberID", memberID);

    if(checkQuery.exec())
    {
        if(checkQuery.next())
        {
            memberExists = true;
        }
    }

    return memberExists;
}

Member dbManager::GetMember(int memberID)
{
    QSqlQuery query;
    int nameIndex;
    int idIndex;
    int mTypeIndex;
    int expDateIndex;
    int rebateIndex;
    int totalSpentIndex;
    QString name;
    int id;
    QString mType;
    MemberType membershipType;
    QString expDate;
    QDate expirationDate;
    float rebate;
    float totalSpent;

    query.prepare("SELECT MemberName, MemberID, MemberStatus, MemberExpire, MemberRebate, MemberTotal FROM MembersInfo WHERE MemberID = :MemberID");
    query.bindValue(":MemberID", memberID);

    if(query.exec())
    {
        nameIndex           = query.record().indexOf("MemberName");
        idIndex             = query.record().indexOf("MemberID");
        mTypeIndex          = query.record().indexOf("MemberStatus");
        expDateIndex        = query.record().indexOf("MemberExpire");
        rebateIndex         = query.record().indexOf("MemberRebate");
        totalSpentIndex     = query.record().indexOf("MemberTotal");
        if(query.next())
        {
            name    = query.value(nameIndex).toString();
            id      = query.value(idIndex).toInt();
            mType   = query.value(mTypeIndex).toString();
            if(mType == "Regular")
            {
                membershipType = REGULAR;
            }
            else if(mType == "Executive")
            {
                membershipType = EXECUTIVE;
            }
            expDate = query.value(expDateIndex).toString();
            expirationDate = QDate::fromString(expDate, "MM/dd/yyyy");
            rebate = query.value(rebateIndex).toFloat();
            totalSpent = query.value(totalSpentIndex).toFloat();
        }
        else
        {
            //  qDebug() << "Can't find member";
            name = "";
            id = 0;
            membershipType = REGULAR;
            expirationDate = QDate(2000, 1, 1);
            rebate = 0;
            totalSpent = 0;
        }
    }
    else
    {
        qDebug() << "Get Member Error: " << query.lastError();
        name = "";
        id = 0;
        membershipType = REGULAR;
        expirationDate = QDate(2000, 1, 1);
        rebate = 0;
        totalSpent = 0;
    }

    return Member(name, id, membershipType, expirationDate, rebate, totalSpent);
}

QList<Member> dbManager::GetAllMembers()
{
    QSqlQuery query;
    QList<Member> memberList;
    int nameIndex;
    int idIndex;
    int mTypeIndex;
    int expDateIndex;
    int rebateIndex;
    int totalSpentIndex;
    QString name;
    int id;
    QString mType;
    MemberType membershipType;
    QString expDate;
    QDate expirationDate;
    float rebate;
    float totalSpent;

    query.prepare("SELECT MemberName, MemberID, MemberStatus, MemberExpire, MemberRebate, MemberTotal FROM MembersInfo");

    if(query.exec())
    {
        nameIndex       = query.record().indexOf("MemberName");
        idIndex         = query.record().indexOf("MemberID");
        mTypeIndex      = query.record().indexOf("MemberStatus");
        expDateIndex    = query.record().indexOf("MemberExpire");
        rebateIndex     = query.record().indexOf("MemberRebate");
        totalSpentIndex = query.record().indexOf("MemberTotal");

        while(query.next())
        {
            name    = query.value(nameIndex).toString();
            id      = query.value(idIndex).toInt();
            mType   = query.value(mTypeIndex).toString();
            if(mType == "Regular")
            {
                membershipType = REGULAR;
            }
            else if(mType == "Executive")
            {
                membershipType = EXECUTIVE;
            }
            expDate = query.value(expDateIndex).toString();
            expirationDate = QDate::fromString(expDate, "MM/dd/yyyy");
            rebate = query.value(rebateIndex).toFloat();
            totalSpent = query.value(totalSpentIndex).toFloat();

            memberList.append(Member(name, id, membershipType, expirationDate, rebate, totalSpent));
        }
    }
    else
    {
        qDebug() << "Error Getting Members: " << query.lastError();
    }

    return memberList;
}

Item dbManager::GetItem(QString itemName)
{
    QSqlQuery query;
    int nameIndex = 0;
    int priceIndex = 0;
    QString name;
    float price = 0.0;

    query.prepare("SELECT ItemName, ItemPrice FROM InventoryInfo WHERE ItemName  = :ItemName");
    query.bindValue(":ItemName", itemName);

    if(query.exec())
    {
        nameIndex       = query.record().indexOf("ItemName");
        priceIndex      = query.record().indexOf("ItemPrice");

        if(query.next())
        {
            name       = query.value(nameIndex).toString();
            price      = query.value(priceIndex).toFloat();
        }
        else
        {
            qDebug() << "Can't find item";
        }
    }
    else
    {
        qDebug() << "Get Item Error: " << query.lastError();
    }

    return Item(name, price);
}

float dbManager::GetItemPrice(QString item)
{
    QSqlQuery query;
    //int priceIndex = 0;
    double itemPrice = 0.0;

    query.prepare("SELECT ItemPrice FROM InventoryInfo WHERE ItemName = :itemName");
    query.bindValue(":itemName", item);
    if(query.exec())
    {
        if(query.next())
        {
            itemPrice = query.value(0).toDouble();
            qDebug() << "itemPrice= " << itemPrice;
        }

    }
    else qDebug() << "GetItemPrice Error"<< query.lastError();
    return itemPrice;
}

bool dbManager::isOpen() const
{
    return bulkdb.isOpen();
}

QList<Item> dbManager::GetAllItems()
{
    QList<Item> itemList;
    QString name;
    float price;
    int nameIndex;
    int priceIndex;
    QSqlQuery query;

    query.prepare("SELECT ItemName, ItemPrice FROM InventoryInfo");

    if(query.exec())
    {
        nameIndex = query.record().indexOf("ItemName");
        priceIndex = query.record().indexOf("ItemPrice");
        while(query.next())
        {
            name = query.value(nameIndex).toString();
            price = query.value(priceIndex).toFloat();
            itemList.append(Item(name, price));
        }
    }
    else
    {
        qDebug() << "Error getting items: " << query.lastError();
    }

    return itemList;
}

QStringList dbManager::GetAllItemNames()
{
    QSqlQuery query;
    int nameIndex;
    QString name;
    QStringList itemNames;

    query.prepare("SELECT ItemName FROM InventoryInfo");
    if(query.exec())
    {
        nameIndex = query.record().indexOf("ItemName");
        while(query.next())
        {
            name = query.value(nameIndex).toString();
            itemNames.append(name);
        }
    }
    else
    {
        qDebug() << "Error getting items: " << query.lastError();
    }
    return itemNames;
}
bool dbManager::AddPurchase(const Purchase& newPurchase)
{
    QSqlQuery query;
    bool success;


    query.prepare("INSERT INTO SalesInfo (Date, ID, Item, Cost, Quantity, Subtotal, Total) "
                  "VALUES (:Date, :ID, :Item, :Cost, :Quantity, :Subtotal, :Total)");
    query.bindValue(":Date", newPurchase.GetPurchaseDate().toString("MM/dd/yyyy"));
    query.bindValue(":ID", newPurchase.GetMemberID());
    query.bindValue(":Item", newPurchase.GetItemName());
    query.bindValue(":Cost", newPurchase.GetItem().GetItemPrice());
    query.bindValue(":Quantity", newPurchase.GetQuantityPurchased());
    query.bindValue(":Subtotal", newPurchase.GetPurchaseSubTotal());
    query.bindValue(":Total", newPurchase.GetPurchaseTotal());

    if(query.exec())
    {
        success = true;
        PurchaseUpdateInventory(newPurchase);
        PurchaseUpdateMemberTotalSpent(newPurchase);
        if(GetMember(newPurchase.GetMemberID()).isExecutive())
        {
            PurchaseUpdateMemberRebate(newPurchase);
        }
    }
    else
    {
        qDebug() << "Failed to add Purchase" << query.lastError();
        success = false;
    }
    return success;
}

bool dbManager::PurchaseUpdateInventory(const Purchase newPurchase)
{
    QSqlQuery query;
    bool success;
    if(ItemExists(newPurchase.GetItem()))
    {
        query.prepare("UPDATE InventoryInfo SET QuantitySold = QuantitySold + :QuantitySold WHERE ItemName = :ItemName");
        query.bindValue(":QuantitySold", newPurchase.GetQuantityPurchased());
        query.bindValue(":ItemName", newPurchase.GetItemName());
        if(query.exec())
        {
            success = true;
            qDebug() << "Updated quantity sold.";
        }
        else
        {
            qDebug() << "Updating item in inventory failed." << query.lastError();
            success = false;
        }
    }
    else
    {
        qDebug() << "Item did not exist";
        success = false;
    }
    return success;
}

bool dbManager::PurchaseUpdateMemberTotalSpent(const Purchase newPurchase)
{
    QSqlQuery query;
    bool success;
    if(MemberExists(newPurchase.GetMemberID()))
    {
        query.prepare("UPDATE MembersInfo SET MemberTotal = MemberTotal + :MemberTotal WHERE MemberID = :MemberID");
        query.bindValue(":MemberTotal", newPurchase.GetPurchaseTotal());
        query.bindValue(":MemberID", newPurchase.GetMemberID());
        if(query.exec())
        {
            success = true;
            qDebug() << "Updated total spent";
        }
        else
        {
            qDebug() << "Updating member's total spent failed: " << query.lastError();
            success = false;
        }
    }
    else
    {
        qDebug() << "Member does not exist";
        success = false;
    }
    return success;
}

bool dbManager::PurchaseUpdateMemberRebate(const Purchase newPurchase)
{
    QSqlQuery query;
    bool success;
    if(MemberExists(newPurchase.GetMemberID()))
    {
        query.prepare("UPDATE MemberInfo SET MemberRebate = MemberRebate + :MemberRebateAmy WHERE MemberID = :MemberID");
        query.bindValue(":MemberRebate", newPurchase.GetPurchaseSubTotal() * REBATE_RATE);
        query.bindValue(":MemberID", newPurchase.GetMemberID());
        if(query.exec())
        {
            success = true;
            qDebug() << "Updated rebate";
        }
        else
        {
            qDebug() << "Updating member's rebate failed: " << query.lastError();
            success = false;
        }
    }
    else
    {
        qDebug() << "Member does not exist";
        success = false;
    }
    return success;
}

bool dbManager::PurchaseExists(const Purchase& trans)
{
    bool PurchaseExists;
    QSqlQuery checkQuery;

    PurchaseExists = false;

    checkQuery.prepare("SELECT ID, Item, Quantity FROM SalesInfo "
                       "WHERE ID = :ID AND Item = :Item AND Quantity = :Quantity");
    checkQuery.bindValue(":ID", trans.GetMemberID());
    checkQuery.bindValue(":Item", trans.GetItemName());
    checkQuery.bindValue(":Quantity", trans.GetQuantityPurchased());

    if(checkQuery.exec())
    {
        if(checkQuery.next())
        {
            PurchaseExists = true;
        }
    }

    return PurchaseExists;
}

QList<Purchase> dbManager::GetAllPurchases()
{
    QSqlQuery query;
    QList<Purchase> PurchaseList;
    int dateIndex;
    int idIndex;
    int itemNameIndex;
    int itemPriceIndex;
    int qtyIndex;
    int subTotIndex;
    int totIndex;
    QDate transDate;
    int id;
    QString itemName;
    float itemPrice;
    int qty;
    float transSubtotal;
    float transTotal;

    query.prepare("SELECT Date, ID, Item, Cost, Quantity, Subtotal, Total FROM SalesInfo");

    if(query.exec())
    {
        dateIndex       = query.record().indexOf("Date");
        idIndex         = query.record().indexOf("ID");
        itemNameIndex   = query.record().indexOf("Item");
        itemPriceIndex  = query.record().indexOf("Cost");
        qtyIndex        = query.record().indexOf("Quantity");
        subTotIndex     = query.record().indexOf("Subtotal");
        totIndex        = query.record().indexOf("Total");

        while(query.next())
        {
            transDate   = QDate::fromString(query.value(dateIndex).toString(), "MM/dd/yyyy");
            id          = query.value(idIndex).toInt();
            itemName    = query.value(itemNameIndex).toString();
            itemPrice   = query.value(itemPriceIndex).toFloat();
            qty         = query.value(qtyIndex).toInt();
            transSubtotal = query.value(subTotIndex).toFloat();
            transTotal  = query.value(totIndex).toFloat();

            PurchaseList.append(Purchase(transDate, id, Item(itemName, itemPrice), qty, transSubtotal, transTotal));
        }
    }
    else
    {
        qDebug() << "Error Getting Purchases: " << query.lastError();
    }

    return PurchaseList;
}

float dbManager::CalcGrossSales()
{
    QSqlQuery query;
    float grossSales;

    query.prepare("SELECT Sum(subtotal) FROM SalesInfo");
    grossSales = 0;

    if(query.exec())
        if(query.next())
            grossSales = query.value(0).toFloat();

    return grossSales;
}

float dbManager::CalcGrossSalesByDate(QDate tDate)
{
    QSqlQuery query;
    float grossSales;

    query.prepare("SELECT Sum(Subtotal) FROM SalesInfo WHERE Date = :Date");
    query.bindValue(":Date", tDate.toString("MM/dd/yyyy"));
    grossSales = 0;

    if(query.exec())
        if(query.next())
            grossSales = query.value(0).toFloat();

    return grossSales;
}

float dbManager::CalcGrossSalesByMember(int buyersID)
{
    QSqlQuery query;
    float grossSales;

    query.prepare("SELECT Sum(Subtotal) FROM SalesInfo WHERE ID = :ID");
    query.bindValue(":ID", buyersID);
    grossSales = 0;

    if(query.exec())
        if(query.next())
            grossSales = query.value(0).toFloat();

    return grossSales;
}

float dbManager::CalcGrossSalesByMember(QString bName)
{
    QSqlQuery queryMember;
    QSqlQuery queryTrans;
    float grossSales;
    int id;

    queryMember.prepare("SELECT MemberID FROM MembersInfo WHERE MemberName = :MemberName COLLATE NOCASE");
    queryMember.bindValue(":MemberName", bName);

    grossSales = 0;

    if(queryMember.exec())
        if(queryMember.next())
        {
            id = queryMember.value(0).toInt();

            queryTrans.prepare("SELECT Sum(Subtotal) FROM SalesInfo WHERE ID = :ID");
            queryTrans.bindValue(":ID", id);

            if(queryTrans.exec())
                if(queryTrans.next())
                    grossSales = queryTrans.value(0).toFloat();
        }

    return grossSales;
}

float dbManager::CalcGrossSalesByMember(MemberType mType)
{
    QSqlQuery query;
    float grossSales;
    int id;

    query.prepare("SELECT Subtotal, ID FROM SalesInfo");

    grossSales = 0;
    if(query.exec())
        while(query.next())
        {
            id = query.value(1).toInt();
            if(GetMember(id).isExecutive())
            {
                if(mType == EXECUTIVE)
                    grossSales += query.value(0).toFloat();
            }
            else
            {
                if(mType == REGULAR)
                    grossSales += query.value(0).toFloat();
            }
        }

    return grossSales;
}

float dbManager::CalcGrossSalesByItem(QString itemName)
{
    QSqlQuery query;
    float grossSales;

    query.prepare("SELECT Sum(Subtotal) FROM SalesInfo WHERE Item = :Item COLLATE NOCASE");
    query.bindValue(":Item", itemName);
    grossSales = 0;

    if(query.exec())
        if(query.next())
            grossSales = query.value(0).toFloat();

    return grossSales;
}
int dbManager::GetTotalQuantity()
{
    QSqlQuery query;
    int quantityTotal;
    query.prepare("SELECT Sum(Quantity) FROM SalesInfo");
    quantityTotal = 0;
    if(query.exec())
        if(query.next())
            quantityTotal = query.value(0).toInt();

    return quantityTotal;

}
int dbManager::GetTotalQuantityByDate(QDate tDate)
{
    QSqlQuery query;
    int quantityTotal;

    query.prepare("SELECT Sum(Quantity) FROM SalesInfo WHERE Date = :Date");
    query.bindValue(":Date", tDate.toString("MM/dd/yyyy"));
    quantityTotal = 0;

    if(query.exec())
        if(query.next())
            quantityTotal = query.value(0).toInt();

    return quantityTotal;
}

int dbManager::GetTotalQuantityByMember(int buyersID)
{
    QSqlQuery query;
    int quantityTotal;

    query.prepare("SELECT Sum(Quantity) FROM SalesInfo WHERE ID = :ID");
    query.bindValue(":ID", buyersID);
    quantityTotal = 0;

    if(query.exec())
        if(query.next())
            quantityTotal = query.value(0).toInt();

    return quantityTotal;
}

int dbManager::GetTotalQuantityByMember(QString bName)
{
    QSqlQuery queryMember;
    QSqlQuery queryTrans;
    int quantityTotal;
    int id;

    queryMember.prepare("SELECT MemberID FROM MembersInfo WHERE MemberName = :MemberName COLLATE NOCASE");
    queryMember.bindValue(":MemberName", bName);

    quantityTotal = 0;

    if(queryMember.exec())
        if(queryMember.next())
        {
            id = queryMember.value(0).toInt();

            queryTrans.prepare("SELECT Sum(Quantity) FROM SalesInfo WHERE ID = :ID");
            queryTrans.bindValue(":ID", id);

            if(queryTrans.exec())
                if(queryTrans.next())
                    quantityTotal = queryTrans.value(0).toInt();
        }

    return quantityTotal;
}

int dbManager::GetTotalQuantityByMember(MemberType mType)
{
    QSqlQuery query;
    int quantityTotal;
    int id;

    query.prepare("SELECT Quantity, ID FROM SalesInfo");

    quantityTotal = 0;
    if(query.exec())
        while(query.next())
        {
            id = query.value(1).toInt();
            if(GetMember(id).isExecutive())
            {
                if(mType == EXECUTIVE)
                    quantityTotal += query.value(0).toInt();
            }
            else
            {
                if(mType == REGULAR)
                    quantityTotal += query.value(0).toInt();
            }
        }

    return quantityTotal;
}

int dbManager::GetTotalQuantityByItem(QString itemName)
{
    QSqlQuery query;
    int quantityTotal;

    query.prepare("SELECT Sum(Quantity) FROM SalesInfo WHERE Item = :Item COLLATE NOCASE");
    query.bindValue(":Item", itemName);
    quantityTotal = 0;

    if(query.exec())
        if(query.next())
            quantityTotal = query.value(0).toInt();

    return quantityTotal;
}

int dbManager::GetTotalShoppers(MemberType mType)
{
    QSqlQuery query;
    int totalShoppers;
    int id;
    int temp;
    QString mTypeString;

    query.prepare("SELECT ID FROM SalesInfo ORDER BY ID");
    totalShoppers = 0;
    if(mType == REGULAR)
        mTypeString = "Regular";
    else
        mTypeString = "Executive";

    id = 0;
    totalShoppers = 0;
    if(query.exec())
        while(query.next())
        {
            temp = query.value(0).toInt();
            if(id != temp)
            {
                id = temp;
                if(GetMember(id).GetMembershipTypeString() == mTypeString)
                {
                    totalShoppers++;
                }
            }
        }

    return totalShoppers;
}
int dbManager::GetTotalShoppersByDate(QDate fDate, MemberType mType)
{
    QSqlQuery query;
    int totalShoppers;
    int id;
    int temp;
    QString mTypeString;

    query.prepare("SELECT ID FROM SalesInfo WHERE Date = :Date ORDER BY ID");
    query.bindValue(":Date", fDate.toString("MM/dd/yyyy"));

    totalShoppers = 0;
    id = 0;
    if(mType == REGULAR)
        mTypeString = "Regular";
    else
        mTypeString = "Executive";

    if(query.exec())
        while(query.next())
        {
            temp = query.value(0).toInt();
            if(id != temp)
            {
                id = temp;
                if(GetMember(id).GetMembershipTypeString() == mTypeString)
                {
                    totalShoppers++;
                }
            }
        }

    return totalShoppers;
}

int dbManager::GetTotalShoppersByItem(QString itemName, MemberType mType)
{
    QSqlQuery query;
    int totalShoppers;
    int id;
    int temp;
    QString mTypeString;

    query.prepare("SELECT Date FROM SalesInfo WHERE Item = :Item ORDER BY ID COLLATE NOCASE");
    query.bindValue(":Item", itemName);

    totalShoppers = 0;
    id = 0;
    if(mType == REGULAR)
        mTypeString = "Regular";
    else
        mTypeString = "Executive";

    if(query.exec())
        while(query.next())
        {
            temp = query.value(0).toInt();
            if(id != temp)
            {
                id = temp;
                if(GetMember(id).GetMembershipTypeString() == mTypeString)
                {
                    totalShoppers++;
                }
            }
        }

    return totalShoppers;
}

bool dbManager::GetValidDates(QDate &earliestDate, QDate &latestDate)
{
    bool success;
    QSqlQuery query;
    int transIndex;
    QDate tempDate;

    query.prepare("SELECT Date FROM SalesInfo");

    if(query.exec())
    {
        transIndex = query.record().indexOf("Date");
        if(query.next())
        {
            success = true;
            earliestDate = QDate::fromString(query.value(transIndex).toString(), "MM/dd/yyyy");
            latestDate = QDate::fromString(query.value(transIndex).toString(), "MM/dd/yyyy");
            while(query.next())
            {
                tempDate = QDate::fromString(query.value(transIndex).toString(), "MM/dd/yyyy");
                if(tempDate < earliestDate)
                {
                    earliestDate = tempDate;
                }
                else if(tempDate > latestDate)
                {
                    latestDate = tempDate;
                }
            }

        }
        else
        {
            success = false;
        }
    }
    else
    {
        success = false;
    }

    return success;
}

QList<Member> dbManager::GetUpgrades()
{
    QSqlQuery query;
    QList<Member> memberList;
    int nameIndex;
    int idIndex;
    int mTypeIndex;
    int expDateIndex;
    int rebateIndex;
    int totalSpentIndex;
    QString name;
    int id;
    QString mType;
    MemberType membershipType;
    QString expDate;
    QDate expirationDate;
    float rebate;
    float totalSpent;

    query.prepare("SELECT MemberName, MemberID, MemberStatus, MemberExpire, MemberRebate, MemberTotal FROM MembersInfo");

    if(query.exec())
    {
        nameIndex       = query.record().indexOf("MemberName");
        idIndex         = query.record().indexOf("MemberID");
        mTypeIndex      = query.record().indexOf("MemberStatus");
        expDateIndex    = query.record().indexOf("MemberExpire");
        rebateIndex     = query.record().indexOf("MemberRebate");
        totalSpentIndex = query.record().indexOf("MemberTotal");

        while(query.next())
        {
            mType   = query.value(mTypeIndex).toString();
            if(mType == "Regular")
            {
                totalSpent = query.value(totalSpentIndex).toFloat();
                if(totalSpent/(1 + TAX_RATE) >= 55)
                {
                    name    = query.value(nameIndex).toString();
                    id      = query.value(idIndex).toInt();
                    membershipType = REGULAR;
                    expDate = query.value(expDateIndex).toString();
                    expirationDate = QDate::fromString(expDate, "MM/dd/yyyy");
                    rebate = query.value(rebateIndex).toFloat();
                    memberList.append(Member(name, id, membershipType, expirationDate, rebate, totalSpent));
                }
            }
        }
    }
    else
    {
        qDebug() << "Error Getting Upgrades: " << query.lastError();
    }

    return memberList;
}

QList<Member> dbManager::GetDowngrades()
{
    QSqlQuery query;
    QList<Member> memberList;
    int nameIndex;
    int idIndex;
    int mTypeIndex;
    int expDateIndex;
    int rebateIndex;
    int totalSpentIndex;
    QString name;
    int id;
    QString mType;
    MemberType membershipType;
    QString expDate;
    QDate expirationDate;
    float rebate;
    float totalSpent;

    query.prepare("SELECT MemberName, MemberID, MemberStatus, MemberExpire, MemberRebate, MemberTotal FROM MembersInfo");

    if(query.exec())
    {
        nameIndex       = query.record().indexOf("MemberName");
        idIndex         = query.record().indexOf("MemberID");
        mTypeIndex      = query.record().indexOf("MemberStatus");
        expDateIndex    = query.record().indexOf("MemberExpire");
        rebateIndex     = query.record().indexOf("MemberRebate");
        totalSpentIndex = query.record().indexOf("MemberTotal");

        while(query.next())
        {
            mType   = query.value(mTypeIndex).toString();
            if(mType == "Executive")
            {
                rebate = query.value(rebateIndex).toFloat();
                if(rebate < 55)
                {
                    name    = query.value(nameIndex).toString();
                    id      = query.value(idIndex).toInt();
                    membershipType = EXECUTIVE;
                    totalSpent = query.value(totalSpentIndex).toFloat();
                    expDate = query.value(expDateIndex).toString();
                    expirationDate = QDate::fromString(expDate, "MM/dd/yyyy");
                    memberList.append(Member(name, id, membershipType, expirationDate, rebate, totalSpent));
                }
            }
        }
    }
    else
    {
        qDebug() << "Error Getting Downgrades: " << query.lastError();
    }

    return memberList;
}

QList<Member> dbManager::GetExpirations(int month)
{
    QSqlQuery query;
    QList<Member> memberList;
    int nameIndex;
    int idIndex;
    int mTypeIndex;
    int expDateIndex;
    QString name;
    int id;
    QString mType;
    QString expDate;
    QDate expirationDate;
    int renewCost;

    query.prepare("SELECT MemberName, MemberID, MemberStatus, MemberExpire FROM MembersInfo");

    if(query.exec())
    {
        nameIndex       = query.record().indexOf("MemberName");
        idIndex         = query.record().indexOf("MemberID");
        mTypeIndex      = query.record().indexOf("MemberStatus");
        expDateIndex    = query.record().indexOf("MemberExpire");

        while(query.next())
        {
            expDate = query.value(expDateIndex).toString();
            expirationDate = QDate::fromString(expDate, "MM/dd/yyyy");
            if(month == expirationDate.month())
            {
                mType   = query.value(mTypeIndex).toString();
                name    = query.value(nameIndex).toString();
                id      = query.value(idIndex).toInt();
                if(mType == "Executive")
                {
                    renewCost = 120;
                }
                else
                {
                    renewCost = 65;
                }

                memberList.append(Member(name, id, renewCost));
            }
        }
    }
    else
    {
        qDebug() << "Error Getting Expirations: " << query.lastError();
    }

    return memberList;
}

QList<Member> dbManager::GetRebates()
{
    QSqlQuery query;
    QList<Member> memberList;
    int nameIndex;
    int idIndex;
    int mTypeIndex;
    int rebateIndex;
    QString name;
    int id;
    QString mType;
    float rebate;

    query.prepare("SELECT MemberName, MemberID, MemberStatus, MemberRebate FROM MembersInfo");

    if(query.exec())
    {
        nameIndex       = query.record().indexOf("MemberName");
        idIndex         = query.record().indexOf("MemberID");
        mTypeIndex      = query.record().indexOf("MemberStatus");
        rebateIndex     = query.record().indexOf("MemberRebate");

        while(query.next())
        {
            mType   = query.value(mTypeIndex).toString();
            if(mType == "Executive")
            {
                name    = query.value(nameIndex).toString();
                id      = query.value(idIndex).toInt();
                rebate = query.value(rebateIndex).toFloat();
                memberList.append(Member(name, id, rebate));
            }
        }
    }
    else
    {
        qDebug() << "Error Getting Rebates: " << query.lastError();
    }

    return memberList;
}

