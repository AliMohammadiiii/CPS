#include "cpsapplication.h"

namespace CPS {


void Application::get_history(const QJsonArray &his) {
    _history->show(his);
    qDebug() << his;
}

void Application::set_port(QString &t)
{
    this->port = t;
}
Application::Application(QObject *parent)
    : QObject{parent},
    _window(new MainWindow),
    _history(new HistoryWindow)
{
    _WebSocketClient = new WebSocketClient();
    setWindowsThemeToDark<MainWindow>(*_window);
    QObject::connect(_window, &MainWindow::historyuBtnClicked, this, &Application::showHistoryWindow);
    QObject::connect(_WebSocketClient, &WebSocketClient::newuser, _window, &MainWindow::showUserDetails);
    QObject::connect(_window, &MainWindow::connectBtnClicked, _WebSocketClient, &WebSocketClient::connectToServer);
    QObject::connect(_WebSocketClient, &WebSocketClient::connectionStateChanged, _window, &MainWindow::changeRightPanelEnabled);
    QObject::connect(_WebSocketClient, &WebSocketClient::history, this, &Application::get_history);
}

Application::~Application()
{
    delete this->_window;
    delete this->_history;
    delete this->_WebSocketClient;
}

void Application::show()
{
    this->_window->show();
}

void Application::showHistoryWindow()
{
    setWindowsThemeToDark<HistoryWindow>(*_history);
    _WebSocketClient->sendMessage(QStringLiteral("history"));

}

} // end of CPS
