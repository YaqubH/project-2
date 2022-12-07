#include "ModifyItem.h"
#include "ui_ModifyItem.h"

ModifyItem::ModifyItem(QWidget *parent, Item item) :
    QDialog(parent),
    ui(new Ui::ModifyItem)
{
    ui->setupUi(this);
    ui->LineEdit_ModifyItemName->setReadOnly(true);
    ui->LineEdit_ModifyItemName->setText(item.GetItemName());
    ui->LineEdit_ModifyItemPrice->setValue(item.GetItemPrice());
    this->itemToModify= item;
}

ModifyItem::~ModifyItem()
{
    delete ui;
}

void ModifyItem::on_PushButton_ModifyItem_clicked()
{
    QString name;
    float price;
    name = ui->LineEdit_ModifyItemName->text();
    price = ui->LineEdit_ModifyItemPrice->value();

    if(dbManager::instance().ItemExists(Item(name,price)))
    {
        if(dbManager::instance().modifyItem(Item(name, price)))
        {
            QMessageBox::information(NULL,"Congrats","Item Modified","OK");
            emit ModifyConfirmed();
        }else{
            QMessageBox::information(NULL,"Try Again","Failed To Add Item","OK");
        }
    }
    else
    {
        QMessageBox::information(NULL,"Sorry","Item doesn't exist","OK");
    }
    close();
}

void ModifyItem::on_PushButton_BackToMain_clicked()
{
    this->close();
}
