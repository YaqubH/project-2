#include "storemanagerpage.h"
#include "dbmanager.h"


StoreManagerPage::StoreManagerPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StoreManagerPage)
{
    ui->setupUi(this);
    setupConnections(); // Calls setupConnections() function to set up connections
    ui->stackedWidget->setCurrentWidget(ui->storeManagerHomePage);  // If a StoreManager class object is called, then the dafault page will be the home page

    // When the constructor is called, the ShowDatesInComboBox() function is called to automatically set the combo box for date input
     ShowDatesInComboBox(databaseObj.loadDateEntriesOnly());

    // When the constructor is called, the ShowItemsInComboBox() function is called to automatically set the combo box for item name input
     ShowItemsInComboBox(databaseObj.loadItemsOnly());

     // When the constructor is called, the ShowNamesInComboBox() function is called to automatically set the combo box for member name input
      ShowNamesInComboBox(databaseObj.loadNamesOnly());

      // When the constructor is called, the ShowExpMonthsInComboBox() function is called to automatically set the combo box for expiration month inputs
      ShowExpMonthsInComboBox(databaseObj.loadExpirationMonthsIntoComboBox());
}

StoreManagerPage::~StoreManagerPage()
{
    delete ui;
}


//------------------------------------------------------------STORY 1 CODE---------------------------------------------------------//
void StoreManagerPage::showTable(QSqlQueryModel *model)
{
    ui->tableView->setModel(model);     // showTable() shows the QSqlQueryModel database model to the tableview
}

/*******************************************************
 * ShowDatesInComboBox(QSqlQueryModel *model) -
 *     This function sets the dates combo box widget
 *     to be whatever the return model of type
 *     QSqlQueryModel is
 *******************************************************/
void StoreManagerPage::ShowDatesInComboBox(QSqlQueryModel *model)
{
    ui->dateComboBox->setModel(model);
}

/*******************************************************
 * on_dateEnterBtn_clicked() -
 *    This function obtains a date from a widget.
 *    Based on the date, it will calculate the
 *    total revenue for that date. In addition, it will
 *    display all the table information for the given date
 *    onto the table view
 *******************************************************/
void StoreManagerPage::on_dateEnterBtn_clicked()
{
    QString dateEntered;
    QString totalRevenueString;
    QString stringExecutiveCount;
    QString stringRegularCount;
    double revenue;
    int executiveCount = 0;
    int regularCount = 0;

    // INPUT - Obtains input from the dateComboBox widget
    dateEntered =  ui->dateComboBox->currentText();

    // Calls on databaseObj obejct to calculate the total revenue for the specified date
    revenue = databaseObj.GetTotalRevenue(dateEntered);
    totalRevenueString = QString::number(revenue, 'f', 2);  // Converts revenue(double) to string to display onto line edit

    // Shows the table for the date entered, and sets the totalRevenue line edit to be the revue for the given date
    showTable(databaseObj.loadEntriesByDate(dateEntered));
    ui->totalRevenueLineEdit->setText("$"+totalRevenueString);

    // PROCESSING & OUTPUT - Gets the number of executive/regular members. Converts both int variables to string,
    // output them to their corresponding line edits
    executiveCount = databaseObj.ReturnMemberTypeCount(dateEntered, "Executive");
    regularCount = databaseObj.ReturnMemberTypeCount(dateEntered, "Regular");
    stringExecutiveCount = QString::number(executiveCount);
    stringRegularCount = QString::number(regularCount);

    ui->execMembersLineEdit->setText(stringExecutiveCount);
    ui->regMembersLineEdit->setText(stringRegularCount);

}

/*******************************************************
 * on_displayByExecBtn_clicked() -
 *    This function displays the executive members
 *    and their information onto the table.
 *    Depending on the date entered, either all of their
 *    purchases or certain purchases on a given day will
 *    be displayed
 *******************************************************/
void StoreManagerPage::on_displayByExecBtn_clicked()
{
     QString dateEntered;

     // INPUT - Obtains input from the dateComboBox widget
     dateEntered =  ui->dateComboBox->currentText();

     showTable(databaseObj.loadEntriesByType(dateEntered, "Executive"));
}

/*******************************************************
 * on_displayByExecBtn_clicked() -
 *    This function displays the regular members
 *    and their information onto the table.
 *    Depending on the date entered, either all of their
 *    purchases or certain purchases on a given day will
 *    be displayed
 *******************************************************/
void StoreManagerPage::on_displayByRegBtn_clicked()
{
    QString dateEntered;

    // INPUT - Obtains input from the dateComboBox widget
    dateEntered =  ui->dateComboBox->currentText();

    showTable(databaseObj.loadEntriesByType(dateEntered, "Regular"));
}

