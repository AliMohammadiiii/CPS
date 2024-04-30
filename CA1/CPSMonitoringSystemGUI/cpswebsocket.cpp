#include "cpswebsocket.h"

#include <QDebug>

WebSocketClient::WebSocketClient(QObject *parent)
    : QObject(parent)
{
    m_webSocket = new QWebSocket();
    connect(m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
}

void WebSocketClient::connectToServer(const QString &address, const QString &username, const QString &password)
{
    // connect(m_webSocket, &QWebSocket::connected, this, );
    connect(m_webSocket, &QWebSocket::connected, [=]() {
        onConnected(username, password);
    });
    qDebug() << address;
    m_webSocket->open(QUrl(QUrl(address)));
}

WebSocketClient::~WebSocketClient()
{
    m_webSocket->close();
}

void WebSocketClient::onConnected(const QString &username, const QString &password)
{
    qDebug() << "WebSocket connected";
    connect(m_webSocket, &QWebSocket::textMessageReceived,
            this, &WebSocketClient::onTextMessageReceived);

    QJsonObject message;
    QJsonObject admin;
    admin["id"] = username;
    admin["pass"] = password;
    message["admin"] = admin;
    QJsonDocument jsonDoc(message);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);
    qDebug() << jsonString;
    m_webSocket->sendTextMessage(jsonString);
}

void WebSocketClient::sendMessage(const QString &message)
{
    m_webSocket->sendTextMessage(message);
}
void WebSocketClient::onDisconnected()
{
    qDebug() << "WebSocket disconnected.";
    emit connectionStateChanged(false);
    emit closed();
}

void WebSocketClient::onTextMessageReceived(const QString &message)
{
    qDebug() << "Message received:" << message;
    if(message == "accept")
    {
        emit connectionStateChanged(true);
    }
    else if(message == "reject")
    {
        m_webSocket->close();
        QWebSocket *tmp = m_webSocket;
        m_webSocket = new QWebSocket();
        delete tmp;

    }
    QJsonDocument doc1 = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject obj = doc1.object();
    if (obj.contains("user"))
    {
        QJsonObject user = obj.value("user").toObject();
        QString id = user.value("id").toString();
        QString date = user.value("date").toString();
        QString time = user.value("time").toString();
        emit newuser(id, date, time);
    }
    else if(obj.contains("history"))
    {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(message.toUtf8());
        //
        QJsonArray jsonObject = jsonResponse.object()["history"].toArray();
        emit history(jsonObject);
    }
}

