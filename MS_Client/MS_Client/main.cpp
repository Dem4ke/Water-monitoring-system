#include "Dialogs/UserDataHandler/UserLoginDlg.h"
#include "Components/LanguageComponent.h"
#include "Components/ConfigComponent.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Dialog::UserLoginDlg w;
    w.show();
    return a.exec();
}
