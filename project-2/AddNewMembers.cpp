#include "AddNewMembers.h"
#include "ui_AddNewMembers.h"

AddNewMembers::AddNewMembers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewMembers)
{
    ui->setupUi(this);
    ui->MembershipTypeComboBox->addItem("Regular");
    ui->MembershipTypeComboBox->addItem("Executive");
    ui->MemberIDSpinBox->setMinimum(10000);
    ui->MemberIDSpinBox->setMaximum(99999);
    connect(ui->LineEdit_MemberName, SIGNAL(textChanged(QString)), this, SLOT(checkInput(QString)));
    ui->PushButton_AddMember->setDisabled(true);
    ui->MemberExpirationDateSpinBox->setMinimumDate(QDate::currentDate());
    ui->PushButton_Yes->setDisabled(true);
    ui->PushButton_No->setDisabled(true);
}

AddNewMembers::~AddNewMembers()
{
    delete ui;
}

void AddNewMembers::lockInputs()
{
    ui->LineEdit_MemberName->setDisabled(true);
    ui->MemberIDSpinBox->setDisabled(true);
    ui->MembershipTypeComboBox->setDisabled(true);
    ui->MemberExpirationDateSpinBox->setDisabled(true);
}

void AddNewMembers::enableInputs()
{
    ui->LineEdit_MemberName->setDisabled(true);
    ui->MemberIDSpinBox->setDisabled(true);
    ui->MembershipTypeComboBox->setDisabled(true);
    ui->MemberExpirationDateSpinBox->setDisabled(true);
}

void AddNewMembers::checkInput(const QString &text)
{
    ui->PushButton_AddMember->setEnabled(!text.isEmpty());
}

void AddNewMembers::on_PushButton_AddMember_clicked()
{
    QString name;
    int id;
    QString mType;
    MemberType memberType;
    QDate expDate;

    name = ui->LineEdit_MemberName->text();
    id = ui->MemberIDSpinBox->value();
    mType = ui->MembershipTypeComboBox->currentText();
    expDate = ui->MemberExpirationDateSpinBox->date();

    if(mType == "Regular")
    {
        memberType = REGULAR;
    }
    else if(mType == "Executive")
    {
        memberType = EXECUTIVE;
    }
    if(!dbManager::instance().MemberExists(id))
    {
        if(dbManager::instance().AddMember(Member(name, id, memberType, expDate)))
        {
            emit memberAdded();
            ui->label_status->setText("Add Purchase?");
            lockInputs();
            ui->PushButton_AddMember->setEnabled(false);
            ui->PushButton_Yes->setEnabled(true);
            ui->PushButton_No->setEnabled(true);
        }
        else
        {
            ui->label_status->setText("Failed to add");
        }
    }
    else
    {
        ui->label_status->setText("Member ID exists");
    }
}


void AddNewMembers::on_PushButton_Yes_clicked()
{
    AddNewPurchase *transWin;
    int id;
    transWin = new AddNewPurchase;
    id = ui->MemberIDSpinBox->value();
    connect(this, SIGNAL(sendID(int)), transWin, SLOT(receiveID(int)));
    connect(transWin, SIGNAL(purchaseAdded()), this, SIGNAL(purchaseAdded()));
    emit sendID(id);
    transWin->setModal(true);
    transWin->exec();
    delete transWin;
}

void AddNewMembers::on_PushButton_No_clicked()
{
    enableInputs();
    ui->label_status->setText("Enter New Member");
    ui->PushButton_Yes->setDisabled(true);
    ui->PushButton_No->setDisabled(true);
}

void AddNewMembers::on_PushButton_BackToMain_clicked()
{
    this->close();
}
