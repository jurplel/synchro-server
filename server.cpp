#include "server.h"

#include <QDebug>

Server::Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    nextClientId = 0;

    server->listen(QHostAddress::Any, 32019);
    connect(server, &QTcpServer::newConnection, this, &Server::clientConnected);
}

void Server::clientConnected()
{
    auto socket = server->nextPendingConnection();
    socket->write("All systems nominal");

    //add client to clientlist
    ClientObject newClient;
    newClient.socket = socket;
    clientList.insert(nextClientId, newClient);

    //connect to discconection signal
    connect(socket, &QTcpSocket::disconnected, this, [this]{ clientDisconnected(nextClientId); });

    //console message
    qInfo() << "Client " << nextClientId << " connected.";

    //increase this value so the next assigned value is always different
    nextClientId++;
}

void Server::clientDisconnected(int id)
{
    clientList.value(id).socket->deleteLater();
    clientList.remove(id);
    qInfo() << "Client " << id << " disconnected.";
}
