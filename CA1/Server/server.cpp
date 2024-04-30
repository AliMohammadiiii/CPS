#include "server.h"

server::server(QObject *parent)
    : QObject{parent}
{
    users = new Users();
    init_user();
    users->addAdmin("ali", "123");
    httpServer = new HttpServer(users);
    webSocketServer = new WebSocketServer(users);
}

void server::init_user()
{
    QFile file;
    file.setFileName("./userLists.json");
    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open file";
    }
    else
    {
        qDebug() << "Opened File";
    }
    QJsonDocument doc=QJsonDocument::fromJson(file.readAll());
    QJsonObject root_obj = doc.object();
    QJsonArray user_list = root_obj.value("userList").toArray();
    qDebug() << user_list.size();
    for (int var = 0; var < user_list.size(); ++var) {
        QJsonObject user_obj = user_list[var].toObject();
        QString idVal = user_obj.value("id").toString();

        users->addUser(idVal);
    }
    file.close();
}

void server::run()
{
    webSocketServer->startServer();
    QObject::connect(users, &Users::newuser, webSocketServer, &WebSocketServer::sendMessageToClients);
    qDebug() << "Running Server";
    httpServer->serverRun(80);
    qDebug() << "Run Server";
}
