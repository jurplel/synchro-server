#include "server.h"

#include <QTcpSocket>
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any, 32019);
    connect(server, &QTcpServer::newConnection, this, &Server::newClientConnected);
}

void Server::newClientConnected()
{
    qInfo() << "New client connected";
    auto socket = server->nextPendingConnection();
    socket->write("All systems nominal");
}
