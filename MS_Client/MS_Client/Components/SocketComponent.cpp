#include "SocketComponent.h"

#include <QMessageBox>

namespace MS {
SocketComponent::SocketComponent(QObject* parent)
    : QObject{parent} {

    // Connect socket to server
    socket_ = new QTcpSocket(this);

    connect(socket_, &QTcpSocket::disconnected, socket_, &QTcpSocket::deleteLater);
    socket_->connectToHost("127.0.0.1", 2323);
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
            int flag;

            input >> flag >> info;

            if (flag == 2) {
                if (info[0] == "denied") {
                    QMessageBox::warning(nullptr, "Denied","Login or password is wrong");
                }
                else if (info[0] == "success") {
                }
            }

            blockSize_ = 0;
            break;
        }
    }
}

void SocketComponent::sendToServer(DataType dataType, QVector<QString> output) {
    data_.clear();

    QDataStream out(&data_, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_7);

    // Calculate and write a size of the sent data package
    out << quint16(0) << dataType << output;
    out.device()->seek(0);
    out << quint16(data_.size() - sizeof(quint16));

    // Send package
    socket_->write(data_);
}
}
