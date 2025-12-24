#pragma once

#include <QObject>
#include <QTcpSocket>

class QGeoCoordinate;

namespace Component {
// Variables of the server's actions
enum class EServerActionType : int {
    AddNewUser = 0,             // Insert user in database
    CheckUserStatement,         // Check user statement (are login and password correct)
    SetVesselInfo,              // Send geo and meteo data to the server
    GetNearLocations,           // Send request to get nearest vessels locations
    GetVesselData,              // Send request to get data of vessel by data base index
};

class SocketComponent : public QObject {
    Q_OBJECT

public:
    static SocketComponent& Instance();

    void checkUserStatement(const QVector<QString>& info);
    void addUser(const QVector<QString>& info);
    void updateVesselData(int id, const QGeoCoordinate& location, double windForce, double waveHeight);
    void getNearVesselLocations(const QGeoCoordinate& location, float radius);
    void getVesselData(int vesselIndex, int searchTimeSec);

signals:
    void updateUserStatusRequest(bool isLogAvalible, int vesselId);
    void updateNearVesselLocationsRequest(const QMap<int, QGeoCoordinate>& locations);
    void updateRoadMapRequest(const QVector<QGeoCoordinate>& roadLocations);
    void updateWindForcePlotRequest(const QVector<QDateTime>& timePoints, const QVector<double>& windForces);
    void updateWaveHeightPlotRequest(const QVector<QDateTime>& timePoints, const QVector<double>& waveHeights);

private slots:
    // Server tools
    void slotReadyRead();
    void sendToServer(Component::EServerActionType dataType, const QVector<QString>& info);

private:
    SocketComponent(QObject* parent = nullptr);

    void serverRequest(EServerActionType dataType, const QVector<QString>& info);

    QTcpSocket* socket_ = nullptr;      // Socket which uses to work with server
    QByteArray data_;                   // Array of the data
    quint16 blockSize_ = 0;             // Size of data package
};

#define SOCKET SocketComponent::Instance()
}
