#ifndef ADMINPAGE_H
#define ADMINPAGE_H

#include <QMainWindow>
#include "dbmanager.h"
#include "ui_adminpage.h"
#include <QtSql>
#include <QSqlQuery>

#include <QMessageBox>
#include <QInputDialog>
#include <QIntValidator>
namespace Ui {
class AdminPage;
}

class AdminPage : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminPage(QWidget *parent = nullptr);
    ~AdminPage();

     void setupConnections();

     void ChangeToaddCustomers();

     void ChangeToaddItem();

     void ChangeToAdminHomePage();

     // Story 7 code
     void ChangeToCreatePurchasesPage();                // Changes to Create Purchases Page, will clear/set all the line edits, combo boxes

     void ShowItemsInComboBox(QSqlQueryModel *model);   // Shows item names in item combo box

     void ShowQuantityInComboBox();                     // Shows quantities in qty combo box

     void ShowCustomerTypeInComboBox();                 // Shows customer type in customer combo box
     // End story 7 code //

private slots:

     void on_deleteCustomersBttn_clicked();

     void on_cancelButton_clicked();

     void on_cancelButton_3_clicked();

     void on_addItemConfirm_clicked();

     void on_addButton_clicked();

     void on_deleteItemBttn_clicked();

     void on_addCustomersBttn_clicked();

     void on_addItemBttn_clicked();

     void on_Close_clicked();

     void on_createPurchasePushBtn_clicked();       // Story 7 slot, will allow purchases to happen after admin clicks on the button

     void on_ViewCustomers_clicked();

     void on_ChangeMembership_clicked();

private:
    Ui::AdminPage *ui;
    DBManager databaseObj;
};

#endif // ADMINPAGE_H
