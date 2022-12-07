#include "Manager.h"
#include "ui_Manager.h"
#include "mainwindow.h"
#include "mainwindow.h"
Manager::Manager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Manager),
    tModel(new PurchaseModel(dbManager::instance().GetAllPurchases())),
    pModel(new ProxyModel),
    pRModel(new ProxyModel),
    eModel(new ExpModel(dbManager::instance().GetExpirations(0))),
    rModel(new RebateModel(dbManager::instance().GetRebates()))
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    if(dbManager::instance().GetValidDates(minDate, maxDate))
    {
        ui->DateEdit->setMinimumDate(minDate);
        ui->DateEdit->setMaximumDate(maxDate);
    }
    else
    {
        qDebug() << "Error setting min/max dates";
        ui->DateEdit->setEnabled(false);
    }

    QStringList months;
    months << "January" << "February" << "March" << "April" << "May"
           << "June" << "July" << "August" << "September" << "October"
           << "November" << "December";
    pModel->setSourceModel(tModel);
    pModel->setSortRole(Qt::UserRole);
    pRModel->setSourceModel(rModel);
    pRModel->setSortRole(Qt::UserRole);
    ui->dailyView->setModel(pModel);
    ui->dailyView->setSortingEnabled(true);
    ui->dailyView->sortByColumn(0, Qt::AscendingOrder);
    ui->MemberTypeComboBox->addItem("Regular");
    ui->MemberTypeComboBox->addItem("Executive");
    ui->ExpTable->setModel(eModel);
    ui->ExpirationMonthComboBox->addItems(months);
    ui->rebateTable->setModel(pRModel);
    ui->rebateTable->setSortingEnabled(true);
    ui->rebateTable->sortByColumn(2, Qt::AscendingOrder);
    ui->dailyView->resizeColumnsToContents();
    ui->rebateTable->resizeColumnsToContents();
    ui->rebateTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ExpTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    updateSalesTotals(dbManager::instance().CalcGrossSales());
    updateMembersTotals(dbManager::instance().GetTotalShoppers(REGULAR),
                        dbManager::instance().GetTotalShoppers(EXECUTIVE));
    updateQuantityTotals(dbManager::instance().GetTotalQuantity());
}

Manager::~Manager()
{
    delete ui;
}
void Manager::updateSalesTotals(float grossSales)
{
    float taxAmt, netSales;

    taxAmt = grossSales * TAX_RATE;
    netSales = grossSales + taxAmt;

    ui->Label_SubtotalOutput->setText("$" +QString::number(grossSales, 'f', 2));
    ui->Label_TaxOutput->setText("$"+ QString::number(taxAmt, 'f', 2));
    ui->Label_TotalOutput->setText("$"+ QString::number(netSales, 'f', 2));
}
void Manager::updateQuantityTotals(int totalQuantity)
{
    ui->Label_PurchaseQuantityOutput->setText(QString::number(totalQuantity));
}
void Manager::updateMembersTotals(int regShoppers, int exShoppers)
{
    ui->Label_ExecutiveShoppersOutput->setText(QString::number(exShoppers));
    ui->Label_RegularShoppersOutput->setText(QString::number(regShoppers));
}

void Manager::on_PushButton_DisplaySalesInfo_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Manager::on_PushButton_DisplayMemberInfo_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    rModel->setList(dbManager::instance().GetRebates());
}

void Manager::on_PushButton_BackToMain_clicked()
{
    this->close();
    MainWindow* mainWindow = new MainWindow;
    mainWindow->show();
}
void Manager::on_PushButton_BackToHome_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void Manager::on_PushButton_BackToHome2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Manager::on_PushButton_DateFilter_clicked()
{
    QDate dateFilter;

    dateFilter = ui->DateEdit->date();
    pModel->setPurchaseDate(dateFilter);
    updateSalesTotals(dbManager::instance().CalcGrossSalesByDate(dateFilter));
    updateQuantityTotals(dbManager::instance().GetTotalQuantityByDate(dateFilter));
    updateMembersTotals(dbManager::instance().GetTotalShoppersByDate(dateFilter, REGULAR),
                        dbManager::instance().GetTotalShoppersByDate(dateFilter, EXECUTIVE));
}

