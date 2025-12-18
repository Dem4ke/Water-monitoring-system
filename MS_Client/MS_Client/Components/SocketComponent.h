#pragma once

#include <QObject>
#include <QTcpSocket>

namespace Component {
// Variables of the server's actions
enum class ServerActionType : int {
    AddNewUser = 0,             // Insert user in database
    CheckUserStatement,         // Check user statement (are login and password correct)
    SetVesselInfo,              // Send geo and meteo data to the server
    GetNearLocations,           // Send request to get nearest vessels locations
};

class SocketComponent : public QObject {
    Q_OBJECT

public:
    static SocketComponent& Instance();

    void checkUserStatement(const QVector<QString>& info);
    void addUser(const QVector<QString>& info);
    void updateVesselData(const QPointF& location, double windForce, double waveHeight);
    void getNearVesselLocations(const QPointF& location, float radius);

signals:
    void getUserStatus(bool isLogAvalible);
    void updateNearVesselLocationsRequest(const QVector<QPointF>& locations);

private slots:
    // Server tools
    void slotReadyRead();
    void sendToServer(Component::ServerActionType dataType, const QVector<QString>& info);

private:
    SocketComponent(QObject* parent = nullptr);

    void serverRequest(ServerActionType dataType, const QVector<QString>& info);

    QTcpSocket* socket_ = nullptr;      // Socket which uses to work with server
    QByteArray data_;                   // Array of the data
    quint16 blockSize_ = 0;             // Size of data package
};

#define SOCKET SocketComponent::Instance()
}
