#pragma once

#include <QVector>
#include <QGeoCoordinate>

namespace VesselAPI {
// Interface of an abstract vessel's monitoring system API
class IVesselAPI {
public:
    virtual ~IVesselAPI() = default;

    virtual const QGeoCoordinate& getGPSLocation() = 0;
    virtual const QVector<double>& getWindForces() = 0;
    virtual const QVector<double>& getWaveHeights() = 0;

    virtual void clearCache() = 0;
};
}
