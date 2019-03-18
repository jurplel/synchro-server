#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include <QDataStream>

class Server : public QObject
{
    Q_OBJECT

public:
    enum class Command : quint8
    {
       Pause
    };
    Q_ENUM(Command)

    struct ClientObject
    {
        QTcpSocket *socket;
        QDataStream *in;
    };

    explicit Server(QObject *parent = nullptr);

    void clientConnected();

    void clientDisconnected(int id);

    void dataRecieved(int id);

    void handleCommand(Command command, int issuerId);

signals:

public slots:

private:
    QTcpServer *server;

    QHash<int, ClientObject> clientList;

    int nextClientId;
};

#endif // SERVER_H
