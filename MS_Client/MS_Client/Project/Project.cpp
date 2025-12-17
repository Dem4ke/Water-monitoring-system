#include "Project.h"

namespace MS {
// Set new vessel's location
void Project::setCurrentLocation(const QPointF& location) {
    location_ = location;
}

// Getter of current vessel's location
const QPointF& Project::getCurrentLocation() {
    return location_;
}

// Add new wind force measure point
void Project::addWindForcePoint(double value) {
    windForcePoints_.push_back(value);
}

// Calculate average wind force and clear measurements
double Project::getAverageWindForce() const {
    return getAverage(windForcePoints_);
}

// Add new wave height measure point
void Project::addWaveHeightPoint(double value) {
    waveHeightPoints_.push_back(value);
}

// Calculate average wave height and clear measurements
double Project::getAverageWaveHeight() const {
    return getAverage(windForcePoints_);
}

// Calculate average value of an array of doubles
double Project::getAverage(const QVector<double>& values) const {
    double average = 0.0;

    for (auto& i : values) {
        average += i;
    }

    average /= values.size();
    return average;
}
}
