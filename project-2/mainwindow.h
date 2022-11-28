#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dbmanager.h"
#include "storemanagerpage.h"
#include "adminpage.h"
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);  // Default constructor
    ~MainWindow();  // Destructor

private slots:
    /*******************************************************
     * on_loginPushBtn_clicked() -
     *  This function error checks for right username and
     *  password. If admin logs in, then the program will
     *  display the admin page. If store manager logs in
     *  the program will display the store manager page
     *******************************************************/
    void on_loginPushBtn_clicked();

    /*******************************************************
     * on_clearPushBtn_clicked() -
     *  Once a user presses the clear button, it will clear
     *  the username and password line edits
     *******************************************************/
    void on_clearPushBtn_clicked();

private:
    Ui::MainWindow *ui;
//    DBManager *db_controller;
    AdminPage adminObj;         // AdminPage class object
    StoreManagerPage storeObj;  // StoreManagerPage class object
};
#endif // MAINWINDOW_H
