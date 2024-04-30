#ifndef USERS_H
#define USERS_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

struct date
{
    QString date;
    QString hour;
};

struct date typedef myDate;

myDate get_time();
class User
{
private:

public:
    QString id;
    QVector<myDate> date;
    User(QString _id);
};

class Admin : public User
{
private:

    QString password;
public:
    bool auth(QString _id, QString pass);
    Admin(QString _id, QString _pass);
};
class Users : public QObject
{
    Q_OBJECT
public:
    explicit Users(QObject *parent = nullptr);
    QString validate_user(QString _id);
    void addUser(QString id);
    void addAdmin(QString id, QString pass);
    QString validate_admin(QString _id, QString pass);
    QVector <User> users;
    QVector <Admin> admins;
    QJsonObject get_history();
signals:
    void newuser(const QString &message);
};

#endif // USERS_H
