#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    void newClientConnected();

signals:

public slots:

private:
    QTcpServer *server;
};

#endif // SERVER_H
