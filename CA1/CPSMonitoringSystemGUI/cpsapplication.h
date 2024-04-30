#ifndef CPSAPPLICATION_H
#define CPSAPPLICATION_H

#include <QObject>
#include <QApplication>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>
#include "cpsmainwindow.h"
#include "cpshistorywindow.h"
#include "cpswindowsapitools.h"
#include <QJsonDocument>
#include "cpswebsocket.h"

namespace CPS {

class Application : public QObject
{
    Q_OBJECT

public:

    explicit Application(QObject *parent = nullptr);
    ~Application();

    void show();

Q_SIGNALS:

private slots:
    void get_history(const QJsonArray &his);
    void set_port(QString &t);
private Q_SLOTS:
    void showHistoryWindow();
    // void get_history(QNetworkReply *reply);

private: // methods

private: // members
    MainWindow    *_window;
    HistoryWindow *_history;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QJsonDocument doc;
    QString port;
    //TODO:
    WebSocketClient *_WebSocketClient;
};

} // end of CPS

#endif // CPSAPPLICATION_H
