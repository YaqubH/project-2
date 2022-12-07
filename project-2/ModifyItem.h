#ifndef MODIFYITEM_H
#define MODIFYITEM_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include "dbManager.h"

namespace Ui {
class ModifyItem;
}

class ModifyItem : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyItem(QWidget *parent = 0, Item item = Item());
    ~ModifyItem();

private slots:

    void on_PushButton_ModifyItem_clicked();

    void on_PushButton_BackToMain_clicked();

signals:

    void ModifyConfirmed();

private:
    Ui::ModifyItem *ui;
    Item itemToModify;
};

#endif // MODIFYITEM_H
