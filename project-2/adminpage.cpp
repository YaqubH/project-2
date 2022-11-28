#include "adminpage.h"
#include "yesno.h"
#include "dbmanager.h"
#include "QDebug"
#include <iostream>
#include <string>
AdminPage::AdminPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminPage)
{
    ui->setupUi(this);
    // Sets default page to be the home page
    ui->stackedWidget->setCurrentWidget(ui->adminHomePage);
    setupConnections();

    // Sets the items for item combo box
    ShowItemsInComboBox(databaseObj.loadItemsOnly());
    ShowQuantityInComboBox();   // sets the number of quantities for qty combo box
    ShowCustomerTypeInComboBox();   // sets the types of customers in customer combo box
}

AdminPage::~AdminPage()
{
    delete ui;
}

void AdminPage::setupConnections()
{
    connect(ui->addCustomersBttn, &QPushButton::clicked, this, &AdminPage::ChangeToaddCustomers);
    connect(ui->addItemBttn,&QPushButton::clicked, this, &AdminPage::ChangeToaddItem);
    connect(ui->createPurchasesBtn, &QPushButton::clicked, this, &AdminPage::ChangeToCreatePurchasesPage);
    connect(ui->createPurchaseBackBtn, &QPushButton::clicked, this, &AdminPage::ChangeToAdminHomePage);

}

void AdminPage::ChangeToaddCustomers()
{
    ui->stackedWidget->setCurrentWidget(ui->addCustomer);
}

void AdminPage::ChangeToaddItem()
{
     ui->stackedWidget->setCurrentWidget(ui->addItem);
}


void AdminPage::on_deleteCustomersBttn_clicked()
{
    QSqlQuery data;
    QString deleteCustomer = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                                   tr("Customer name:"), QLineEdit::Normal,
                                                   QDir::home().dirName());

    data.prepare("DELETE FROM Customers WHERE name == '"+deleteCustomer+"'");

    if (data.exec() == false)
    {
        QMessageBox::about(this, "", "this customer does not exist in the database");
    }

    data.exec();
    if(data.exec())
    {
    QMessageBox::about(this, "", "The customer was deleted. double check if error occured");
    }

    /*else
    {
    QMessageBox::about(this, "", "This customer does not exist in the database.");
    }*/

 }

void AdminPage::on_addButton_clicked()
{
    QSqlQuery data;
    QString name = ui->addName ->text();
    QString ID = ui->addID->text();
    QString customerType = ui->typeComboBox_2->currentText();
    QString expDay = ui->dayComboBox_2->currentText();
    QString expMonth = ui->monthComboBox_2->currentText();
    QString expYear = ui->yearComboBox_2->currentText();
    bool check = false;

    for (int i = 0; i < ID.length(); i++)
    {
        if(ID[i] != '0' && ID[i] != '1' && ID[i] != '2' && ID[i] != '3' && ID[i] != '4'
                && ID[i] != '5' && ID[i] != '6' && ID[i] != '7' && ID[i] != '8' && ID[i] != '9')
        {
            check = true;
            break;
        }
    }

    if(check == true)
    {
        QMessageBox::about(this, "Invalid Entry", "There were invalid inputs for your ID. Please enter only numerals (0-9)");
         ui->stackedWidget->setCurrentWidget(ui->adminHomePage);
         return;
    }



    data.prepare("INSERT INTO Customers (name, ID, type, expMonth, expDay, expYear) VALUES('"+name+"', '"+ID+"', '"+customerType+"', '"+expMonth+"', '"+expDay+"', '"+expYear+"')");

    if(data.exec())
    {
        QMessageBox::about(this, "", "The Customer was added. be sure to double check if added correctly");
    }
    else
    {
        QMessageBox::about(this, "Error", "Database not found. double check connection");
    }
    ui->stackedWidget->setCurrentWidget(ui->adminHomePage);

}

void AdminPage::on_cancelButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->adminHomePage);
}


