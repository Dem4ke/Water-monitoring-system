#pragma once

#include <QVector>
#include <QPointF>

namespace VesselAPI {
// Interface of an abstract vessel's monitoring system API
class IVesselAPI {
public:
    virtual ~IVesselAPI() = default;

    virtual const QPointF& getGPSLocation() = 0;
    virtual const QVector<double>& getWindForces() = 0;
    virtual const QVector<double>& getWaveHeights() = 0;
};
}
