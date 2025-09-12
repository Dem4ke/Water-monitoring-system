#include "MainWindow.h"
#include "ui_MainWindow.h"

namespace MS {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow) {
    ui_->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui_;
}
}
