#include <QPainter>
#include <QPainterPath>
#include "MainWindow.h"
#include "ChatForm.h"
#include "ui_chatform.h"

ChatForm::ChatForm(QWidget *parent) : QWidget(parent), ui(new Ui::ChatForm), id(-1), countMembers(-1), role(VIEWER),
                                      group(false) {
    ui->setupUi(this);
}

ChatForm::~ChatForm() {
    delete ui;
}

void ChatForm::mouseReleaseEvent(QMouseEvent *event) {
    qDebug() << "Pressed";
    MainWindow::currentChat = this;
    auto *mainWindow = qobject_cast<MainWindow *>(this->window()); // TODO it may also be done in choosing avatar dialog
    mainWindow->openChat();
}

void ChatForm::setName(const QString &name) {
    ui->name->setText(name);
    this->name = name;
}

void ChatForm::setAvatar(const QImage &image) {
    QPixmap resultPixmap = MainWindow::getCircularPixmap(image, CHAT_IMAGE_SIZE);

    ui->avatar->setPixmap(resultPixmap);
    this->avatar = image;
}

bool ChatForm::isGroup() const {
    return group;
}

void ChatForm::setGroup(bool group) {
    ChatForm::group = group;
}

int ChatForm::getId() const {
    return id;
}

void ChatForm::setId(int id) {
    ChatForm::id = id;
}

const QImage & ChatForm::getAvatar() const {
    return avatar;
}

const QString &ChatForm::getName() const {
    return name;
}

int ChatForm::getCountMembers() const {
    return countMembers;
}

void ChatForm::setCountMembers(int countMembers) {
    ChatForm::countMembers = countMembers;
}

ROLE ChatForm::getRole() const {
    return role;
}

void ChatForm::setRole(ROLE role) {
    ChatForm::role = role;
}