/*******************************************************
* on_displayAllInfoBtn_clicked()-
*   After a store manager clicks on the
*   displayAllInfoBtn widget, then the table in
*   dailySalesReportPage will contain ALL the information
*   throughout the week. Moreover, the total revenue
*   generated as well as the # of executive/regular members
*   will be shown
*******************************************************/
void StoreManagerPage::on_displayAllInfoBtn_clicked()
{
    QString totalRevenueString;
    double revenue;
    QString stringExecutiveCount;
    QString stringRegularCount;
    int executiveCount = 0;
    int regularCount = 0;

    // PROCESSING - Obtains total revenue for the entire week.
    // Sets the table to show all the information from the dailySalesReport table from the database
    // Sets the total revenue line edit to be the grand total of all purchases
    revenue = databaseObj.GetTotalRevenue("");
    totalRevenueString = QString::number(revenue, 'f', 2);

    showTable(databaseObj.loadEntries());
    ui->dateComboBox->setCurrentText("");   // Once the 'DISPLAY ALL' button is clicked, then the currentText of the combo box will be empty

    ui->totalRevenueLineEdit->setText("$"+totalRevenueString);

    // PROCESSING & OUTPUT - Gets the number of executive/regular members by calling db manager function
    // Converts both int variables to string,
    // output them to their corresponding line edits
    executiveCount = databaseObj.ReturnMemberTypeCount("", "Executive");
    regularCount = databaseObj.ReturnMemberTypeCount("", "Regular");
    stringExecutiveCount = QString::number(executiveCount);
    stringRegularCount = QString::number(regularCount);

    ui->execMembersLineEdit->setText(stringExecutiveCount);
    ui->regMembersLineEdit->setText(stringRegularCount);
}


//------------------------------------------------------------STORY 2 & 3 CODE---------------------------------------------------------//
/*******************************************************
* showTotalPurchasesTable(QSqlQueryModel *model) -
*   This function will set the totalPurchasesTableView
*   to be the model that is passsed in
*******************************************************/
void StoreManagerPage::showTotalPurchasesTable(QSqlQueryModel *model)
{
    ui->totalPurchasesTableView->setModel(model);
}

/*******************************************************
* on_displayByItemBtn_clicked() -
*   This functions calls on showTablePurchasesTable()
*   function in order to load the total purchases regarding
*   item information
*******************************************************/
void StoreManagerPage::on_displayByItemBtn_clicked()
{
    showTotalPurchasesTable(databaseObj.loadTotalMemberOrItemPurchases("Items"));
    ui->itemComboBox->setCurrentText("");
    ui->nameComoboBox->setCurrentText("");
}

/*******************************************************
* on_displayByAllMembersBtn_clicked() -
*   This functions calls on showTablePurchasesTable()
*   function in order to load the total purchases regarding
*   members' information
*******************************************************/
void StoreManagerPage::on_displayByAllMembersBtn_clicked()
{
    showTotalPurchasesTable(databaseObj.loadTotalMemberOrItemPurchases("Members"));
    ui->itemComboBox->setCurrentText("");
    ui->nameComoboBox->setCurrentText("");
}


//------------------------------------------------------STORY 9 and 10 Code----------------------------------------------//

void StoreManagerPage::ShowItemsInComboBox(QSqlQueryModel *model)
{
    ui->itemComboBox->setModel(model);
}

void StoreManagerPage::ShowNamesInComboBox(QSqlQueryModel *model)
{
    ui->nameComoboBox->setModel(model);
}

void StoreManagerPage::on_enterItemPushBtn_clicked()
{
    QString itemName = ui->itemComboBox->currentText();
    if(itemName == "")
    {
       QMessageBox::warning(this, "Warning", "Please enter an item");
    }
    else
    {
       showTotalPurchasesTable(databaseObj.ShowInfoForOneItem(itemName));
    }
}

void StoreManagerPage::on_enterNamePushBtn_clicked()
{
    QString memberName = ui->nameComoboBox->currentText();
    if(memberName == "")
    {
       QMessageBox::warning(this, "Warning", "Please enter a name");
    }
    else
    {
       showTotalPurchasesTable(databaseObj.ShowInfoForOneMember(memberName));
    }
}

//--------------------------------------------------------------------------------------------------//

// -----------------------------------CONNECTIONS FUNCTIONS-------------------------------------------------//
// setupConnections() - Sets up connections so that different pages open based on various clicked buttons
void StoreManagerPage::setupConnections()
{
    connect(ui->dailySalesReportsPushBtn, &QPushButton::clicked, this, &StoreManagerPage::ChangeToDailySalesReportsPage);
    connect(ui->dailySalesBackBtn,&QPushButton::clicked, this, &StoreManagerPage::GoToHomePage);
    connect(ui->totalPurchasesPushBtn, &QPushButton::clicked, this, &StoreManagerPage::ChangeToTotalPurchasesPage);
    connect(ui->totalPurchasesBackBtn, &QPushButton::clicked, this, &StoreManagerPage::GoToHomePage);
    connect(ui->rebatePushBtn, &QPushButton::clicked, this, &StoreManagerPage::ChangeToRebateAndExpPage);
    connect(ui->rebateBackBtn, &QPushButton::clicked, this, &StoreManagerPage::GoToHomePage);
}

/*******************************************************
 * ChangeToDailySalesReportsPage() -
 *  This function changes the page to daily sales
 *  reports page after user clicks "Go to daily sales
 *  report" button in store manager home page. Will show
 *  the entire daily sales report table
 *******************************************************/
