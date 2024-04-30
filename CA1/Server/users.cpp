#include "users.h"
User::User(QString _id)
{
    id = _id;
}
Users::Users(QObject *parent)
    : QObject{parent}
{}

Admin::Admin(QString _id, QString _pass)
    : User{_id}
{
    password = _pass;
}
QJsonObject Users::get_history()
{
    QVariantList list;
    for (int i = 0; i < users.size(); i++)
    {
        User user = users[i];
        for (int j = 0; j < user.date.size(); j++)
        {
            QJsonObject obj1;
            obj1["username"] = user.id;
            obj1["date"] = user.date[j].date;
            obj1["time"] = user.date[j].hour;
            list.append(obj1);
        }
    }
    QJsonObject data;
    data["history"] = QJsonArray::fromVariantList(list);
    // QJsonArray data =
    return data;
}
myDate get_time()
{
    QDateTime date = QDateTime::currentDateTime();
    myDate dateUser;
    dateUser.date = date.toString("dd.MM.yyyy");
    dateUser.hour = date.toString("hh:mm:ss");
    qDebug() << dateUser.date;
    qDebug() << dateUser.hour;
    // QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
    return dateUser;
}
QString Users::validate_user(QString _id)
{
    QString message;
    for (int var = 0; var < users.size(); ++var) {
        if( _id == users[var].id)
        {
            message = _id;
            myDate dateUser = get_time();
            users[var].date.append(dateUser);
            QJsonObject message2;
            QJsonObject user;
            user["id"] = _id;
            user["date"] = dateUser.date;
            user["time"] = dateUser.hour;
            message2["user"] = user;
            QJsonDocument jsonDoc(message2);
            QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);
            qDebug() << jsonString;
            emit newuser(jsonString);
            return jsonString;
        }
    }
    message = "Access Denied";
    return message;
}

bool Admin::auth(QString _id, QString pass)
{
    return id == _id && password == pass;
}

QString Users::validate_admin(QString _id, QString _pass)
{
    QString message;
    for (int var = 0; var < admins.size(); var++) {
        if(admins[var].auth(_id, _pass))
        {
            message = "accept";
            myDate dateUser = get_time();
            admins[var].date.append(dateUser);
            return message;
        }
    }
    message = "reject";
    return message;
}

void Users::addUser(QString id)
{
    users.append(User(id));
}

void Users::addAdmin(QString id, QString pass)
{
    admins.append(Admin(id, pass));
}
