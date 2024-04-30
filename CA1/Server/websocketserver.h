
// server.h
#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QList>
#include <QJsonDocument>
#include <QJsonObject>
#include "users.h"

class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketServer(Users* _users, QObject *parent = nullptr);
    void startServer();

public slots:
    void sendMessageToClients(QString message);

private slots:
    void onNewConnection();
    void processMessage(QString message);
    void socketDisconnected();

private:
    QWebSocketServer *m_server;
    QList<QWebSocket *> m_clients;
    Users* users;
};

#endif // SERVER_H
