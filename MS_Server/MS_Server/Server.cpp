#include "Server.h"

namespace MS{
Server::Server() {
    // Checks server state, if it works said start
    if (this->listen(QHostAddress::Any, 2323)) {
        qDebug() << "start";
    }
    else {
        qDebug() << "error";
    }
}

// Create connection with database
bool Server::connectToDatabase() {
    // Database which contains all information about users and waters
    QSqlDatabase MSDB = QSqlDatabase::addDatabase("QPSQL");
    MSDB.setDatabaseName("MS_DataBase");
    MSDB.setUserName("postgres");
    MSDB.setPassword("1337");

    if (!MSDB.open()) {
        QSqlError error = MSDB.lastError();
        qCritical() << "waters database error" << error.text();
        return false;
    }

    MSDB.tables(QSql::AllTables);
    qCritical() << "Database opened, tables:" << MSDB.tables(QSql::Tables);
    return true;
}

// Handler of a new connections
void Server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);

    //connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(socket, &QTcpSocket::readyRead, this, &MS::Server::slotReadyRead);

    sockets_.push_back(socket);

    qDebug() << "client connected: " << socketDescriptor;
}

// Handler of a client's disconnection
void Server::clienDisconnected() {
    // The socket which sent a request
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    // Delete socket of a client which left
    sockets_.erase(std::remove(sockets_.begin(), sockets_.end(), socket), sockets_.end());
    socket->deleteLater();
}

// Handler of a client's messages
void Server::slotReadyRead() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    if (!socket) {
        return;
    }

    QDataStream input(socket);

    input.setVersion(QDataStream::Qt_6_10);
    if (input.status() == QDataStream::Ok) {
        while(true) {
            // Calculate size of the block of data
            if (blockSize_ == 0) {
                if (socket->bytesAvailable() < 2) {
                    break;
                }

                // Write the block size
                input >> blockSize_;
            }

            // If the block size is bigger than data from client, we should wait to other data
            if (socket->bytesAvailable() < blockSize_) {
                qDebug() << "data not full, break";
                break;
            }

            // Write data from user
            QVector<QString> info;
            ServerActionType actionType;

            input >> actionType >> info;

            dataBaseRequest(actionType, info);

            blockSize_ = 0;
            break;
        }
    }
    else {
        qDebug() << "DataStream error";
    }
}

