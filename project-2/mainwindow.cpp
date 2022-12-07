#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Admin.h"
#include "Manager.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->LineEdit_Username, SIGNAL(returnPressed()), ui->PushButton_login, SLOT(click()));
    connect(ui->LineEdit_Password, SIGNAL(returnPressed()), ui->PushButton_login, SLOT(click()));
    if(!dbManager::instance().isOpen())
    {
        qDebug() << "Successfully Connected to the Database";
    }
    else
    {
        qDebug() << "Successfully Connected to the Database";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_PushButton_login_clicked()
{
    QString employeeType;
    Manager manager;
    Admin admin;

    if(dbManager::instance().VerifyLogin(Credentials(ui->LineEdit_Username->text(),
                                                     ui->LineEdit_Password->text()), employeeType))
    {
        if(employeeType == "administrator")
        {
            QMessageBox::information(NULL,"Hello","Welcome to the Administration Page","OK");
            admin.setModal(true);
            admin.exec();
            hide();
        }
        else if(employeeType == "store_manager")
        {
            QMessageBox::information(NULL,"Hello","Welcome to the Store Manager Page","OK");
            manager.setModal(true);
            manager.exec();
            hide();
        }
    }else
    {
         QMessageBox::information(NULL,"Try Again","Invalid username or password","OK");
         ui->LineEdit_Username->setText("");
         ui->LineEdit_Password->setText("");
    }
}
