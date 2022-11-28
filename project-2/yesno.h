#ifndef YESNO_H
#define YESNO_H

#include <QDialog>
#include "dbmanager.h"

namespace Ui {
class yesno;
}

class yesno : public QDialog
{
    Q_OBJECT

public:
    explicit yesno(QWidget *parent = nullptr);
    ~yesno();

    yesno(QWidget *parent, bool &returnAnswer);
    bool getData();
private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::yesno *ui;
    bool answer = false;
};

#endif // YESNO_H
