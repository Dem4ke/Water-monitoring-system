#pragma once

#include <QVector>
#include <QPointF>

namespace MS {
class IProject {
public:
    virtual ~IProject() = default;

    virtual void setCurrentLocation(const QPointF& location) = 0;
    virtual const QPointF& getCurrentLocation() = 0;

    virtual void setNearVesselLocations(const QVector<QPointF>& locations) = 0;
    virtual const QVector<QPointF>& getNearVesselLocations() = 0;
};
}
