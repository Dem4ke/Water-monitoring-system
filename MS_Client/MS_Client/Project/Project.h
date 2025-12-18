#pragma once

#include "IProject.h"

namespace MS {
// Class that provides vessel and handles all its measurements
class Project : public IProject {
public:
    Project() = default;
    ~Project() = default;

    void setCurrentLocation(const QPointF& location) override;
    const QPointF& getCurrentLocation() override;

    void setNearVesselLocations(const QVector<QPointF>& locations) override;
    const QVector<QPointF>& getNearVesselLocations() override;

private:
    QPointF currentLocation_;           // Last received location of vessel
    QVector<QPointF> locations_;        // All locations of near vessels
};
}
