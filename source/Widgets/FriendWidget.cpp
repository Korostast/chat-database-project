#include "MainWindow.h"
#include "AvatarEditor.h"
#include "FriendWidget.h"
#include "ui_friendwidget.h"
#include "ClickableLabel.h"
#include "SqlInterface.h"

FriendWidget::FriendWidget(QWidget *parent) : ui(new Ui::FriendWidget) {
    setFriendID(-1);
    ui->setupUi(this);
    auto *mainWindow = qobject_cast<MainWindow *>(parent);
    connect(ui->friend_remove, SIGNAL(released()), this, SLOT(friend_remove_button_released()));
    connect(ui->friend_username, &ClickableLabel::released, this, [this, mainWindow]() {
        // TODO database searchMessages profile
        UserInfo user = sqlLoadProfile(getFriendID());
        mainWindow->showProfile(&user);
    });
    connect(ui->friend_avatar, &ClickableLabel::released, this, [this, mainWindow]() {
        // TODO database searchMessages profile
        UserInfo user = sqlLoadProfile(getFriendID());
        mainWindow->showProfile(&user);
    });
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