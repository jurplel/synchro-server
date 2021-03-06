#include "server.h"

#include <QDebug>

Server::Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    nextClientId = 0;

    server->listen(QHostAddress::Any, 32019);
    if (server->isListening())
        qInfo() << "Server sucessfully started.";
    connect(server, &QTcpServer::newConnection, this, &Server::clientConnected);
}

void Server::clientConnected()
{
    auto socket = server->nextPendingConnection();

    //add client to clientlist
    ClientObject newClient;
    newClient.socket = socket;
    newClient.in = new QDataStream(socket);
    newClient.in->setVersion(QDataStream::Qt_5_9);

    int id = nextClientId;
    clientList.insert(id, newClient);
    nextClientId++;

    //make connections
    connect(socket, &QTcpSocket::disconnected, [this, id]{ clientDisconnected(id); });
    connect(socket, &QTcpSocket::readyRead, [this, id]{ dataRecieved(id); });

    //console message
    qInfo() << "Client" << id << "connected.";
}

void Server::clientDisconnected(int id)
{
    clientList.value(id).socket->deleteLater();
    clientList.remove(id);
    qInfo() << "Client" << id << "disconnected.";
}

void Server::dataRecieved(int id)
{
    ClientObject cliObj = clientList.value(id);

    cliObj.in->startTransaction();

    quint16 incomingData;
    bool hasArguments;
    quint8 numericCommand;
    QVariantList arguments;
    *cliObj.in >> incomingData >> hasArguments >> numericCommand;

    if (hasArguments)
        *cliObj.in >> arguments;

    if (!cliObj.in->commitTransaction())
        return;

    auto command = static_cast<Command>(numericCommand);

    qInfo() << "Recieved new data from" << QString::number(id) + ":" << incomingData << hasArguments << command << arguments;

    handleCommand(id, command, arguments);

    if (!cliObj.in->atEnd())
        dataRecieved(id);
}

void Server::handleCommand(int issuerId, Command command, QVariantList arguments)
{
    QByteArray dataBlock;
    QDataStream dataBlockStream(&dataBlock, QIODevice::WriteOnly);
    dataBlockStream << quint16(0);

    switch(command) {
    case Command::Pause: {
        dataBlockStream << true << quint8(Command::Pause) << arguments;
        break;
    }
    case Command::Seek: {
        dataBlockStream << true << quint8(Command::Seek) << arguments;
        break;
    }
    }

    dataBlockStream.device()->seek(0);
    dataBlockStream << quint16(dataBlock.size() - static_cast<int>(sizeof(quint16)));

    foreach (int id, clientList.keys())
    {
        if (issuerId == id)
            continue;

        auto cliObj = clientList.value(id);
        cliObj.socket->write(dataBlock);
    }
}
