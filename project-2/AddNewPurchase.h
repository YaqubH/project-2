#ifndef ADDNEWPURCHASE_H
#define ADDNEWPURCHASE_H

#include <QDialog>
#include <QMessageBox>
#include "dbManager.h"

namespace Ui {
class AddNewPurchase;
}

class AddNewPurchase : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewPurchase(QWidget *parent = 0);

    ~AddNewPurchase();

signals:
    void purchaseAdded();

private slots:
    void receiveID(int id);

    void on_PushButton_AddPurchase_clicked();

    void on_PushButton_CancelPurchase_clicked();

private:
    Ui::AddNewPurchase *ui;
    int memberID;
};

#endif // ADDNEWPURCHASE_H
