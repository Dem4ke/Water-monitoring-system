#include "UserLoginDlg.h"
#include "ui_UserLoginDlg.h"
#include "../../Components/SocketComponent.h"

namespace MS {
UserLoginDlg::UserLoginDlg(QWidget* parent)
    : QWidget(parent)
    , ui_(new Ui::UserLoginDlg) {
    ui_->setupUi(this);

    // Connect to server
    socketComponent_ = std::make_shared<SocketComponent>();
}

UserLoginDlg::~UserLoginDlg() {
    delete ui_;
}

void UserLoginDlg::on_sign_in_button_clicked() {

}

void UserLoginDlg::on_create_account_button_clicked() {

}
}
