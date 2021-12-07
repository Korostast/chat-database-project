#include <QtCore/QtCore>
#include "sqlFunctions.h"

UserInfo sqlAuthenticate(QString &handle, QString &password) {
    QSqlQuery q;
    q.prepare("call existsUser(:handle)");
    q.bindValue(":handle", handle);
    q.exec();
    if (q.size() == 0)
        throw "User with such username/email does not exist";

    q.next();
    int id = q.value(0).toInt();

    q.prepare("call authenticateUser(:id,:password)");
    q.bindValue(":id", id);
    q.bindValue(":password", password);
    q.exec();
    if (q.size() == 0)
        throw "Incorrect password";

    q.next();
    QString username = q.value("username").toString();
    QString status = q.value("profile_status").toString();
    int friend_count = q.value("friend_count").toInt();
    QString first_name = q.value("first_name").toString();
    QString last_name = q.value("last_name").toString();
    QString profile_status = q.value("profile_status").toString();
    QImage avatar;
    avatar.loadFromData(q.value("thumbnail").toByteArray(), "PNG");
    QString email = q.value("email").toString();
    QString phone_number = q.value("phone_number").toString();
    QDate created_date = q.value("created_date").toDate();
    return UserInfo(id, username, avatar, profile_status, email, phone_number,
                    first_name, last_name, friend_count, created_date);
}