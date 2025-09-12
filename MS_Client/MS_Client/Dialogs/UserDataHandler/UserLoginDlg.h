#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class UserLoginDlg;
}
QT_END_NAMESPACE

namespace Dialog {
class UserLoginDlg : public QWidget {
    Q_OBJECT

public:
    UserLoginDlg(QWidget* parent = nullptr);
    ~UserLoginDlg();

public slots:
    void checkUserStatus(bool isLogAvalible);

private slots:
    void on_sign_in_button_clicked();
    void on_create_account_button_clicked();

private:
    Ui::UserLoginDlg* ui_;  // User interface
};
}
