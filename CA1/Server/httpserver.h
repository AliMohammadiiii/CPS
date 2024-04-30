#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>

#include <QHttpServer>
#include <QHttpServerResponse>
#include <QCoreApplication>

#include <QFile>
#include <QJsonObject>
#include <QString>
#include <Qvector>
#include <Qjsondocument.h>
#include <QJsonArray>
#include "users.h"

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(Users* _users, QObject *parent = nullptr);
    int serverRun(int port);
signals:

private:
    void route();
    QHttpServer *httpServer;
    Users *users;

};

#endif // HTTPSERVER_H
