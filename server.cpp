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
    socket->write("All systems nominal");

    //add client to clientlist
    ClientObject newClient;
    newClient.socket = socket;

    int id = nextClientId;
    clientList.insert(id, newClient);
    nextClientId++;

    //connect to discconection signal
    connect(socket, &QTcpSocket::disconnected, this, [this, id]{ clientDisconnected(id); });

    //console message
    qInfo() << "Client" << id << "connected.";
}

void Server::clientDisconnected(int id)
{
    clientList.value(id).socket->deleteLater();
    clientList.remove(id);
    qInfo() << "Client" << id << "disconnected.";
}
