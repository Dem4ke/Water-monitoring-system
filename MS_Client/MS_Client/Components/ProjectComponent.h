#pragma once

#include <QObject>
#include <QVector>
#include <QGeoCoordinate>
#include <QDateTime>

class QTimer;

namespace MS {
class IProject;
}

namespace VesselAPI {
class IVesselAPI;
}

namespace Component {
// Component that contais project and all methods to handle it
class ProjectComponent : public QObject {
    Q_OBJECT

public:
    ProjectComponent(QObject* parent, int vesselId);
    ~ProjectComponent() = default;

signals:
    void locationsUpdateRequest(const QMap<int, QGeoCoordinate>& locations);
    void currentLocationUpdateRequest(const QGeoCoordinate& currentlLocation);

public slots:
    void dataUpdate();
    void locationsUpdate();
    void updateNearVesselLocations(const QMap<int, QGeoCoordinate>& locations);
    void vesselDataRequested(int index);

private:
    std::shared_ptr<MS::IProject> project_ = nullptr;               // Project (vessel) that contains information
    std::shared_ptr<VesselAPI::IVesselAPI> vesselAPI_ = nullptr;    // Interface of vessel's monitoring system

    QTimer* timer_;                                                 // Timer that helps collect information
};
}
