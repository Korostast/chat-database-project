#include "ui_usermessageform.h"
#include "UserMessageForm.h"
#include "MainWindow.h"

UserMessageForm::UserMessageForm(QWidget *parent) : QWidget(parent), ui(new Ui::UserMessageForm) {
    ui->setupUi(this);
}

UserMessageForm::~UserMessageForm() {
    delete ui;
}

int UserMessageForm::getId() const {
    return id;
}

void UserMessageForm::setId(int id) {
    UserMessageForm::id = id;
}

const QString &UserMessageForm::getAvatar() const {
    return avatar;
}

void UserMessageForm::setAvatar(const QString &pathToImage) {
    UserMessageForm::avatar = pathToImage;
    ui->message_avatar->setPixmap(MainWindow::getCircularPixmap(pathToImage, 40));
}

const QString &UserMessageForm::getName() const {
    return name;
}

void UserMessageForm::setName(const QString &name) {
    UserMessageForm::name = name;
    ui->message_username->setText(name);
}

const QString &UserMessageForm::getTime() const {
    return time;
}

void UserMessageForm::setTime(const QString &time) {
    UserMessageForm::time = time;
    ui->message_time->setText(time);
}

const QString &UserMessageForm::getContent() const {
    return content;
}

void UserMessageForm::setContent(const QString &content) {
    UserMessageForm::content = content;
    ui->message_content->setText(content);
}
