#include "UserLoginDlg.h"
#include "ui_UserLoginDlg.h"
#include "AccountCreationDlg.h"
#include "../../MainWindow.h"
#include "../../Components/SocketComponent.h"

#include <QMessageBox>

namespace Dialog {
UserLoginDlg::UserLoginDlg(QWidget* parent)
    : QWidget(parent)
    , ui_(new Ui::UserLoginDlg) {
    ui_->setupUi(this);
}

UserLoginDlg::~UserLoginDlg() {
    delete ui_;
}

void UserLoginDlg::checkUserStatus(bool isLogAvalible) {
    if (isLogAvalible) {
        this->hide();

        MS::MainWindow* mainWindow = new MS::MainWindow;
        mainWindow->show();
    }
    else {
        QMessageBox::warning(this, QObject::tr("Such user doesn't exist"), QObject::tr("Incorrect login or password"));
    }
}

void UserLoginDlg::on_sign_in_button_clicked() {
    QVector<QString> userInfo;
    userInfo.push_back(ui_->user_login->text());
    userInfo.push_back(ui_->user_password->text());

    connect(&Component::SOCKET.Instance(), &Component::SocketComponent::getUserStatus, this, &UserLoginDlg::checkUserStatus);
    Component::SOCKET.checkUserStatement(userInfo);
    disconnect(&Component::SOCKET.Instance(), &Component::SocketComponent::getUserStatus, this, &UserLoginDlg::checkUserStatus);
}

void UserLoginDlg::on_create_account_button_clicked() {
    this->hide();

    AccountCreationDlg* accountCreationDlg = new AccountCreationDlg(this);
    connect(accountCreationDlg, &AccountCreationDlg::accountCreated, this, &UserLoginDlg::checkUserStatus);
    connect(accountCreationDlg, &AccountCreationDlg::finished, accountCreationDlg, &AccountCreationDlg::deleteLater);

    accountCreationDlg->exec();
}
}