void AdminPage::on_addItemConfirm_clicked()
{
    QSqlQuery data;
    QString itemName = ui->addItemName->text();
    QString itemPrice = ui->addItemPrice->text();
    QString itemQuantity = ui->addItemQty->text();

    bool check1 = false;
    bool check2 = false;
    int count = 0;

    for (int i = 0; i < itemPrice.length(); i++)\
    {
        if(itemPrice[i] != '0' && itemPrice[i] != '1' && itemPrice[i] != '2' && itemPrice[i] != '3'
                && itemPrice[i] != '4' && itemPrice[i] != '5'
                && itemPrice[i] != '6' && itemPrice[i] != '7'
                && itemPrice[i] != '8' && itemPrice[i] != '9' && itemPrice[i] != '.')
        {
            check1 = true;
            break;
        }
        if(itemPrice[i] == '.')
        {
            count++;
        }

    }

    for (int i = 0; i < itemQuantity.length(); i++)\
    {
        if(itemQuantity[i] != '0' && itemQuantity[i] != '1' && itemQuantity[i] != '2' && itemQuantity[i] != '3'
                && itemQuantity[i] != '4' && itemQuantity[i] != '5'
                && itemQuantity[i] != '6' && itemQuantity[i] != '7'
                && itemQuantity[i] != '8' && itemQuantity[i] != '9')
        {
            check2 = true;
            break;
        }
    }

    if(check1 == true)
    {
        QMessageBox::about(this, "Invalid Input", "There was an invalid input. Please use only Numerals(0-9) and the . ");
        ui->stackedWidget->setCurrentWidget(ui->adminHomePage);
        return;
    }

    data.prepare("INSERT INTO inventory (item, price, quantity) VALUES ('"+itemName+"', '"+itemPrice+"', '"+itemQuantity+"')");

    if(check2 == true)
    {
        QMessageBox::about(this, "Invalid Input", "There was an invalid input. Please use only Numerals(0-9) and the . ");
        ui->stackedWidget->setCurrentWidget(ui->adminHomePage);
        return;
    }

    if(check1 == true && check2 == true)
    {
        QMessageBox::about(this, "Invalid Input", "both itemName and ItemQuantity have invalid inputs\nPlease use numerals and a . for the price\nPlease use only numerals for the quantity");
        ui->stackedWidget->setCurrentWidget(ui->adminHomePage);
        return;
    }

    if (count > 1)
    {
        QMessageBox::about(this,"", "Invalid input. Use only one . for price");
        ui->stackedWidget->setCurrentWidget(ui->adminHomePage);
        return;
    }


    if(data.exec())
    {
        QMessageBox::about(this, "", "The item was added. double check if error occured");
    }
    else
    {
        QMessageBox::about(this, "Error", "Database not found double check path to database");
    }
    ui->stackedWidget->setCurrentWidget(ui->adminHomePage);
}


void AdminPage::on_cancelButton_3_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->adminHomePage);
}

void AdminPage::on_deleteItemBttn_clicked()
{
    QSqlQuery data;
    QString deleteItem = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                               tr("Item to be deleted:"), QLineEdit::Normal,
                                               QDir::home().dirName());

    data.prepare("DELETE FROM inventory WHERE item == '"+deleteItem+"'");

    if (data.exec() == false)
    {
        QMessageBox::about(this, "", "this item does not exist in the database");
    }

    data.exec();
    if(data.exec() == true)
    {
    QMessageBox::about(this, "", "The item was deleted. double check if error occured");
    }


    /*{
    QMessageBox::about(this, "", "This item does not exist in the database.");
    }*/

}

void AdminPage::on_addCustomersBttn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->addCustomer);
}

void AdminPage::on_addItemBttn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->addItem);
}

void AdminPage::on_Close_clicked()
{
    this->close();
}


void AdminPage::ChangeToAdminHomePage()             // changes to admin home page
{
    ui->stackedWidget->setCurrentWidget(ui->adminHomePage);
}


// -------------------------STORY 7 CODE-------------------------------//

void AdminPage::ChangeToCreatePurchasesPage()        // changes to create purchases page
{
    ui->stackedWidget->setCurrentWidget(ui->CreatePurchasesPage);

    QLineEdit *lineEdit = new QLineEdit;             // Item is used for item combo box
    lineEdit->setPlaceholderText("SELECT ITEMS");    // Sets default date for combobox(will turn into lineedit)

    // Changes item combo box to be a line edit in order to set the current text to be empty
    ui->pickItemComboBox->setLineEdit(lineEdit);
    ui->pickItemComboBox->setCurrentText("");

    // Clears all the line edits
    ui->addNameLineEdit->clear();
    ui->addIdLineEdit->clear();
    ui->addMonthLineEdit->clear();
    ui->addDayLineEdit->clear();
    ui->addYearLineEdit->clear();

    // sets new place holder/line edit text for quantity combo box
    lineEdit = new QLineEdit;
    lineEdit->setPlaceholderText("SELECT QTY");
    ui->pickQtyLComboBox->setLineEdit(lineEdit);
    ui->pickQtyLComboBox->setCurrentText("");

     // sets new place holder/line edit text for customer combo box
    lineEdit = new QLineEdit;
    lineEdit->setPlaceholderText("SELECT TYPE");
    ui->customerTypeComboBox->setLineEdit(lineEdit);
    ui->customerTypeComboBox->setCurrentText("");
}

void AdminPage::ShowItemsInComboBox(QSqlQueryModel *model)  // sets the item combo box to be the item values from inventory table
{
    ui->pickItemComboBox->setModel(model);
}

// Sets the quantity combo box to be quantities: 1-100
void AdminPage::ShowQuantityInComboBox()
{
    int i;  // used for quantity items
    for(i = 1; i <=100; i++)
    {
        ui->pickQtyLComboBox->addItem(QString::number(i));
    }
}

// Sets the types of customers in customers combo box
void AdminPage::ShowCustomerTypeInComboBox()
{
    QString regular = "Regular";
    QString executive = "Executive";

    ui->customerTypeComboBox->addItem(regular);
     ui->customerTypeComboBox->addItem(executive);

}

