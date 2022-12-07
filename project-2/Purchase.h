#ifndef PURCHASE_H
#define PURCHASE_H

#include <QDate>
#include "Item.h"

const float TAX_RATE = 0.0775;

class Purchase
{
public:

    Purchase(): purchaseDate(1970, 1, 1), itemPurchased("", 0.0){
        membersID = 0;
        quantityPurchased = 0;
        purchaseSubtotal = 0.0;
        purchaseTotal = 0.0;
    }

    Purchase(QDate transDate, int bID, Item item, int quantity, float transSub, float transTotal){
        purchaseDate = transDate;
        membersID = bID;
        itemPurchased = item;
        quantityPurchased = quantity;
        purchaseSubtotal = transSub;
        purchaseTotal = transTotal;
    }

    QDate GetPurchaseDate() const{return purchaseDate;}

    int GetMemberID() const{return membersID;}


    Item GetItem() const{return itemPurchased;}

    QString GetItemName() const{return itemPurchased.GetItemName();}

    int GetQuantityPurchased() const{return quantityPurchased;}

    float GetPurchaseSubTotal() const{return purchaseSubtotal;}


    float GetPurchaseTotal() const{return purchaseTotal;}

private:
    QDate  purchaseDate;
    int    membersID;
    Item   itemPurchased;
    int    quantityPurchased;
    float  purchaseSubtotal;
    float  purchaseTotal;
};

#endif // PURCHASE_H
