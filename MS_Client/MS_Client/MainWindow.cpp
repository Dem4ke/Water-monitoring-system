#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Components/ProjectComponent.h"

#include <QDir>

namespace MS {
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow) {

    ui_->setupUi(this);

    // QML widgets start
    ui_->map->setSource(QUrl::fromLocalFile(QDir::currentPath() + "/MS_Client_QML/QML/Map.qml"));
    ui_->plot_waves->setSource(QUrl::fromLocalFile(QDir::currentPath() + "/MS_Client_QML/QML/Plot.qml"));
    ui_->plot_wind->setSource(QUrl::fromLocalFile(QDir::currentPath() + "/MS_Client_QML/QML/Plot.qml"));

    // Create project component and start to collect information
    projectComponent_ = new Component::ProjectComponent(this);
    connect(projectComponent_, &Component::ProjectComponent::mapUpdateRequest,
            this, &MainWindow::mapUpdate);
}

MainWindow::~MainWindow() {
    delete ui_;
}

// Update vessels locations on map
void MainWindow::mapUpdate(const QPointF& vesselLocation, const QVector<QPointF>& locations) {

}
}