// Story 7 slot, will allow purchases to happen after admin clicks on the button
void AdminPage::on_createPurchasePushBtn_clicked()
{
    QString name, item, type, id;
    int month, day, year, quantity;
    bool valid = true;
    QIntValidator v(100000, 200000, this);  // Used for checking id from line edit, will only accept id between 100000 to 200000
    int pos = 0;

    // INPUT - Obtains input from line edits/combo boxes from each variables' respective values
    name = ui->addNameLineEdit->text();
    item = ui->pickItemComboBox->currentText();
    month = ui->addMonthLineEdit->text().toInt();
    day = ui->addDayLineEdit->text().toInt();
    year = ui->addYearLineEdit->text().toInt();
    quantity = ui->pickQtyLComboBox->currentText().toInt();
    type = ui->customerTypeComboBox->currentText();
    id = ui->addIdLineEdit->text();

    // PROCESSING - Checks for valid conditions
    // Validates id, if the value entered does not meet the specifications, then will set valid to false
    if(!v.validate(id, pos))
    {
        valid = false;
    }
    // First check if any of the line edits/combo boxes are empty, if they are then valid is false
    if(name == "" || item == "" || type == "" || ui->addMonthLineEdit->text() == "" ||
       ui->addDayLineEdit->text() == "" || ui->addYearLineEdit->text() == "" ||
       ui->pickQtyLComboBox->currentText() == "" || id == "")
    {
        valid = false;
    }

    // Checks for valid month, if valid month, then will check for valid days of each month
    if(month < 1 || month > 12)
    {
        valid = false;
    }
    else
    {
        if(month == 2)
        {
            if(day < 1 || day > 28)
            {
                valid = false;
            }
        }
        else if(month % 2 == 0)
        {
            if(day < 1 || day > 31)
            {
                valid = false;
            }
        }
        else
        {
            if(day < 1 || day > 30)
            {
                valid = false;
            }
        }
    }

    // Checks for valid year
    if(year < 2019 || year > 2023)
    {
        valid = false;
    }


    // If the final value of valid is false, then it will display an error message
    // Otherwise, the new customer purchase will be stored in the database along with a successful message
    if(valid == false)
    {
       QMessageBox::warning(this, "Invalid", "Please input valid information");
    }
    else
    {
        databaseObj.AddToCustomersTable(name, id, type, month, day, year);  // adds new info to customers table
        databaseObj.AddToDailySalesReport(id, item, quantity);              // adds new info to dailySalesReport table
        QMessageBox::information(this, "Success", "Purchase has been created");
        AdminPage::ChangeToAdminHomePage();
    }
}
//---------------------END of story 7 code ----------------------------------------//


void AdminPage::on_ViewCustomers_clicked()
{
    QSqlQueryModel* modal=new QSqlQueryModel();

    QSqlQuery* view=new QSqlQuery();

    view->prepare("SELECT * FROM Customers ORDER by name ASC");

    view->exec();
    modal->setQuery(*view);

    ui->tableView->setModel(modal);
    ui->tableView->setColumnWidth(0, 120);

}

void AdminPage::on_ChangeMembership_clicked()
{
    QSqlQuery qry;
    yesno yesno;
    bool confirm = false;

  QString search = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                               tr("Customer name:"), QLineEdit::Normal);
    qDebug() << search;

    qry.prepare("SELECT name, type FROM Customers WHERE name == '" + search + "'");
    QString data;

    data = qry.exec();
    qry.last();
    int rowCount = qry.at() + 1;
    qDebug() << "getNoteCounts = " << rowCount;
    if (rowCount > 0)
    {
        yesno.setModal(true);
        yesno.exec();
        qDebug() << qry.record().count();

        confirm=yesno.getData();
        if(confirm)
        {
            QSqlQuery updateExe;
            QSqlQuery updateReg;
            QSqlQuery find;

            qDebug() << "confirm";

//            std::string custType = "";
//            QString aType;

//            while (qry.next())
//            {
//                QSqlRecord record = qry.record();
//                aType = qry.record().value((type).toString();
//                qDebug() << custType;

//                qDebug() << "Type : " << record.value(type).toString();
//            }

            updateExe.prepare("update customers set type = 'Executive' where name = '" + search + "'");
            updateReg.prepare("update customers set type = 'Regular'   where name = '" + search + "'");
            find.prepare("select type from customers where name = '" + search + "'");
            find.exec();
            if(find.next())
            {
                QString aType;
                aType = qry.value(0).toString();
            }

            QSqlRecord record;
            record = qry.record();

            while(qry.next())
            {
                record.value(data).toString();
            }



            if(true)
            {
                updateReg.exec();
                qDebug() << "updating to regular";
            }
            else
            {
                updateExe.exec();
                qDebug() << "updating to executive";
            }




            QSqlQueryModel* modal=new QSqlQueryModel();

            QSqlQuery* view=new QSqlQuery();

            view->prepare("SELECT * FROM Customers ORDER by name ASC");

            view->exec();
            modal->setQuery(*view);

            ui->tableView->setModel(modal);
            ui->tableView->setColumnWidth(0, 120);
        }
        else
        {
            ui->adminHomePage->show();
        }
    }
    else
    {
        QMessageBox::warning(this, "Invalid", "That Customer does not exist.");
    }


}
