#include "UserLoginDlg.h"
#include "ui_UserLoginDlg.h"

namespace MS {
UserLoginDlg::UserLoginDlg(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserLoginDlg)
{
    ui->setupUi(this);
}

UserLoginDlg::~UserLoginDlg()
{
    delete ui;
}

void UserLoginDlg::on_sign_in_button_clicked()
{

}

void UserLoginDlg::on_create_account_button_clicked()
{

}



}
