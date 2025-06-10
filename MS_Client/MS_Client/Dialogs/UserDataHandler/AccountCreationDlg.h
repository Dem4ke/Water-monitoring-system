#pragma once

#include <QDialog>

namespace Ui {
class AccountCreationDlg;
}

namespace MS {
class AccountCreationDlg : public QDialog
{
    Q_OBJECT

public:
    explicit AccountCreationDlg(QWidget *parent = nullptr);
    ~AccountCreationDlg();

private slots:
    void on_create_account_button_clicked();

private:
    Ui::AccountCreationDlg *ui;
};
}
