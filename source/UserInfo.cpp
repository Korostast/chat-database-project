#include "UserInfo.h"

#include <utility>

int UserInfo::getId() const {
    return id;
}

void UserInfo::setId(int id) {
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

const QString &UserInfo::getAvatar() const {
    return avatar;
}

void UserInfo::setAvatar(const QString &avatar) {
    UserInfo::avatar = avatar;
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

UserInfo::UserInfo(int id, QString username, QString email, QString avatar,
                   QString phoneNumber, QString firstName, QString lastName)
                   : id(id), username(std::move(username)), email(std::move(email)), avatar(std::move(avatar)),
                   phoneNumber(std::move(phoneNumber)), firstName(std::move(firstName)), lastName(std::move(lastName))
                   {}
