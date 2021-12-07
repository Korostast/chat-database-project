#include "sqlFunctions.h"

#include <utility>

QSqlException::QSqlException(std::string message) : msg(std::move(message)) {}

const char *QSqlException::what() const noexcept {
    return msg.c_str();
}

UserInfo sqlAuthenticate(QString &handle, QString &password_hash) {
    QSqlQuery q;

    q.exec("call existsUser('" + handle + "')");
    if (!q.next())
        throw QSqlException("User with such username/email does not exist");

    int id = q.value(0).toInt();

    q.exec("call authenticateUser(" + QString::number(id) + ", '" + password_hash + "')");
    if (!q.next())
        throw QSqlException("Incorrect password_hash");

    QString username = q.value("username").toString();
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