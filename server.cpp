#include "server.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent)
{
    auto server = new QTcpServer();
    server->listen(QHostAddress::Any, 32019);
    connect(server, &QTcpServer::newConnection,
    [server]{
        qInfo() << "recieved connection, sending handshake";
        auto socket = server->nextPendingConnection();
        socket->write("all systems nominal");
    });
}
