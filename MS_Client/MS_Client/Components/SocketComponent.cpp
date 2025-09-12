#include "SocketComponent.h"

#include <QMessageBox>

namespace Component {
SocketComponent::SocketComponent(QObject* parent)
    : QObject{parent} {

    // Connect socket to server
    socket_ = new QTcpSocket(this);

    connect(socket_, &QTcpSocket::disconnected, socket_, &QTcpSocket::deleteLater);
    socket_->connectToHost("127.0.0.1", 2323);
}

// Get Instance of the socket
SocketComponent &SocketComponent::Instance() {
    static SocketComponent socketComponent(nullptr);
    return socketComponent;
}

// Send to server info about user and request an answer
void SocketComponent::checkUserStatement(QVector<QString> info) {
    sendToServer(ServerActionType::CheckUserStatement, info);
}

// Handler of a server's messages
void SocketComponent::slotReadyRead() {
    QDataStream input(socket_);
    input.setVersion(QDataStream::Qt_6_8);

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
            int actionType;

            input >> actionType>> info;

            if (static_cast<ServerActionType>(actionType) == ServerActionType::AddNewUser ||
                static_cast<ServerActionType>(actionType) == ServerActionType::CheckUserStatement) {

                if (info.isEmpty()) {
                    break;
                }

                bool isLogAvalible = false;

                if (info[0] == "1") {
                    isLogAvalible = true;
                }

                emit getUserStatus(isLogAvalible);
            }

            blockSize_ = 0;
            break;
        }
    }
}

void SocketComponent::sendToServer(ServerActionType actionType, QVector<QString> output) {
    data_.clear();

    QDataStream out(&data_, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_8);

    // Calculate and write a size of the sent data package
    out << quint16(0) << static_cast<int>(actionType) << output;
    out.device()->seek(0);
    out << quint16(data_.size() - sizeof(quint16));

    // Send package
    socket_->write(data_);
}
}
