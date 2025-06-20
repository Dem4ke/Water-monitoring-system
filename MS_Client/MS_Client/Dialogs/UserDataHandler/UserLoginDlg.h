#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class UserLoginDlg;
}
QT_END_NAMESPACE

namespace MS {
class SocketComponent;

class UserLoginDlg : public QWidget {
    Q_OBJECT

public:
    UserLoginDlg(QWidget* parent = nullptr);
    ~UserLoginDlg();

private slots:
    void on_sign_in_button_clicked();
    void on_create_account_button_clicked();

private:
    Ui::UserLoginDlg* ui_;                                          // User interface

    std::shared_ptr<SocketComponent> socketComponent_ = nullptr;    // Pointer to the component that contains TCP socket
};
}
