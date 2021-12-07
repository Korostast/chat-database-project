#include "MainWindow.h"
#include "AvatarEditor.h"
#include "FriendWidget.h"
#include "ui_friendwidget.h"

FriendWidget::FriendWidget(QWidget *parent) : ui(new Ui::FriendWidget) {
    setFriendId(-1);
    ui->setupUi(this);
    connect(ui->friend_remove, SIGNAL(released()), this, SLOT(friend_remove_button_released()));
}

FriendWidget::~FriendWidget() {
    delete ui;
}

void FriendWidget::setUsername(const QString &username) {
    AbstractFriendWidget::setUsername(username);
    ui->friend_username->setText(username);
}

void FriendWidget::setAvatar(const QImage &avatar) {
    QPixmap resultPixmap = AvatarEditor::getCircularPixmap(avatar, FRIEND_IMAGE_SIZE);
    ui->friend_avatar->setPixmap(resultPixmap);
    AbstractFriendWidget::setAvatar(avatar);
}