#include "websocketserver.h"


WebSocketServer::WebSocketServer(Users* _users, QObject *parent) : QObject(parent), m_server(new QWebSocketServer("WebSocket Server", QWebSocketServer::NonSecureMode, this))
{
    users = _users;
    connect(m_server, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
}

void WebSocketServer::startServer()
{
    m_server->listen(QHostAddress::Any, 1234);
    QHostAddress  m_address = m_server->serverAddress();
    quint16  m_port = m_server->serverPort();
    qDebug() << m_address.toString() + ":" + QString::number(m_port);
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *socket = m_server->nextPendingConnection();
    qDebug() << "New Client Connected";
    connect(socket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processMessage);
    connect(socket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);
    m_clients.append(socket);
}

void WebSocketServer::processMessage(QString message)
{
    qDebug() << message;
    QJsonDocument doc1 = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj = doc1.object();
    qDebug() << obj;
    if(message == "history")
    {
        QJsonDocument doc(users->get_history());
        QString strJson(doc.toJson(QJsonDocument::Compact));
        sendMessageToClients(strJson);
    }
    else if (obj.contains("admin"))
    {
        QJsonObject admin = obj.value("admin").toObject();
        QString id = admin.value("id").toString();
        QString pass = admin.value("pass").toString();
        QString message = users->validate_admin(id, pass);
        qDebug() << message;
        sendMessageToClients(message);

    }
    message.clear();
}
void WebSocketServer::socketDisconnected()
{
    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    if (socket) {
        m_clients.removeAll(socket);
        socket->deleteLater();
        qDebug() << "close connection";
    }
}

void WebSocketServer::sendMessageToClients(QString message)
{
    for (QWebSocket *client : std::as_const(m_clients)) {
        client->sendTextMessage(message);
    }
}
