#include "httpserver.h"

HttpServer::HttpServer(Users* _users, QObject *parent)
    : QObject{parent}
{
    httpServer = new QHttpServer();
    users = _users;

}
void HttpServer::route()
{

}
int HttpServer::serverRun(int _port)
{
    httpServer->route("/hello/", []() {
        qDebug() << "Call";
        return "Hello world";
    });
    qDebug () << "Route initializing";

    httpServer->route("/login/<arg>", [this](QString id, const QHttpServerRequest &request) {
        qDebug() << "Login";
        QString message = this->users->validate_user(id);
        qDebug() << message;
        return message;
    });

    httpServer->route("/history", [this]( const QHttpServerRequest &request) {
        qDebug() << "history";
        return this->users->get_history();
    });
    qDebug () << "Route initialized";
    //! [Using afterRequest()]
    httpServer->afterRequest([](QHttpServerResponse &&resp) {
        resp.setHeader("Server", "Qt HTTP Server");
        return std::move(resp);
    });
    //! [Using afterRequest()]
    const auto port = httpServer->listen(QHostAddress::Any, _port);
    if (!port) {
        qWarning() << QCoreApplication::translate("QHttpServerExample",
                                                  "Server failed to listen on a port.");
        return -1;
    }

    qInfo().noquote()
        << QCoreApplication::translate("QHttpServerExample",
                                       "Running on http://127.0.0.1:%1/"
                                       "(Press CTRL+C to quit)").arg(port);
}
