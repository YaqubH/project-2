#include "AddNewPurchase.h"
#include "ui_AddNewPurchase.h"

AddNewPurchase::AddNewPurchase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewPurchase)
{
    ui->setupUi(this);
    ui->ItemNameComboBox->addItems(dbManager::instance().GetAllItemNames());
}

AddNewPurchase::~AddNewPurchase()
{
    delete ui;
}
void AddNewPurchase::receiveID(int id)
{
    memberID = id;
}
void AddNewPurchase::on_PushButton_AddPurchase_clicked()
{
    QDate   transDate;
    int     id;
    QString transItemName;
    float   itemPrice;
    int     transQuantity;
    float   transSubtotal;
    float   taxAmt;
    float   transTotal;

    transDate = ui->purchaseDate->date();
    id = memberID;
    transItemName = ui->ItemNameComboBox->itemText(ui->ItemNameComboBox->currentIndex());
    transQuantity = ui->PurchaseQuantitySpinBox->value();
    itemPrice = dbManager::instance().GetItemPrice(transItemName);
    transSubtotal = itemPrice * transQuantity;
    taxAmt = transSubtotal * TAX_RATE;
    transTotal = transSubtotal + taxAmt;
    ui->Label_ItemPriceOutput->setText(QString::number(itemPrice, 'f', 2));
    ui->Label_ItemQuantityTotal->setText(QString::number(transQuantity));
    ui->Label_PurchaseSubtotalOutput->setText(QString::number(transSubtotal, 'f', 2));
    ui->Label_PurchaseTaxOutput->setText(QString::number(taxAmt, 'f', 2));
    ui->Label_PurchaseTotalOutput->setText(QString::number(transTotal, 'f', 2));
    qDebug() << "Added " << transItemName << " at $" << itemPrice;

    Item transItem(transItemName, itemPrice);
    if (!dbManager::instance().PurchaseExists(Purchase(transDate, id, transItem, transQuantity, transSubtotal, transTotal)))
    {
        if (dbManager::instance().AddPurchase(Purchase(transDate, id, transItem, transQuantity, transSubtotal, transTotal)))
        {
            qDebug() << "Trans Date " << transDate;
             QMessageBox::information(NULL,"Status","Successfully Added Purchase to the Database","OK");
            emit purchaseAdded();
        }
        else
        {
            QMessageBox::information(NULL,"Status","Failed to Add Purchase to the Database","OK");
        }
    }
}

void AddNewPurchase::on_PushButton_CancelPurchase_clicked()
{
    close();
}
