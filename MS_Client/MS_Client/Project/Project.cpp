#include "Project.h"

namespace MS {
// Set new vessel's location
void Project::setCurrentLocation(const QPointF& location) {
    currentLocation_ = location;
}

// Getter of current vessel's location
const QPointF& Project::getCurrentLocation() {
    return currentLocation_;
}

void Project::setNearVesselLocations(const QVector<QPointF>& locations) {
    locations_ = locations;
}

const QVector<QPointF> &Project::getNearVesselLocations() {
    return locations_;
}
}
