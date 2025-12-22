#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Components/ProjectComponent.h"
#include "Components/SocketComponent.h"
#include "Components/ViewComponents/MapViewComponent.h"
#include "Components/ViewComponents/PlotViewComponent.h"

#include <QTimer>

namespace MS {
MainWindow::MainWindow(QWidget* parent, int vesselId)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow) {

    ui_->setupUi(this);

    projectComponent_ = new Component::ProjectComponent(this, vesselId);
    mapViewComponent_ = new Component::MapViewComponent(this, ui_->map);
    windPlotViewComponent_ = new Component::PlotViewComponent(this, ui_->plot_wind, "Wind force");
    wavePlotViewComponent_ = new Component::PlotViewComponent(this, ui_->plot_waves, "Wave height");

    // Connect components with each other
    connect(projectComponent_, &Component::ProjectComponent::currentLocationUpdateRequest,
            mapViewComponent_, &Component::MapViewComponent::currentlocationUpdate);
    connect(projectComponent_, &Component::ProjectComponent::locationsUpdateRequest,
            mapViewComponent_, &Component::MapViewComponent::locationsUpdate);
    connect(mapViewComponent_, &Component::MapViewComponent::vesselDataRequest,
            projectComponent_, &Component::ProjectComponent::vesselDataRequested);

    connect(&Component::SOCKET.Instance(), &Component::SocketComponent::updateRoadMapRequest,
            mapViewComponent_, &Component::MapViewComponent::updateRoadMap);
    connect(&Component::SOCKET.Instance(), &Component::SocketComponent::updateWindForcePlotRequest,
            windPlotViewComponent_, &Component::PlotViewComponent::setPoints);
    connect(&Component::SOCKET.Instance(), &Component::SocketComponent::updateWaveHeightPlotRequest,
            wavePlotViewComponent_, &Component::PlotViewComponent::setPoints);

    // First launch update
    QTimer::singleShot(0, this, [this]() {
        projectComponent_->dataUpdate();
        projectComponent_->locationsUpdate();
    });
}

MainWindow::~MainWindow() {
    delete ui_;
}
}
