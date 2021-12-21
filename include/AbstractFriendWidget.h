#ifndef CHATDATABASEPROJECT_ABSTRACTFRIENDWIDGET_H
#define CHATDATABASEPROJECT_ABSTRACTFRIENDWIDGET_H

#include <QWidget>
#include "Defines.h"

class AbstractFriendWidget : public QWidget {
Q_OBJECT

private:
    int friendID;

    QString username;

    QImage avatar;

public:
    int getFriendID() const {
        return friendID;
    }

    void setFriendID(int friendID) {
        AbstractFriendWidget::friendID = friendID;
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
        AbstractFriendWidget::avatar = avatar.isNull() ? QImage(":user default avatar") : avatar;
    }

protected:
    explicit AbstractFriendWidget(QWidget *parent = nullptr) : QWidget(parent), friendID(-1) {};

};

#endif //CHATDATABASEPROJECT_ABSTRACTFRIENDWIDGET_H