void StoreManagerPage::ChangeToDailySalesReportsPage()
{
    QString totalRevenueString;
    double revenue;
    QString stringExecutiveCount;
    QString stringRegularCount;
    int executiveCount = 0;
    int regularCount = 0;
    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setPlaceholderText("SELECT DATE");    // Sets default date for combobox(will turn into lineedit)

    // PROCESSING - Obtains total revenue for the entire week.
    // Sets the table to show all the information from the dailySalesReport table from the database
    // Sets the total revenue line edit to be the grand total of all purchases
    revenue = databaseObj.GetTotalRevenue("");
    totalRevenueString = QString::number(revenue, 'f', 2);

    showTable(databaseObj.loadEntries());
    ui->stackedWidget->setCurrentWidget(ui->dailySalesReportsPage);

    // Changes combo box to be a line edit in order to set the current text to be empty
    ui->dateComboBox->setLineEdit(lineEdit);
    ui->dateComboBox->setCurrentText("");


    ui->totalRevenueLineEdit->setText("$"+totalRevenueString);

    // PROCESSING & OUTPUT - Gets the number of executive/regular members by calling db manager function
    // Converts both int variables to string,
    // output them to their corresponding line edits
    executiveCount = databaseObj.ReturnMemberTypeCount("", "Executive");
    regularCount = databaseObj.ReturnMemberTypeCount("", "Regular");
    stringExecutiveCount = QString::number(executiveCount);
    stringRegularCount = QString::number(regularCount);

    ui->execMembersLineEdit->setText(stringExecutiveCount);
    ui->regMembersLineEdit->setText(stringRegularCount);

}

void StoreManagerPage::GoToHomePage()
{   // Sets current widget to home page
    ui->stackedWidget->setCurrentWidget(ui->storeManagerHomePage);
}

// Sets current widget to total purchases page
void StoreManagerPage::ChangeToTotalPurchasesPage()
{
    QLineEdit *lineEdit = new QLineEdit;             // Item is used for item combo box
    lineEdit->setPlaceholderText("SELECT ITEMS");    // Sets default date for combobox(will turn into lineedit)

    // Changes item combo box to be a line edit in order to set the current text to be empty
    ui->itemComboBox->setLineEdit(lineEdit);
    ui->itemComboBox->setCurrentText("");

    // Changes name combo box to be a line edit in order to set the current text to be empty
    lineEdit = new QLineEdit;
    lineEdit->setPlaceholderText("SELECT MEMBERS");
    ui->nameComoboBox->setLineEdit(lineEdit);
    ui->nameComoboBox->setCurrentText("");

    ui->stackedWidget->setCurrentWidget(ui->totalPurchasesPage);

    QString totalRevenueString;
    double revenue;
    revenue = databaseObj.GetTotalRevenue("");
    totalRevenueString = QString::number(revenue, 'f', 2);

    ui->grandTotalLineEdit->setText("$"+totalRevenueString);
}

//---------------------------------------------------Story 4 and 5 code-----------------------------------------------//
// Changes to rebate and expiration page and will set all the widgets to be their intended features
void StoreManagerPage::ChangeToRebateAndExpPage()
{
    ui->stackedWidget->setCurrentWidget(ui->rebateAndExpPage);

    QLineEdit *lineEdit = new QLineEdit;             // Item is used for item combo box
    lineEdit->setPlaceholderText("SELECT MONTHS");    // Sets default date for combobox(will turn into lineedit)

    // Sets the expiration month combo box to be empty
    ui->expMonthComboBox->setLineEdit(lineEdit);
    ui->expMonthComboBox->setCurrentText("");
}

// Will set the rebate and exp table view to be the QSqlQueryModel's model
void StoreManagerPage::ShowRebateAndExpMonthTable(QSqlQueryModel *model)
{
    ui->rebateAndExpTableView->setModel(model);
}

// This function will show the expiration months for combo box entries
void StoreManagerPage::ShowExpMonthsInComboBox(QSqlQueryModel *model)
{
    ui->expMonthComboBox->setModel(model);
}

 // Story 4 slot, will show the rebate table once button is clicked
void StoreManagerPage::on_displayRebatePushBtn_clicked()
{
    ShowRebateAndExpMonthTable(databaseObj.LoadRebateModel());

    ui->expMonthComboBox->setCurrentText("");
}

 // Story 5 slot, will display members whose memberships expire on a given month
void StoreManagerPage::on_expMonthEnterBtn_clicked()
{
    QString expMonth = ui->expMonthComboBox->currentText(); // Obtains month input from combo box

    // if month is empty, then an error message will appear
    if(expMonth == "") {
        QMessageBox::warning(this, "Warning", "Please enter a month");
    }
    // Otherwise, the table view will be set so that it contains information from a given exp month
    else {
        ShowRebateAndExpMonthTable(databaseObj.loadMemberInfoFromExpMonth(expMonth));
    }
}
//------------------------------------------------END OF STORY 4 and 5-----------------------------------------------//
