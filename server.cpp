#include "server.h"

#include <iostream>
#include <utility>

#include <asio/error.hpp>
#include <asio/buffer.hpp>

using asio::ip::tcp;

Server::Server(asio::io_context &io_ctx, unsigned short port) : synchroAcceptor(io_ctx, tcp::endpoint(tcp::v4(), port))
{
    nextClientId = 0;

    if (!synchroAcceptor.is_open())
        throw std::runtime_error("Failed to open socket");
        
    std::cout << "Server successfully started" << std::endl;

    acceptClient();

    // server->listen(QHostAddress::Any, 32019);
    // if (server->isListening())
    //     qInfo() << "Server sucessfully started.";
    // connect(server, &QTcpServer::newConnection, this, &Server::clientConnected);
}

void Server::acceptClient()
{
    synchroAcceptor.async_accept(
    [this](const std::error_code& ec, tcp::socket socket)
    {
        if (ec)
        {
            std::cerr << "Connection error: " << ec.message() << std::endl;
        }
        else
        {
            int id = nextClientId;
            auto socketPointer = std::make_shared<tcp::socket>(std::move(socket));
            clientList.insert({id, socketPointer});
            nextClientId++;

            receiveData(id);

            std::cout << "Client " << id << " connected" << std::endl;
        }

        acceptClient();
    });
}

void Server::receiveData(const int clientId, const int remainingDataToRead)
{
    char data[1024];
    auto buffer = asio::buffer(data, 1024);

    clientList[clientId]->async_receive(buffer, 
    [this, clientId, remainingDataToRead](const std::error_code& ec, std::size_t bytesTransferred)
    {

        int newRemainingDataToRead = 0;

        if (ec)
        {
            if ((asio::error::eof == ec) || (asio::error::connection_reset == ec))
            {
                clientList.erase(clientId);
                std::cout << "Client " << clientId  << " disconnected - " << ec.message() << std::endl;
                return;
            }

            std::cerr << "Data Recieve Error: " << ec.message() << std::endl;
        }
        else
        {
            std::cout << "Recieved " << bytesTransferred << " bytes" << std::endl;
        }

        receiveData(clientId, newRemainingDataToRead);
    });
}

void Server::acceptConnection(std::error_code ec, tcp::socket socket)
{

//     auto socket = server->nextPendingConnection();

//     //add client to clientlist
//     ClientObject newClient;
//     newClient.socket = socket;
//     newClient.in = new QDataStream(socket);
//     newClient.in->setVersion(QDataStream::Qt_5_9);

//     int id = nextClientId;
//     clientList.insert(id, newClient);
//     nextClientId++;

//     //make connections
//     connect(socket, &QTcpSocket::disconnected, [this, id]{ clientDisconnected(id); });
//     connect(socket, &QTcpSocket::readyRead, [this, id]{ dataRecieved(id); });

//     //console message
//     qInfo() << "Client" << id << "connected.";
}

// void Server::clientDisconnected(int id)
// {
//     clientList.value(id).socket->deleteLater();
//     clientList.remove(id);
//     qInfo() << "Client" << id << "disconnected.";
// }

// void Server::dataRecieved(int id)
// {
//     ClientObject cliObj = clientList.value(id);

//     cliObj.in->startTransaction();

//     quint16 incomingData;
//     bool hasArguments;
//     quint8 numericCommand;
//     QVariantList arguments;
//     *cliObj.in >> incomingData >> hasArguments >> numericCommand;

//     if (hasArguments)
//         *cliObj.in >> arguments;

//     if (!cliObj.in->commitTransaction())
//         return;

//     auto command = static_cast<Command>(numericCommand);

//     qInfo() << "Recieved new data from" << QString::number(id) + ":" << incomingData << hasArguments << command << arguments;

//     handleCommand(id, command, arguments);

//     if (!cliObj.in->atEnd())
//         dataRecieved(id);
// }

// void Server::handleCommand(int issuerId, Command command, QVariantList arguments)
// {
//     QByteArray dataBlock;
//     QDataStream dataBlockStream(&dataBlock, QIODevice::WriteOnly);
//     dataBlockStream << quint16(0);

//     switch(command) {
//     case Command::Pause: {
//         dataBlockStream << true << quint8(Command::Pause) << arguments;
//         break;
//     }
//     case Command::Seek: {
//         dataBlockStream << true << quint8(Command::Seek) << arguments;
//         break;
//     }
//     }

//     dataBlockStream.device()->seek(0);
//     dataBlockStream << quint16(dataBlock.size() - static_cast<int>(sizeof(quint16)));

//     foreach (int id, clientList.keys())
//     {
//         if (issuerId == id)
//             continue;

//         auto cliObj = clientList.value(id);
//         cliObj.socket->write(dataBlock);
//     }
// }
