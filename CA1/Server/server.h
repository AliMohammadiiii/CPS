#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "users.h"
#include "httpserver.h"
#include "websocketserver.h"

class server : public QObject
{
    Q_OBJECT
public:
    explicit server(QObject *parent = nullptr);
    void run();
signals:

private:
    Users *users;
    HttpServer *httpServer;
    WebSocketServer *webSocketServer;

private:
    void init_user();
};

#endif // SERVER_H
