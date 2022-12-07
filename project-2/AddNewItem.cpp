#include "AddNewItem.h"
#include "ui_AddNewItem.h"

AddNewItem::AddNewItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewItem)
{
    ui->setupUi(this);
}

AddNewItem::~AddNewItem()
{
    delete ui;
}

void AddNewItem::on_PushButton_AddItem_clicked()
{
    QString name;
    float price;
    name = ui->LineEdit_ItemName->text();
    price = ui->LineEdit_ItemPrice->value();

    if(!dbManager::instance().ItemExists(Item(name,price)))
    {
        if(dbManager::instance().AddItem(Item(name, price)))
        {
            QMessageBox::information(NULL,"Congrats","Item Added","OK");
            emit itemAdded();
        }
        else
        {
            QMessageBox::information(NULL,"Try Again","Failed To Add Item","OK");
        }
    }
    else
    {
        QMessageBox::information(NULL,"Sorry","Item already exists","OK");
    }
}

void AddNewItem::on_PushButton_BackToMain_clicked()
{
    this->close();
}
