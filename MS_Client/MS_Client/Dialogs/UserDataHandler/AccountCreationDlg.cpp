#include "AccountCreationDlg.h"
#include "ui_AccountCreationDlg.h"

namespace MS {
AccountCreationDlg::AccountCreationDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AccountCreationDlg)
{
    ui->setupUi(this);
}

AccountCreationDlg::~AccountCreationDlg()
{
    delete ui;
}

void AccountCreationDlg::on_create_account_button_clicked()
{

}
}
