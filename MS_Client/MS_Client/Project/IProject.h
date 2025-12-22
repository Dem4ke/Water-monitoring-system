#pragma once

#include <QMap>
#include <QGeoCoordinate>

namespace MS {
class IProject {
public:
    virtual ~IProject() = default;

    virtual int getId() const = 0;

    virtual void setCurrentLocation(const QGeoCoordinate& location) = 0;
    virtual const QGeoCoordinate& getCurrentLocation() = 0;

    virtual void setNearVesselLocations(const QMap<int, QGeoCoordinate>& locations) = 0;
    virtual const QMap<int, QGeoCoordinate>& getNearVesselLocations() = 0;
};
}
