#ifndef ADDNEWMEMBERS_H
#define ADDNEWMEMBERS_H

#include <QDialog>
#include "dbManager.h"
#include "AddNewPurchase.h"

namespace Ui {
class AddNewMembers;
}

class AddNewMembers : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewMembers(QWidget *parent = nullptr);
    ~AddNewMembers();

    void lockInputs();

    void enableInputs();

private slots:

    void on_PushButton_AddMember_clicked();

    void checkInput(const QString& text);

    void on_PushButton_Yes_clicked();

    void on_PushButton_No_clicked();

    void on_PushButton_BackToMain_clicked();

signals:
    void sendID(int id);

    void memberAdded();

    void purchaseAdded();

private:
    Ui::AddNewMembers *ui;
};

#endif // ADDNEWMEMBERS_H
