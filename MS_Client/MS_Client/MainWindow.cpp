#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDir>

namespace MS {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow) {
    ui_->setupUi(this);

    ui_->map->setSource(QUrl::fromLocalFile(QDir::currentPath() + "/QML/Map.qml"));
}

MainWindow::~MainWindow() {
    delete ui_;
}
}
