#pragma once

#include <QObject>
#include <QTcpSocket>

namespace MS {
/*  Flags to work with database is:
    1 - insert user in database
    2 - check user statement
    3 - get waters names
    4 - get info about choosen water
*/

// Variables of the server's information
enum class DataType : int {
    AddUser = 0,    // Insert user in database
    CheckUser,      // Check user statement (are login and password correct)
};

class SocketComponent : public QObject {
    Q_OBJECT

public:
    explicit SocketComponent(QObject* parent = nullptr);

public slots:
    // Server tools
    void slotReadyRead();
    void sendToServer(DataType dataType, QVector<QString> output);

private:


signals:

private:
    QTcpSocket* socket_ = nullptr;      // Socket which uses to work with server
    QByteArray data_;                   // Array of the data
    quint16 blockSize_ = 0;             // Size of data package
};
}
