#ifndef CPSWEBSOCKET_H
#define CPSWEBSOCKET_H



#include <QObject>
#include <QWebSocket>
#include <QtNetwork/qnetworkaccessmanager.h>

#include <QObject>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
class WebSocketClient : public QObject
{
    Q_OBJECT

public:
    explicit WebSocketClient(QObject *parent = nullptr);
    ~WebSocketClient();

signals:
    void closed();
    void connectionStateChanged(bool isConnected);
    void newuser(const QString &username, const QString &date, const QString &time);
    void history(const QJsonArray &his);

public slots:
    void onConnected(const QString &username, const QString &password);
    void onDisconnected();
    void onTextMessageReceived(const QString &message);
    void connectToServer(const QString &address, const QString &username, const QString &password);
    void sendMessage(const QString &message);
private:
    QWebSocket *m_webSocket;
    QUrl m_url;
};



#endif // CPSWEBSOCKET_H
