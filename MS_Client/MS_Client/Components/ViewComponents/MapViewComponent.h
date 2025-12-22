#pragma once

#include <QObject>
#include <QMap>
#include <QVector>
#include <QGeoCoordinate>

class QQuickWidget;

namespace Component {
// Component that handle map QML widget data
class MapViewComponent : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QGeoCoordinate> locations READ getLocations NOTIFY updateLocationsRequest)
    Q_PROPERTY(QVector<QGeoCoordinate> roadMap READ getRoadMap NOTIFY updateRoadMapRequest)
    Q_PROPERTY(QGeoCoordinate currentLocation READ getCurrentLocation NOTIFY updatecurrentLocationRequest)

public:
    MapViewComponent(QObject* parent, QQuickWidget* map);

    const QVector<QGeoCoordinate>& getLocations();
    const QVector<QGeoCoordinate>& getRoadMap();
    const QGeoCoordinate& getCurrentLocation();

signals:
    void updateLocationsRequest();
    void updateRoadMapRequest();
    void updatecurrentLocationRequest();
    void vesselDataRequest(int vesselIndex);

public slots:
    void currentlocationUpdate(const QGeoCoordinate& currentLocation);
    void locationsUpdate(const QMap<int, QGeoCoordinate>& locations);
    void updateRoadMap(const QVector<QGeoCoordinate>& roadMap);
    void vesselDataRequested(int vesselIndex);

private:
    QQuickWidget* map_;                         // Map QML
    QGeoCoordinate currentLocation_;            // Vessel's location
    QVector<int> indicies_;                     // Indicies of the nearest vessels
    QVector<QGeoCoordinate> locations_;         // Locations of the nearest vessels
    QVector<QGeoCoordinate> roadMap_;           // Locations of vessel's road map
};
}
