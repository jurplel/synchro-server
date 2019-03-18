#include "server.h"

#include <QDebug>
#include <QDataStream>

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
    QByteArray dataBlock;
    QDataStream dataBlockStream(&dataBlock, QIODevice::WriteOnly);
    dataBlockStream << quint16(0) << quint8(Command::Pause);
    dataBlockStream.device()->seek(0);
    dataBlockStream << quint16(dataBlock.size() - static_cast<int>(sizeof(quint16)));

    socket->write(dataBlock);

    //add client to clientlist
    ClientObject newClient;
    newClient.socket = socket;
    newClient.incomingSize = 0;

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

    if (!cliObj.incomingSize)
    {
        QByteArray data = cliObj.socket->read(2);
        QDataStream dataStream(data);
        dataStream >> cliObj.incomingSize;
    }
    qDebug() << cliObj.incomingSize;
    qDebug() << cliObj.socket->bytesAvailable();
    if (cliObj.socket->bytesAvailable() != cliObj.incomingSize)
        return;

    cliObj.incomingSize = 0;

    QByteArray data = cliObj.socket->read(cliObj.incomingSize);
    QDataStream dataStream(data);

    quint8 numericCommand;
    dataStream >> numericCommand;
    auto command = static_cast<Command>(numericCommand);

    qDebug() << "Recieved new data:" << command;
}
