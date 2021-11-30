#ifndef CHATDATABASEPROJECT_USERINFO_H
#define CHATDATABASEPROJECT_USERINFO_H

#include <QObject>

class UserInfo {
private:
    int id;
    QString username;
    QString email;
    QString avatar;
    QString phoneNumber;
    QString firstName;
    QString lastName;

public:
    explicit UserInfo(int id = -1, QString username = nullptr, QString email = nullptr, QString avatar = nullptr,
             QString phoneNumber = nullptr, QString firstName = nullptr, QString lastName = nullptr);

    int getId() const;

    void setId(int id);

    const QString &getUsername() const;

    void setUsername(const QString &username);

    const QString &getEmail() const;

    void setEmail(const QString &email);

    const QString &getAvatar() const;

    void setAvatar(const QString &avatar);

    const QString &getPhoneNumber() const;

    void setPhoneNumber(const QString &phoneNumber);

    const QString &getFirstName() const;

    void setFirstName(const QString &firstName);

    const QString &getLastName() const;

    void setLastName(const QString &lastName);

};


#endif //CHATDATABASEPROJECT_USERINFO_H
