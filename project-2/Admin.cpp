#include "Admin.h"
#include "ui_Admin.h"
#include "mainwindow.h"
#include "ConfirmRemoval.h"
#include "ModifyItem.h"
Admin::Admin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Admin),
    mModel(new MemberModel(dbManager::instance().GetAllMembers())),
    iModel(new ItemModel(dbManager::instance().GetAllItems())),
    uModel(new UpgradeModel(dbManager::instance().GetUpgrades())),
    dModel(new DowngradeModel(dbManager::instance().GetDowngrades())),
    selectedID(0),
    selectedName("")
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->membersTable->setModel(mModel);
    ui->itemsTable->setModel(iModel);
    ui->UpgradeTable->setModel(uModel);
    ui->DowngradeTable->setModel(dModel);
    ui->membersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->itemsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->PushButton_RemoveMember->setEnabled(false);
    ui->PushButton_RemoveItem->setEnabled(false);
    ui->PushButton_ModifyItem->setEnabled(false);
    ui->membersTable->resizeColumnsToContents();
    ui->itemsTable->resizeColumnsToContents();
    ui->UpgradeTable->resizeColumnsToContents();
    ui->DowngradeTable->resizeColumnsToContents();
    ui->itemsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->UpgradeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->DowngradeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

Admin::~Admin()
{
    delete ui;
    delete mModel;
    delete iModel;
}

void Admin::on_PushButton_DisplayMembers_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Admin::on_PushButton_DisplayInventory_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Admin::on_PushButton_DisplayUpAndDown_clicked()
{
    int upgradeCount = uModel->rowCount();
    int downgradeCount = dModel->rowCount();
    int totalCount = upgradeCount + downgradeCount;
    ui->Label_MembershipUpgrades->setText("Suggested Membership " +QString::number(upgradeCount) +" To Upgrades");
    ui->Label_MembershipDowngrades->setText("Suggested Membership " +QString::number(downgradeCount) +" To Downgrade");
    ui->Label_ConversionNum->setText("Number of Recommened Conversions: "+QString::number(totalCount));
    qDebug() << "Received signal, updating upgrades/downgrades";
    uModel->setList(dbManager::instance().GetUpgrades());
    dModel->setList(dbManager::instance().GetDowngrades());
    ui->UpgradeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->DowngradeTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->stackedWidget->setCurrentIndex(3);
}

void Admin::on_PushButton_BackToMain_clicked()
{
    this->close();
    MainWindow* mainWindow = new MainWindow;
    mainWindow->show();
}

void Admin::on_PushButton_AddMember_clicked()
{
    AddNewMembers *addWin;
    addWin = new AddNewMembers(this);
    connect(addWin, SIGNAL(memberAdded()), this, SLOT(updateMemberView()));
    connect(addWin, SIGNAL(purchaseAdded()), this, SLOT(updateMemberView()));
    addWin->setModal(true);
    addWin->exec();
    delete addWin;
    updateMemberView();
}

void Admin::on_PushButton_RemoveMember_clicked()
{
    ConfirmRemoval *remWin;
    QList<Member> mList;
    MemberModel *tempModel;

    if(selectedID != 0 && dbManager::instance().MemberExists(selectedID))
    {
        mList.append(Member(dbManager::instance().GetMember(selectedID)));
        tempModel = new MemberModel(mList);
        remWin = new ConfirmRemoval(this);
        connect(this, SIGNAL(sendModel(MemberModel*)), remWin, SLOT(setMemberView(MemberModel*)));
        connect(remWin, SIGNAL(removalConfirmed()), this, SLOT(removeMember()));
        emit sendModel(tempModel);
        remWin->exec();
        delete tempModel;
        delete remWin;
    }
    updateMemberView();
}

void Admin::on_PushButton_BackToHome_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Admin::on_PushButton_BackToHome2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Admin::on_PushButton_BackToHome3_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
void Admin::removeMember()
{

    if(dbManager::instance().MemberExists(selectedID))
    {
        if(dbManager::instance().RemoveMember(dbManager::instance().GetMember(selectedID)))
        {
            updateMemberView();
        }
    }
}
void Admin::on_membersTable_clicked(const QModelIndex &index)
{
    QModelIndex idIndex;

    idIndex = ui->membersTable->model()->index(index.row(), 1);
    selectedID = ui->membersTable->model()->data(idIndex).toInt();
    ui->PushButton_RemoveMember->setEnabled(true);
}

void Admin::on_itemsTable_clicked(const QModelIndex &index)
{
    QModelIndex nameIndex;
    nameIndex = ui->itemsTable->model()->index(index.row(), 0);
    selectedName = ui->itemsTable->model()->data(nameIndex).toString();
    qDebug() << selectedName;
    ui->PushButton_RemoveItem->setEnabled(true);
    ui->PushButton_ModifyItem->setEnabled(true);
}
void Admin::removeItem()
{
    if(dbManager::instance().RemoveItem(dbManager::instance().GetItem(selectedName)))
    {
        updateItemView();
    }
}

void Admin::on_PushButton_AddItem_clicked()
{
    AddNewItem *addWin;
    addWin = new  AddNewItem(this);
    connect(addWin, SIGNAL(itemAdded()), this, SLOT(updateItemView()));
    addWin->setModal(true);
    addWin->exec();
    delete addWin;
}

void Admin::on_PushButton_RemoveItem_clicked()
{
    ConfirmRemoval *remWin;
    QList<Item> iList;
    ItemModel *tempModel;

    if(selectedName != "")
    {
        iList.append(Item(dbManager::instance().GetItem(selectedName)));
        tempModel = new ItemModel(iList);
        remWin = new ConfirmRemoval(this);
        connect(this, SIGNAL(sendModel(ItemModel*)), remWin, SLOT(setItemView(ItemModel*)));
        connect(remWin, SIGNAL(removalConfirmed()), this, SLOT(removeItem()));
        emit sendModel(tempModel);
        remWin->exec();
        delete tempModel;
        delete remWin;
    }
}
void Admin::on_PushButton_ModifyItem_clicked()
{
    ModifyItem *modWin;
    if(selectedName != "")
    {
        Item selectedItem = dbManager::instance().GetItem(selectedName);
        modWin = new ModifyItem(this,selectedItem);
        connect(modWin, SIGNAL(ModifyConfirmed()), this, SLOT(updateItemView()));
        modWin->setModal(true);
        modWin->exec();
        delete modWin;
    }
}

void Admin::updateItemView()
{
    qDebug() << "Received signal, updating item view";
    iModel->setList(dbManager::instance().GetAllItems());
}

void Admin::updateMemberView()
{
    qDebug() << "Received signal, updating member view";
    mModel->setList(dbManager::instance().GetAllMembers());
}
