#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Components/ConfigComponent.h"
#include "Components/SocketComponent.h"
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

    // Connect to socket component
    connect(&Component::SOCKET.Instance(), &Component::SocketComponent::updateLocationsRequest, this, &MainWindow::updateLocations);

    // Find first
    getNearVesselLocation(Component::CONFIG.getSearchRadius());
}

MainWindow::~MainWindow() {
    delete ui_;
}

// Update near vessels locations on the map
void MainWindow::updateLocations(const QVector<QString>& locations) {

}

// Get near vessels' locations from server and set them on map widgetvoid
MainWindow::getNearVesselLocation(const QPointF& location, float radius) {
    Component::SOCKET.getNearVesselLocationRequest(radius);
}
}
