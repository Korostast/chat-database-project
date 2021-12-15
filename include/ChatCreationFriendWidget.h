#ifndef CHATDATABASEPROJECT_CHATCREATIONFRIENDWIDGET_H
#define CHATDATABASEPROJECT_CHATCREATIONFRIENDWIDGET_H

#include <QWidget>
#include "Defines.h"

namespace Ui {
    class ChatCreationFriendWidget;
}
QT_END_NAMESPACE

class ChatCreationFriendWidget : public QWidget {
Q_OBJECT

private:
    Ui::ChatCreationFriendWidget *ui;

    int friendID;

    QString username;

    QImage avatar;

public:
    int getFriendID() const;

    void setFriendID(int friendId);

    const QString &getUsername() const;

    void setUsername(const QString &username);

    const QImage &getAvatar() const;

    void setAvatar(const QImage &avatar);

public:
    explicit ChatCreationFriendWidget(QWidget *parent = nullptr, int friendID = -1, QString username = "",
                                      QImage avatar = QImage(":chatDefaultImage"));

};

#endif //CHATDATABASEPROJECT_CHATCREATIONFRIENDWIDGET_H