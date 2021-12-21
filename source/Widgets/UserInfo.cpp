#include "UserInfo.h"

#include <utility>

int UserInfo::getID() const {
    return id;
}

void UserInfo::setID(int id) {
    UserInfo::id = id;
}

const QString &UserInfo::getUsername() const {
    return username;
}

void UserInfo::setUsername(const QString &username) {
    UserInfo::username = username;
}

const QString &UserInfo::getEmail() const {
    return email;
}

void UserInfo::setEmail(const QString &email) {
    UserInfo::email = email;
}

const QImage &UserInfo::getAvatar() const {
    return avatar;
}

void UserInfo::setAvatar(const QImage &avatar) {
    UserInfo::avatar = avatar.isNull() ? QImage(":user default avatar") : avatar;
}

const QString &UserInfo::getPhoneNumber() const {
    return phoneNumber;
}

void UserInfo::setPhoneNumber(const QString &phoneNumber) {
    UserInfo::phoneNumber = phoneNumber;
}

const QString &UserInfo::getFirstName() const {
    return firstName;
}

void UserInfo::setFirstName(const QString &firstName) {
    UserInfo::firstName = firstName;
}

const QString &UserInfo::getLastName() const {
    return lastName;
}

void UserInfo::setLastName(const QString &lastName) {
    UserInfo::lastName = lastName;
}

UserInfo::UserInfo(int id, QString username, QImage avatar, QString status, QString email, QString phoneNumber,
                   QString firstName, QString lastName)
        : id(id), username(std::move(username)), status(std::move(status)), email(std::move(email)),
          avatar(std::move(avatar)), phoneNumber(std::move(phoneNumber)), firstName(std::move(firstName)),
          lastName(std::move(lastName)) {}

const QString &UserInfo::getStatus() const {
    return status;
}

void UserInfo::setStatus(const QString &status) {
    UserInfo::status = status;
}

// Just why not
UserInfo &UserInfo::operator=(const UserInfo &user) {
    if (this == &user)
        return *this;

    id = user.id;
    username = user.username;
    avatar = user.avatar;
    status = user.status;
    email = user.email;
    phoneNumber = user.phoneNumber;
    firstName = user.firstName;
    lastName = user.lastName;

    return *this;
}