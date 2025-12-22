#include "NMEA2000VesselAPI.h"

namespace VesselAPI {

NMEA2000VesselAPI::NMEA2000VesselAPI() {
    location_.setLatitude(0.0);
    location_.setLongitude(0.0);
}

// Getter for gps location
const QGeoCoordinate& NMEA2000VesselAPI::getGPSLocation() {
    return location_;
}

// Geeter for measured wind forces
const QVector<double>& NMEA2000VesselAPI::getWindForces() {
    return windForces_;
}

// Getter for measured wave heights
const QVector<double>& NMEA2000VesselAPI::getWaveHeights() {
    return waveHeights_;
}

// Clear all cache after that information had sent
void NMEA2000VesselAPI::clearCache() {
    windForces_.clear();
    waveHeights_.clear();
}
}