// Request to a data base based on action type
void Server::dataBaseRequest(ServerActionType actionType, const QVector<QString>& info) {
    QVector<QString> answerToClient;

    switch (actionType) {
    case ServerActionType::AddNewUser: {
        if (info.size() != 3) {
            break;
        }

        // Mock work
        if (info[0] == mockInfo_.name) {
            answerToClient.push_back("0");
        }
        else {
            mockInfo_.name = info[0];
            mockInfo_.pass = info[1];
            answerToClient.push_back("1");
            answerToClient.push_back(QString::number(mockInfo_.id));
        }
        sendToClient(ServerActionType::AddNewUser, answerToClient);
        //addUser(info);
        break;
    }
    case ServerActionType::CheckUserStatement: {
        if (info.size() != 2) {
            break;
        }

        // mock work
        if (info[0] == mockInfo_.name && info[1] == mockInfo_.pass) {
            answerToClient.push_back("1");
            answerToClient.push_back(QString::number(mockInfo_.id));
        }
        sendToClient(ServerActionType::CheckUserStatement, answerToClient);
        //checkUserStatement(info);
        break;
    }
    case ServerActionType::SetVesselInfo: {
        if (info.size() != 6) {
            break;
        }

        //setVesselInfo(info);

        break;
    }
    case ServerActionType::GetNearLocations: {
        if (info.size() != 3) {
            break;
        }

        answerToClient.push_back("10");
        answerToClient.push_back("0.92");
        answerToClient.push_back("-9.63");

        answerToClient.push_back("11");
        answerToClient.push_back("0.15");
        answerToClient.push_back("-9.01");

        answerToClient.push_back("12");
        answerToClient.push_back("-0.84");
        answerToClient.push_back("-8.37");

        answerToClient.push_back("13");
        answerToClient.push_back("-1.63");
        answerToClient.push_back("-7.94");

        answerToClient.push_back("14");
        answerToClient.push_back("-2.47");
        answerToClient.push_back("-7.12");

        answerToClient.push_back("15");
        answerToClient.push_back("-3.31");
        answerToClient.push_back("-6.55");

        answerToClient.push_back("16");
        answerToClient.push_back("-4.08");
        answerToClient.push_back("-5.89");

        answerToClient.push_back("17");
        answerToClient.push_back("-4.96");
        answerToClient.push_back("-5.21");

        answerToClient.push_back("18");
        answerToClient.push_back("-5.74");
        answerToClient.push_back("-4.66");

        answerToClient.push_back("19");
        answerToClient.push_back("-6.59");
        answerToClient.push_back("-3.98");

        answerToClient.push_back("20");
        answerToClient.push_back("-7.41");
        answerToClient.push_back("-3.35");

        answerToClient.push_back("21");
        answerToClient.push_back("-8.27");
        answerToClient.push_back("-2.71");

        answerToClient.push_back("22");
        answerToClient.push_back("-9.06");
        answerToClient.push_back("-2.14");

        answerToClient.push_back("23");
        answerToClient.push_back("-9.93");
        answerToClient.push_back("-1.46");

        answerToClient.push_back("24");
        answerToClient.push_back("-10.78");
        answerToClient.push_back("-0.83");

        answerToClient.push_back("25");
        answerToClient.push_back("-11.64");
        answerToClient.push_back("-0.21");

        answerToClient.push_back("26");
        answerToClient.push_back("-12.49");
        answerToClient.push_back("0.44");

        answerToClient.push_back("27");
        answerToClient.push_back("-13.37");
        answerToClient.push_back("1.08");

        answerToClient.push_back("28");
        answerToClient.push_back("-14.22");
        answerToClient.push_back("1.71");

        answerToClient.push_back("29");
        answerToClient.push_back("-15.04");
        answerToClient.push_back("2.36");

        answerToClient.push_back("30");
        answerToClient.push_back("-15.91");
        answerToClient.push_back("3.02");

        answerToClient.push_back("31");
        answerToClient.push_back("-16.78");
        answerToClient.push_back("3.69");

        answerToClient.push_back("32");
        answerToClient.push_back("-17.64");
        answerToClient.push_back("4.31");

        answerToClient.push_back("33");
        answerToClient.push_back("-18.47");
        answerToClient.push_back("4.96");

        answerToClient.push_back("34");
        answerToClient.push_back("-19.32");
        answerToClient.push_back("5.62");

        answerToClient.push_back("35");
        answerToClient.push_back("-20.18");
        answerToClient.push_back("6.29");

        answerToClient.push_back("36");
        answerToClient.push_back("-21.04");
        answerToClient.push_back("6.94");

        answerToClient.push_back("37");
        answerToClient.push_back("-21.91");
        answerToClient.push_back("7.61");

        answerToClient.push_back("38");
        answerToClient.push_back("-22.76");
        answerToClient.push_back("8.27");

        sendToClient(ServerActionType::GetNearLocations, answerToClient);
        //getNearLocations(info);

        break;
    }
    case ServerActionType::GetVesselData: {
        if (info.size() != 2) {
            break;
        }

        //getVesselData(info);

        answerToClient.push_back("road");
        answerToClient.push_back("1.22");
        answerToClient.push_back("-10.23");
        answerToClient.push_back("1.12");
        answerToClient.push_back("-10.03");
        answerToClient.push_back("1.02");
        answerToClient.push_back("-9.93");
        answerToClient.push_back("0.95");
        answerToClient.push_back("-9.83");
        answerToClient.push_back("0.92");
        answerToClient.push_back("-9.63");

        answerToClient.push_back("date");
        answerToClient.push_back("2025-12-20 10:00:00+00");
        answerToClient.push_back("2025-12-20 10:01:00+00");
        answerToClient.push_back("2025-12-20 10:02:00+00");
        answerToClient.push_back("2025-12-20 10:03:00+00");
        answerToClient.push_back("2025-12-20 10:04:00+00");
        answerToClient.push_back("2025-12-20 10:05:00+00");
        answerToClient.push_back("2025-12-20 10:06:00+00");
        answerToClient.push_back("2025-12-20 10:07:00+00");
        answerToClient.push_back("2025-12-20 10:08:00+00");
        answerToClient.push_back("2025-12-20 10:09:00+00");
        answerToClient.push_back("2025-12-20 10:10:00+00");
        answerToClient.push_back("2025-12-20 10:11:00+00");
        answerToClient.push_back("2025-12-20 10:12:00+00");
        answerToClient.push_back("2025-12-20 10:13:00+00");
        answerToClient.push_back("2025-12-20 10:14:00+00");
        answerToClient.push_back("2025-12-20 10:15:00+00");
        answerToClient.push_back("2025-12-20 10:16:00+00");

        answerToClient.push_back("wind");
        answerToClient.push_back("17.0");
        answerToClient.push_back("17.2");
        answerToClient.push_back("17.6");
        answerToClient.push_back("17.4");
        answerToClient.push_back("18.6");
        answerToClient.push_back("18.3");
        answerToClient.push_back("18.0");
        answerToClient.push_back("17.9");
        answerToClient.push_back("18.6");
        answerToClient.push_back("18.9");
        answerToClient.push_back("19.0");
        answerToClient.push_back("19.3");
        answerToClient.push_back("19.6");
        answerToClient.push_back("19.4");
        answerToClient.push_back("19.6");
        answerToClient.push_back("19.8");
        answerToClient.push_back("20.0");

        answerToClient.push_back("wave");
        answerToClient.push_back("7.0");
        answerToClient.push_back("7.1");
        answerToClient.push_back("7.3");
        answerToClient.push_back("7.0");
        answerToClient.push_back("7.6");
        answerToClient.push_back("7.3");
        answerToClient.push_back("8.0");
        answerToClient.push_back("8.9");
        answerToClient.push_back("8.6");
        answerToClient.push_back("8.9");
        answerToClient.push_back("9.0");
        answerToClient.push_back("9.3");
        answerToClient.push_back("9.6");
        answerToClient.push_back("9.4");
        answerToClient.push_back("9.6");
        answerToClient.push_back("10.0");
        answerToClient.push_back("10.2");

        sendToClient(ServerActionType::GetVesselData, answerToClient);
        break;
    }
    }
}

