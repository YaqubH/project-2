#include "yesno.h"
#include "ui_yesno.h"
#include "dbmanager.h"

yesno::yesno(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::yesno)
{
    ui->setupUi(this);
}

yesno::~yesno()
{
    delete ui;
}

bool yesno::getData()
{
    return answer;
}

void yesno::on_pushButton_clicked()
{
    answer = true;
    hide();
}

void yesno::on_pushButton_2_clicked()
{
    answer = false;
    hide();
}
