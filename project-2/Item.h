#ifndef ITEM_H
#define ITEM_H

#include <QObject>

class Item
{
public:

    Item(){
        itemName = "";
        itemPrice = 0.00;
    }

    Item(QString name, float price){
        itemName = name;
        itemPrice = price;
    }

    QString GetItemName() const{return itemName;}

        float GetItemPrice() const{return itemPrice;}
private:
    QString itemName;
    float  itemPrice;
};

#endif // ITEM_H
