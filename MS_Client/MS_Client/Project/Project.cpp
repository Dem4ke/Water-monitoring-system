#include "Project.h"

namespace MS {
// Set unique unchangable vessel's id
Project::Project(int id)
    : id_(id) {}

// Get data base id of this vessel
int Project::getId() const {
    return id_;
}

// Set new vessel's location
void Project::setCurrentLocation(const QGeoCoordinate& location) {
    currentLocation_ = location;
}

// Getter of current vessel's location
const QGeoCoordinate& Project::getCurrentLocation() {
    return currentLocation_;
}

void Project::setNearVesselLocations(const QMap<int, QGeoCoordinate>& locations) {
    locations_ = locations;
}

const QMap<int, QGeoCoordinate>& Project::getNearVesselLocations() {
    return locations_;
}
}
