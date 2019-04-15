#ifndef SERVER_H
#define SERVER_H

#include <unordered_map>
#include <memory>

#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>

class Client
{
    public: 
        std::shared_ptr<asio::ip::tcp::socket> socket;
        std::array<char, 35> data = {};
};

class Server
{

public:
    enum class Command : uint8_t
    {
       Pause,
       Seek
    };

    Server(asio::io_context &io_ctx, unsigned short port);

    void acceptClient();

    void readHeader(const int &clientId);

    void acceptConnection(std::error_code ec, asio::ip::tcp::socket socket);

    // void clientDisconnected(int id);

    // void dataRecieved(int id);

    // void handleCommand(int issuerId, Command command, QVariantList arguments = QVariantList());

private:
    
    asio::ip::tcp::acceptor synchroAcceptor;

    std::unordered_map<int, std::shared_ptr<Client>> clientList;

    int nextClientId;
};

#endif // SERVER_H
