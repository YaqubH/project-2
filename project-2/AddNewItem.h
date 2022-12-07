#ifndef ADDNEWITEM_H
#define ADDNEWITEM_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include "dbManager.h"

namespace Ui {
class AddNewItem;
}
class AddNewItem : public QDialog
{
    Q_OBJECT

public:

    explicit AddNewItem(QWidget *parent = 0);
    ~AddNewItem();

private slots:
    void on_PushButton_AddItem_clicked();

    void on_PushButton_BackToMain_clicked();

signals:
    void itemAdded();

private:
    Ui::AddNewItem *ui;
};

#endif // ADDNEWITEM_H
