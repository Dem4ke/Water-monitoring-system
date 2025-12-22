#pragma once

#include "VesselAPI.h"

namespace VesselAPI {
// NMEA-2000 vessel's monitoring system API
class NMEA2000VesselAPI : public IVesselAPI {
public:
    NMEA2000VesselAPI();
    ~NMEA2000VesselAPI() = default;

    const QGeoCoordinate& getGPSLocation() override;
    const QVector<double>& getWindForces() override;
    const QVector<double>& getWaveHeights() override;

    void clearCache() override;

private:
    QGeoCoordinate location_;           // Current GPS location of the vessel
    QVector<double> windForces_;        // Measures of wind force
    QVector<double> waveHeights_;       // Measures of wave heights
};
}
