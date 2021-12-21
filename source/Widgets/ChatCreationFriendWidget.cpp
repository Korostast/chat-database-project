#include "ChatCreationFriendWidget.h"

#include <utility>
#include "ui_chatcreationfriendwidget.h"
#include "AvatarEditor.h"

ChatCreationFriendWidget::ChatCreationFriendWidget(QWidget *parent, int friendID, QString username, QImage avatar)
        : QWidget(parent),
          ui(new Ui::ChatCreationFriendWidget), friendID(friendID), username(std::move(username)), avatar(std::move(avatar)){

    ui->setupUi(this);
}

int ChatCreationFriendWidget::getFriendID() const {
    return friendID;
}

void ChatCreationFriendWidget::setFriendID(int friendId) {
    ChatCreationFriendWidget::friendID = friendId;
}

const QString &ChatCreationFriendWidget::getUsername() const {
    return username;
}

void ChatCreationFriendWidget::setUsername(const QString &username) {
    ChatCreationFriendWidget::username = username;
    ui->chat_creation_friend_name->setText(username);
}

const QImage &ChatCreationFriendWidget::getAvatar() const {
    return avatar;
}

void ChatCreationFriendWidget::setAvatar(const QImage &avatar) {
    ChatCreationFriendWidget::avatar = avatar.isNull() ? QImage(":user default avatar") : avatar;
    QPixmap resultPixmap = AvatarEditor::getCircularPixmap(ChatCreationFriendWidget::avatar, CHAT_CREATION_FRIEND_IMAGE_SIZE);
    ui->chat_creation_friend_avatar->setPixmap(resultPixmap);
}