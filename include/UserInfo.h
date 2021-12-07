#ifndef CHATDATABASEPROJECT_USERINFO_H
#define CHATDATABASEPROJECT_USERINFO_H

#include <QObject>
#include <QImage>
#include <QDate>

class UserInfo {
private:
    int id;
    QString username;
    QImage avatar;
    QString status;
    QString email;
    QString phoneNumber;
    QString firstName;
    QString lastName;
    int friend_count;
    QDate created_date;

public:
    explicit UserInfo(int id = -1, QString username = nullptr, QImage avatar = QImage(":chatDefaultImage"),
                      QString status = nullptr,
                      QString email = nullptr, QString phoneNumber = nullptr, QString firstName = nullptr,
                      QString lastName = nullptr, int friend_count = -1, QDate created_date = QDate());

    int getId() const;

    void setId(int id);

    const QString &getUsername() const;

    void setUsername(const QString &username);

    const QString &getEmail() const;

    void setEmail(const QString &email);

    const QImage &getAvatar() const;

    void setAvatar(const QImage &avatar);

    const QString &getPhoneNumber() const;

    void setPhoneNumber(const QString &phoneNumber);

    const QString &getFirstName() const;

    void setFirstName(const QString &firstName);

    const QString &getLastName() const;

    void setLastName(const QString &lastName);

};


#endif //CHATDATABASEPROJECT_USERINFO_H