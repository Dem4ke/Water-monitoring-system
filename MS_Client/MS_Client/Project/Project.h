#pragma once

#include "IProject.h"

#include <QPointF>
#include <QVector>

namespace MS {
// Class that provides vessel and handles all its measurements
class Project : public IProject {
public:
    Project() = default;
    ~Project() = default;

    void setCurrentLocation(const QPointF& location) override;
    const QPointF& getCurrentLocation() override;

    void addWindForcePoint(double value) override;
    double getAverageWindForce() const override;

    void addWaveHeightPoint(double value) override;
    double getAverageWaveHeight() const override;

private:
    double getAverage(const QVector<double>& values) const;

    QVector<QPointF> locations_;        // All locations of near vessels
    QVector<double> windForcePoints_;   // Measure of wind force
    QVector<double> waveHeightPoints_;  // Measure of wave height
};
}
