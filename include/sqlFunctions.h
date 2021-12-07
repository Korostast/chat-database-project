#ifndef CHATDATABASEPROJECT_SQLFUNCTIONS_H
#define CHATDATABASEPROJECT_SQLFUNCTIONS_H

#include "UserInfo.h"

#include <QSqlDatabase>
#include <QSqlQuery>

class QSqlException : std::exception {
    std::string msg;
public:
    QSqlException(std::string message);

    const char *what() const noexcept override;
};

UserInfo sqlAuthenticate(QString &handle, QString &password_hash);


#endif //CHATDATABASEPROJECT_SQLFUNCTIONS_H