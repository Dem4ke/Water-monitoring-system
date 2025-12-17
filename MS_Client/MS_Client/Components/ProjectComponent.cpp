#include "ProjectComponent.h"
#include "ConfigComponent.h"
#include "SocketComponent.h"
#include "../Project/Project.h"
#include "../VesselAPI/VesselAPI.h"
#include "../VesselAPI/VesselAPIFactory.h".h"

#include <QTimer>
#include <QDebug>

namespace Component {
ProjectComponent::ProjectComponent(QObject* parent)
    : QObject{parent} {

    // Create concrete vessel's monitoring API
    // ! In this case that slill is an abstract API !
    std::unique_ptr<VesselAPI::IVesselAPIFactory> vesselAPIFactory = nullptr;

    switch(CONFIG.getVesselMonSysIndex()) {
    case 0: {
        vesselAPIFactory = std::make_unique<VesselAPI::NMEA2000VesselAPIFacrory>();
        break;
    }
    }

    if (vesselAPIFactory) {
        vesselAPI_ = vesselAPIFactory->createVesselAPI();
    }
    else {
        qCritical() << "Failed to create the vessel API";
        return;
    }

    // Create project and update data
    project_ = std::make_shared<MS::Project>();
    project_->setCurrentLocation(vesselAPI_->getGPSLocation());

    // Create timer to update information from vessel's monitoring system API and send information to server
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &ProjectComponent::dataUpdate);
}

// Collect geo and meteo data from vessel's monitoring system API and create request to update data on the server side
void ProjectComponent::dataUpdate() {
    // Calculate average value
    auto getAverage = [](const QVector<double>& values) {
        double average = 0.0;

        for (auto& i : values) {
            average += i;
        }

        average /= values.size();
        return average;
    };

    QPointF location = vesselAPI_->getGPSLocation();
    double windForce = getAverage(vesselAPI_->getWindForces());
    double waveHeight = getAverage(vesselAPI_->getWaveHeights());


}
}
