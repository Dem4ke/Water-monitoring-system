#pragma once

#include <QObject>
#include <QTcpSocket>

namespace Component {
/*  Flags to work with database is:
    1 - insert user in database
    2 - check user statement
    3 - get waters names
    4 - get info about choosen water
*/

// Variables of the server's actions
enum class ServerActionType : int {
    AddNewUser = 0,             // Insert user in database
    CheckUserStatement,         // Check user statement (are login and password correct)
};

class SocketComponent : public QObject {
    Q_OBJECT

public:
    static SocketComponent& Instance();

    void checkUserStatement(QVector<QString> info);
    void addUser(QVector<QString> info);

private slots:
    // Server tools
    void slotReadyRead();
    void sendToServer(ServerActionType dataType, QVector<QString> output);

signals:
    void getUserStatus(bool isLogAvalible);

private:
    SocketComponent(QObject* parent = nullptr);

    QTcpSocket* socket_ = nullptr;      // Socket which uses to work with server
    QByteArray data_;                   // Array of the data
    quint16 blockSize_ = 0;             // Size of data package
};

#define SOCKET SocketComponent::Instance()
}
