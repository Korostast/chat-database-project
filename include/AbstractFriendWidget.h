#ifndef CHATDATABASEPROJECT_ABSTRACTFRIENDWIDGET_H
#define CHATDATABASEPROJECT_ABSTRACTFRIENDWIDGET_H

#include <QWidget>
#include "Defines.h"

class AbstractFriendWidget : public QWidget {
Q_OBJECT

private:
    int friendId;

    QString username;

    QImage avatar;

public:
    int getFriendId() const {
        return friendId;
    }

    void setFriendId(int friendId) {
        AbstractFriendWidget::friendId = friendId;
    }

    const QString &getUsername() const {
        return username;
    }

    virtual void setUsername(const QString &username) {
        AbstractFriendWidget::username = username;
    }

    const QImage &getAvatar() const {
        return avatar;
    }

    virtual void setAvatar(const QImage &avatar) {
        AbstractFriendWidget::avatar = avatar;
    }

protected:
    explicit AbstractFriendWidget(QWidget *parent = nullptr) : QWidget(parent), friendId(-1) {};

};

#endif //CHATDATABASEPROJECT_ABSTRACTFRIENDWIDGET_H
