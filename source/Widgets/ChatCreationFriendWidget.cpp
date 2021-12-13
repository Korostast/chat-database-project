#include "ChatCreationFriendWidget.h"

#include <utility>
#include "ui_chatcreationfriendwidget.h"
#include "AvatarEditor.h"

ChatCreationFriendWidget::ChatCreationFriendWidget(QWidget *parent, int friendId, QString username, QImage avatar)
        : QWidget(parent),
          ui(new Ui::ChatCreationFriendWidget), friendId(friendId), username(std::move(username)), avatar(std::move(avatar)){

    ui->setupUi(this);
}

int ChatCreationFriendWidget::getFriendId() const {
    return friendId;
}

void ChatCreationFriendWidget::setFriendId(int friendId) {
    ChatCreationFriendWidget::friendId = friendId;
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
    QPixmap resultPixmap = AvatarEditor::getCircularPixmap(avatar, CHAT_CREATION_FRIEND_IMAGE_SIZE);
    ui->chat_creation_friend_avatar->setPixmap(resultPixmap);
    ChatCreationFriendWidget::avatar = avatar;
}
