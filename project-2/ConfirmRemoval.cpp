#include "ConfirmRemoval.h"
#include "ui_ConfirmRemoval.h"

ConfirmRemoval::ConfirmRemoval(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmRemoval)
{
    ui->setupUi(this);
     ui->tableView->resizeColumnsToContents();
}

ConfirmRemoval::~ConfirmRemoval()
{
    delete ui;
}

void ConfirmRemoval::setMemberView(MemberModel *model)
{
    ui->tableView->setModel(model);
     ui->tableView->resizeColumnsToContents();
}

void ConfirmRemoval::setItemView(ItemModel *model)
{
    ui->tableView->setModel(model);
     ui->tableView->resizeColumnsToContents();
       ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
void ConfirmRemoval::on_PushButton_ConfirmRemoval_clicked()
{
    emit removalConfirmed();
    close();
}

void ConfirmRemoval::on_PushButton_BackToAdmin_clicked()
{
    close();
}
