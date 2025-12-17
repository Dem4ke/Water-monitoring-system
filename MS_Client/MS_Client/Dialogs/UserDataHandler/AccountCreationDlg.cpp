#include "AccountCreationDlg.h"
#include "ui_AccountCreationDlg.h"
#include "../../Components/SocketComponent.h"

#include <QMessageBox>

namespace Dialog {
AccountCreationDlg::AccountCreationDlg(QWidget* parent)
    : QDialog(parent)
    , ui_(new Ui::AccountCreationDlg) {

    ui_->setupUi(this);
}

AccountCreationDlg::~AccountCreationDlg() {
    delete ui_;
}

void AccountCreationDlg::checkUserStatus(bool isLogAvalible) {
    disconnect(&Component::SOCKET.Instance(), &Component::SocketComponent::getUserStatus, this, &AccountCreationDlg::checkUserStatus);

    if (isLogAvalible) {
        emit accountCreated(true);

        this->close();
    }
    else {
        QMessageBox::warning(this, QObject::tr("Denied"), QObject::tr("User with the same login exists"));
    }
}

void AccountCreationDlg::on_create_account_button_clicked() {
    QVector<QString> userInfo;
    userInfo.push_back(ui_->user_login->text());
    userInfo.push_back(ui_->user_password->text());
    userInfo.push_back(ui_->vessel_name->text());

    connect(&Component::SOCKET.Instance(), &Component::SocketComponent::getUserStatus, this, &AccountCreationDlg::checkUserStatus);
    Component::SOCKET.addUser(userInfo);
}
}
