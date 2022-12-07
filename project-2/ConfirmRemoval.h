#ifndef CONFIRMREMOVAL_H
#define CONFIRMREMOVAL_H

#include <QDialog>
#include <QAbstractTableModel>
#include "Models.h"

namespace Ui {
class ConfirmRemoval;
}

class ConfirmRemoval : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmRemoval(QWidget *parent = 0);
    ~ConfirmRemoval();

private slots:
    void setMemberView(MemberModel *model);
    void setItemView(ItemModel *model);

    void on_PushButton_ConfirmRemoval_clicked();

    void on_PushButton_BackToAdmin_clicked();

signals:
    void removalConfirmed();

private:
    Ui::ConfirmRemoval *ui;
};

#endif // CONFIRMREMOVAL_H
