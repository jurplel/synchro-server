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
    quint8 extraFieldCount;
    quint8 numericCommand;
    QVariantList additionalData;
    *cliObj.in >> incomingData >> extraFieldCount >> numericCommand;

    for (int i = extraFieldCount; i > 0; i--)
    {
        *cliObj.in >> additionalData;
    }

    if (!cliObj.in->commitTransaction())
        return;

    auto command = static_cast<Command>(numericCommand);

    qInfo() << "Recieved new data from" << id << ":" << incomingData << extraFieldCount << command << additionalData;

    handleCommand(id, command, additionalData);

    if (!cliObj.in->atEnd())
        dataRecieved(id);
}

void Server::handleCommand(int issuerId, Command command, QVariantList data)
{
    QByteArray dataBlock;
    QDataStream dataBlockStream(&dataBlock, QIODevice::WriteOnly);
    dataBlockStream << quint16(0);

    switch(command) {
    case Command::Pause: {
        dataBlockStream << quint8(1) << quint8(Command::Pause) << data;
        break;
    }
    case Command::Seek: {
        dataBlockStream << quint8(2) << quint8(Command::Seek) << data;
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
