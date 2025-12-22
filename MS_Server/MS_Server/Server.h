#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>

namespace MS {
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
    SetVesselInfo,              // Set geo and meteo data to the data base
    GetNearLocations,           // Send request to get nearest vessels locations
    GetVesselData,              // Get data of vessel by data base index
};

// Data base tables imitation
struct VesselTableString {
    QDateTime time;
    double latitude_coord;
    double longtitude_coord;
    double wind_force;
    double wave_heignt;
};

// table of the vessel
struct VesselTable {
    QString name;
    QMap<int, VesselTableString> table;
};

struct MockInfo {
    int id = 1;
    QString name = "1";
    QString pass = "1";

    QMap<int, VesselTable> vessels;
};

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
    void dataBaseRequest(ServerActionType actionType, const QVector<QString>& info);
    void sendToClient(ServerActionType actionType, const QVector<QString>& info);

    // Database tools
    void addUser(const QVector<QString>& info);
    void checkUserStatement(const QVector<QString>& info);
    void setVesselInfo(const QVector<QString>& info);
    void getNearLocations(const QVector<QString>& info);
    void getVesselData(const QVector<QString>& info);

private:
    quint16 blockSize_ = 0;             // Size of data package

    QVector<QTcpSocket*> sockets_;      // Vector of "users" of server
    QByteArray data_;                   // Information byte array which server sents to client and get from it

    MockInfo mockInfo_;                   // Info for tests
};
}
