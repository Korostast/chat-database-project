#include <QPainter>
#include <QPainterPath>
#include "MainWindow.h"
#include "ChatWidget.h"
#include "ui_chatform.h"
#include "Defines.h"

ChatWidget::ChatWidget(QWidget *parent)
        : QWidget(parent), ui(new Ui::ChatWidget), id(-1), countMembers(-1), role(VIEWER), group(false) {
    ui->setupUi(this);
}

ChatWidget::~ChatWidget() {
    delete ui;
}

void ChatWidget::mouseReleaseEvent(QMouseEvent *event) {
    qDebug() << "Pressed";
    MainWindow::currentChat = this;
    auto *mainWindow = qobject_cast<MainWindow *>(this->window()); // TODO it may also be done in choosing avatar dialog
    mainWindow->openChat();
}

void ChatWidget::setName(const QString &name) {
    ui->name->setText(name);
    this->name = name;
}

void ChatWidget::setAvatar(const QImage &image) {
    QPixmap resultPixmap = AvatarEditor::getCircularPixmap(image, CHAT_IMAGE_SIZE);

    ui->avatar->setPixmap(resultPixmap);
    this->avatar = image;
}

bool ChatWidget::isGroup() const {
    return group;
}

void ChatWidget::setGroup(bool group) {
    ChatWidget::group = group;
}

int ChatWidget::getId() const {
    return id;
}

void ChatWidget::setId(int id) {
    ChatWidget::id = id;
}

const QImage &ChatWidget::getAvatar() const {
    return avatar;
}

const QString &ChatWidget::getName() const {
    return name;
}

int ChatWidget::getCountMembers() const {
    return countMembers;
}

void ChatWidget::setCountMembers(int countMembers) {
    ChatWidget::countMembers = countMembers;
}

ROLE ChatWidget::getRole() const {
    return role;
}

void ChatWidget::setRole(ROLE role) {
    ChatWidget::role = role;
}

int ChatWidget::getFriendId() const {
    return friendId;
}

void ChatWidget::setFriendId(int friendId) {
    ChatWidget::friendId = friendId;
}