void Manager::on_PushButton_ItemFilter_clicked()
{
    QString itemFilter;

    itemFilter = ui->LineEdit_ItemName->text();
    pModel->setItemName(itemFilter);
    updateSalesTotals(dbManager::instance().CalcGrossSalesByItem(itemFilter));
     updateQuantityTotals(dbManager::instance().GetTotalQuantityByItem(itemFilter));
    updateMembersTotals(dbManager::instance().GetTotalShoppersByItem(itemFilter, REGULAR),
                        dbManager::instance().GetTotalShoppersByItem(itemFilter, EXECUTIVE));

}

void Manager::on_PushButton_MemberIDFilter_clicked()
{
    int idFilter;

    idFilter = ui->MemberIDSpinBox->value();
    pModel->setMemberID(idFilter);
    updateSalesTotals(dbManager::instance().CalcGrossSalesByMember(idFilter));
     updateQuantityTotals(dbManager::instance().GetTotalQuantityByMember(idFilter));
    ui->Label_RegularShoppersOutput->setText("");
    ui->Label_ExecutiveShoppersOutput->setText("");
}

void Manager::on_PushButton_MemberName_clicked()
{
    QString mName;

    mName = ui->LineEdit_MemberName->text();
    pModel->setMemberName(mName);
    updateSalesTotals(dbManager::instance().CalcGrossSalesByMember(mName));
     updateQuantityTotals(dbManager::instance().GetTotalQuantityByMember(mName));
    ui->Label_RegularShoppersOutput->setText("");
    ui->Label_ExecutiveShoppersOutput->setText("");
}

void Manager::on_PushButton_MemberType_clicked()
{
    MemberType mType;

    if(ui->MemberTypeComboBox->currentText() == "Regular")
    {
        mType = REGULAR;
        updateMembersTotals(dbManager::instance().GetTotalShoppers(REGULAR), 0);
    }
    else
    {
        mType = EXECUTIVE;
        updateMembersTotals(0, dbManager::instance().GetTotalShoppers(EXECUTIVE));
    }

    pModel->setMemberType(mType);
    updateSalesTotals(dbManager::instance().CalcGrossSalesByMember(mType));
     updateQuantityTotals(dbManager::instance().GetTotalQuantityByMember(mType));
}

void Manager::on_PushButton_ResetFilter_clicked()
{
    ui->LineEdit_ItemName->clear();
    ui->LineEdit_MemberName->clear();

    pModel->resetFilter();
    updateSalesTotals(dbManager::instance().CalcGrossSales());
    updateQuantityTotals(dbManager::instance().GetTotalQuantity());
    updateMembersTotals(dbManager::instance().GetTotalShoppers(REGULAR),
                        dbManager::instance().GetTotalShoppers(EXECUTIVE));
}

void Manager::on_PushButton_MemberExpiration_clicked()
{
    if(ui->ExpirationMonthComboBox->currentText() == "January")
    {
        eModel->setList(dbManager::instance().GetExpirations(1));
    }
    else if(ui->ExpirationMonthComboBox->currentText() == "Febrauary")
    {
        eModel->setList(dbManager::instance().GetExpirations(2));
    }
    else if(ui->ExpirationMonthComboBox->currentText() == "March")
    {
        eModel->setList(dbManager::instance().GetExpirations(3));
    }
    else if(ui->ExpirationMonthComboBox->currentText() == "April")
    {
        eModel->setList(dbManager::instance().GetExpirations(4));
    }
    else if(ui->ExpirationMonthComboBox->currentText() == "May")
    {
        eModel->setList(dbManager::instance().GetExpirations(5));
    }
    else if(ui->ExpirationMonthComboBox->currentText() == "June")
    {
        eModel->setList(dbManager::instance().GetExpirations(6));
    }
    else if(ui->ExpirationMonthComboBox->currentText() == "July")
    {
        eModel->setList(dbManager::instance().GetExpirations(7));
    }
    else if(ui->ExpirationMonthComboBox->currentText() == "August")
    {
        eModel->setList(dbManager::instance().GetExpirations(8));
    }
    else if(ui->ExpirationMonthComboBox->currentText() == "September")
    {
        eModel->setList(dbManager::instance().GetExpirations(9));
    }
    else if(ui->ExpirationMonthComboBox->currentText() == "October")
    {
        eModel->setList(dbManager::instance().GetExpirations(10));
    }
    else if(ui->ExpirationMonthComboBox->currentText() == "November")
    {
        eModel->setList(dbManager::instance().GetExpirations(11));
    }
    else
    {
        eModel->setList(dbManager::instance().GetExpirations(12));
    }
}
