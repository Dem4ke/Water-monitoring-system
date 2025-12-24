#pragma once

#include "IProject.h"

namespace MS {
// Class that provides vessel and handles all its measurements
class Project : public IProject {
public:
    Project(int id);
    ~Project() = default;

    int getId() const override;

    void setCurrentLocation(const QGeoCoordinate& location) override;
    const QGeoCoordinate& getCurrentLocation() override;

    void setNearVesselLocations(const QMap<int, QGeoCoordinate>& locations) override;
    const QMap<int, QGeoCoordinate>& getNearVesselLocations() override;

private:
    const int id_ = -1;                         // Unique data base id for the vessel
    QGeoCoordinate currentLocation_;            // Last received location of vessel
    QMap<int, QGeoCoordinate> locations_;       // All locations of near vessels and it's id
};
}
