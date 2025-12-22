#include "SocketComponent.h"

#include <QGeoCoordinate>
#include <QMessageBox>
#include <QDateTime>

namespace Component {
SocketComponent::SocketComponent(QObject* parent)
    : QObject{parent} {

    // Connect socket to server
    socket_ = new QTcpSocket(this);

    connect(socket_, &QTcpSocket::readyRead, this, &SocketComponent::slotReadyRead);
    connect(socket_, &QTcpSocket::disconnected, socket_, &QTcpSocket::deleteLater);
    socket_->connectToHost("127.0.0.1", 2323);
}

// Get Instance of the socket
SocketComponent &SocketComponent::Instance() {
    static SocketComponent socketComponent(nullptr);
    return socketComponent;
}

// Send to server info about user and request an answer
void SocketComponent::checkUserStatement(const QVector<QString>& info) {
    sendToServer(ServerActionType::CheckUserStatement, info);
}

// Send to server info about user and request to add it into database
void SocketComponent::addUser(const QVector<QString>& info) {
    sendToServer(ServerActionType::AddNewUser, info);
}

// Send monitoring's data to server
void SocketComponent::updateVesselData(int id, const QGeoCoordinate& location,
                                       double windForce, double waveHeight) {
    QVector<QString> info;
    info.push_back(QString::number(id));
    info.push_back(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss+00"));
    info.push_back(QString::number(location.latitude()));
    info.push_back(QString::number(location.longitude()));
    info.push_back(QString::number(windForce));
    info.push_back(QString::number(waveHeight));

    sendToServer(ServerActionType::SetVesselInfo, info);
}

// Send request to find nearest vessels in inputted radius to the server
void SocketComponent::getNearVesselLocations(const QGeoCoordinate& location, float radius) {
    QVector<QString> info;
    info.push_back(QString::number(location.latitude()));
    info.push_back(QString::number(location.longitude()));
    info.push_back(QString::number(radius));

    sendToServer(ServerActionType::GetNearLocations, info);
}

void SocketComponent::getVesselData(int vesselIndex, int searchTimeSec) {
    QVector<QString> info;
    info.push_back(QString::number(vesselIndex));
    info.push_back(QString::number(searchTimeSec));

    sendToServer(ServerActionType::GetVesselData, info);
}

// Handler of a server's messages
void SocketComponent::slotReadyRead() {
    QDataStream input(socket_);
    input.setVersion(QDataStream::Qt_6_10);

    if (input.status() == QDataStream::Ok) {
        while(true) {
            // Calculate size of the block of data
            if (blockSize_ == 0) {
                if (socket_->bytesAvailable() < 2) {
                    break;
                }

                // Write the block size
                input >> blockSize_;
            }

            // If the block size is bigger than data from server, we should wait to other data
            if (socket_->bytesAvailable() < blockSize_) {
                break;
            }

            // Write data from server
            QVector<QString> info;
            ServerActionType actionType;

            input >> actionType>> info;

            serverRequest(actionType, info);

            blockSize_ = 0;
            break;
        }
    }
}

// Server message handler
void SocketComponent::serverRequest(ServerActionType actionType, const QVector<QString>& info) {
    if (actionType == ServerActionType::AddNewUser ||
        actionType == ServerActionType::CheckUserStatement) {

        if (info.isEmpty()) {
            return;
        }

        bool isLogAvalible = false;
        int vesselId = -1;

        // If user is validated, he gets his database unique id
        if (info[0] == "1") {
            isLogAvalible = true;
            vesselId = info[1].toInt();
        }

        emit updateUserStatusRequest(isLogAvalible, vesselId);
    }
    else if (actionType == ServerActionType::GetNearLocations) {
        if (info.isEmpty() || info.size() % 3 != 0) {
            return;
        }

        // Map with key - id of the vessel, value - it's coordinates
        QMap<int, QGeoCoordinate> locations;

        for (int i = 0, end = info.size(); i < end; i += 3) {
            QGeoCoordinate geo(info[i + 1].toDouble(), info[i + 2].toDouble());
            locations.insert(info[i].toInt(), geo);
        }

        emit updateNearVesselLocationsRequest(locations);
    }
    else if (actionType == ServerActionType::GetVesselData) {
        if (info.isEmpty()) {
            return;
        }

        // Map with key - id of the vessel, value - it's coordinates
        QVector<double> tempLocations;
        QVector<QGeoCoordinate> roadLocations;
        QVector<QDateTime> timePoints;
        QVector<double> windForces;
        QVector<double> waveHeights;

        int switcher = -1;

        for (auto& i : info) {
            if (i == "road") {
                switcher = 0;
                continue;
            }
            else if (i == "date") {
                switcher = 1;
                continue;
            }
            else if (i == "wind") {
                switcher = 2;
                continue;
            }
            else if (i == "wave") {
                switcher = 3;
                continue;
            }

            switch(switcher) {
            case 0: {
                tempLocations.push_back(i.toDouble());
                break;
            }
            case 1: {
                timePoints.emplaceBack(QDateTime(QDateTime::fromString(i, "yyyy-MM-dd hh:mm:ss+00")));
                break;
            }
            case 2: {
                windForces.push_back(i.toDouble());
                break;
            }
            case 3: {
                waveHeights.push_back(i.toDouble());
                break;
            }
            }
        }

        if (tempLocations.size() % 2 != 0) {
            return;
        }

        for (int i = 0, end = tempLocations.size(); i < end; i += 2) {
            roadLocations.emplaceBack(QGeoCoordinate(tempLocations[i], tempLocations[i + 1]));
        }

        emit updateRoadMapRequest(roadLocations);
        emit updateWindForcePlotRequest(timePoints, windForces);
        emit updateWaveHeightPlotRequest(timePoints, waveHeights);
    }
}

// Send information to server
void SocketComponent::sendToServer(ServerActionType actionType, const QVector<QString>& info) {
    data_.clear();

    QDataStream out(&data_, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_10);

    // Calculate and write a size of the sent data package
    out << quint16(0) << actionType << info;
    out.device()->seek(0);
    out << quint16(data_.size() - sizeof(quint16));

    // Send package
    socket_->write(data_);
}
}
