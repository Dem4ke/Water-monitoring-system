#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

namespace MS {
class Server : public QTcpServer {
    Q_OBJECT

public:
    Server();

    // Database work tools
    bool connectToDatabase();

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void clienDisconnected();
    void slotReadyRead();

private:
    void sendToClient(int flag, QVector<QString> output);

    // Database users tools
    void addUser(QVector<QString> userInfo);
    void checkUserStatement(QVector<QString> userInfo);

    // Database waters tools
    void getWatersNames();
    void getWaterInfo(QVector<QString> waterName);

private:
    quint16 blockSize_ = 0;             // Size of data package

    QTcpSocket* socket_;
    QVector<QTcpSocket*> sockets_;      // Vector of "users" of server
    QByteArray data_;                   // Information byte array which server sents to client and get from it
};
}

/*  Flags to work with database is:
    1 - insert user in database
    2 - check user statement (is login and password are correct)
    3 - get waters names
    4 - get info about choosen water
*/
