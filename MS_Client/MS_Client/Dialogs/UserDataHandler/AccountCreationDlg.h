#pragma once

#include <QDialog>

namespace Ui {
class AccountCreationDlg;
}

namespace Dialog {
class AccountCreationDlg : public QDialog {
    Q_OBJECT

public:
    explicit AccountCreationDlg(QWidget* parent = nullptr);
    ~AccountCreationDlg();

signals:
    void accountCreated(bool isAccountCreated);

public slots:
    void checkUserStatus(bool isLogAvalible);

private slots:
    void on_create_account_button_clicked();

private:
    Ui::AccountCreationDlg* ui_;
};
}