void Server::sendToClient(ServerActionType actionType, const QVector<QString>& output) {
    data_.clear();

    QDataStream out(&data_, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_10);

    // Calculate and write a size of the sent data package
    out << quint16(0) << actionType << output;
    out.device()->seek(0);
    out << quint16(data_.size() - sizeof(quint16));

    // Send package
    for (auto& s : sockets_) {
        s->write(data_);
    }
}

// Checks is user exists, if not, adds it in database
void Server::addUser(const QVector<QString>& info) {
    QVector<QString> answerToClient;

    // Check is user with the same login exists
    bool isUserExists = false;
    QSqlQuery query;
    query.exec("SELECT * FROM Users");

    while (query.next()) {
        QString dbLogin = query.value("login").toString();

        if (info[0] == dbLogin) {
            isUserExists = true;
        }
    }

    if (isUserExists) {
        answerToClient.push_back("0");
    }
    else {
        answerToClient.push_back("1");

        QString insertInDB = "INSERT INTO public.\"Users\" (login, password, email) VALUES ('" +
                             info[0] + "', '" + info[1] + "', '" + info[2] + "');";

        query.exec(insertInDB);
    }

    sendToClient(ServerActionType::AddNewUser, answerToClient);
}


// Checks is user exists and entered password was right
void Server::checkUserStatement(const QVector<QString>& info) {
    QVector<QString> answerToClient;

    // Check is user with the same login and pasword exists
    bool isUserExists = false;
    QSqlQuery query;
    query.exec("SELECT * FROM public.Users");

    while (query.next()) {
        QString dbLogin = query.value("login").toString();
        QString dbPassword = query.value("password").toString();

        if (info[0] == dbLogin && info[1] == dbPassword) {
            isUserExists = true;
        }
    }

    if (isUserExists) {
        answerToClient.push_back("1");
    }
    else {
        answerToClient.push_back("0");
    }

    sendToClient(ServerActionType::CheckUserStatement, answerToClient);
}

// Add new mesurements to vessel's table
void Server::setVesselInfo(const QVector<QString>& info) {
    QSqlQuery query;

    QString insertInDB = "INSERT INTO public.\"Users\" (login, password, email) VALUES ('" +
                         info[0] + "', '" + info[1] + "', '" + info[2] + "');";

    query.exec(insertInDB);
}

// Find all vessels based by search radius and send their locations and ids to user
void Server::getNearLocations(const QVector<QString>& info) {
    QVector<QString> answerToClient;

    QSqlQuery query;
    query.exec("SELECT * FROM public.Users");

    while (query.next()) {
        QString dbLogin = query.value("login").toString();
        QString dbPassword = query.value("password").toString();

        answerToClient.push_back("");
    }

    sendToClient(ServerActionType::GetNearLocations, answerToClient);
}

// Get measurments of vessel by data base index
void Server::getVesselData(const QVector<QString>& info) {

}
}
