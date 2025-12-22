#include "MapViewComponent.h"

#include <QQuickWidget>
#include <QQmlContext>
#include <QDir>

namespace Component {
MapViewComponent::MapViewComponent(QObject* parent, QQuickWidget* map)
    : QObject(parent)
    , map_(map) {

    // QML widget start
    map_->rootContext()->setContextProperty("mapData", this);
    map_->setSource(QUrl::fromLocalFile(QDir::currentPath() + "/MS_Client_QML/QML/Map.qml"));
}

// Getter of locations for map QML
const QVector<QGeoCoordinate>& MapViewComponent::getLocations() {
    return locations_;
}

// Getter of road map locations for map QML
const QVector<QGeoCoordinate>& MapViewComponent::getRoadMap() {
    return roadMap_;
}

// Getter of current vessel location
const QGeoCoordinate& MapViewComponent::getCurrentLocation() {
    return currentLocation_;
}

// Update current vessel's location
void MapViewComponent::currentlocationUpdate(const QGeoCoordinate& currentLocation) {
    currentLocation_ = currentLocation;

    emit updatecurrentLocationRequest();
}

// Set new locations and update map QML
void MapViewComponent::locationsUpdate(const QMap<int, QGeoCoordinate>& locations) {
    indicies_.clear();
    locations_.clear();

    for (auto it = locations.cbegin(); it != locations.cend(); ++it) {
        indicies_.push_back(it.key());
        locations_.push_back(it.value());
    }

    emit updateLocationsRequest();
}

// Set road map of chosen vessel to show it's path
void MapViewComponent::updateRoadMap(const QVector<QGeoCoordinate>& roadMap) {
    roadMap_ = roadMap;
    emit updateRoadMapRequest();
}

// User requested road of clicked vessel
void MapViewComponent::vesselDataRequested(int vesselIndex) {
    // If vessel index is bigger than vessels' array size, that index is invalid
    if (vesselIndex >= indicies_.size()) {
        qCritical() << "vessel index is invalid";
        return;
    }

    emit vesselDataRequest(indicies_[vesselIndex]);
}
}
