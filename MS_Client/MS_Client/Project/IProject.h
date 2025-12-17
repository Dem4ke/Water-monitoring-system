#pragma once

class QPointF;

namespace MS {
class IProject {
public:
    virtual ~IProject() = default;

    virtual void setCurrentLocation(const QPointF& location) = 0;
    virtual const QPointF& getCurrentLocation() = 0;

    virtual void addWindForcePoint(double value) = 0;
    virtual double getAverageWindForce() const = 0;

    virtual void addWaveHeightPoint(double value) = 0;
    virtual double getAverageWaveHeight() const = 0;
};
}
