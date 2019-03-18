#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>

class Server : public QObject
{
    Q_OBJECT

    struct ClientObject
    {
        QTcpSocket *socket;
    };

public:
    explicit Server(QObject *parent = nullptr);

    void clientConnected();

    void clientDisconnected(int id);

signals:

public slots:

private:
    QTcpServer *server;

    QHash<int, ClientObject> clientList;

    int nextClientId;
};

#endif // SERVER_H
