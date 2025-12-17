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
        // Mock work
        if (info[0] == mockInfo_.name) {
            answerToClient.push_back("0");
        }
        else {
            mockInfo_.name = info[0];
            mockInfo_.pass = info[1];
            answerToClient.push_back("1");
        }
        sendToClient(ServerActionType::AddNewUser, answerToClient);
        //addUser(info);
        break;
    }
    case ServerActionType::CheckUserStatement: {
        // mock work
        if (info[0] == mockInfo_.name && info[1] == mockInfo_.pass) {
            answerToClient.push_back("1");
        }
        sendToClient(ServerActionType::CheckUserStatement, answerToClient);
        //checkUserStatement(info);
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
}
