#include <QPainter>
#include <QPainterPath>
#include "MainWindow.h"
#include "ChatForm.h"
#include "ui_chatform.h"

ChatForm::ChatForm(QWidget *parent) : QWidget(parent), ui(new Ui::ChatForm), group(false) {
    ui->setupUi(this);
}

ChatForm::~ChatForm() {
    delete ui;
}

void ChatForm::mouseReleaseEvent(QMouseEvent *event) {
    qDebug() << "Pressed";
    MainWindow::currentChat = this;
    MainWindow::openChat();
}

void ChatForm::setName(const QString& name) {
    ui->name->setText(name);
    this->name = name;
}

void ChatForm::setAvatar(const QString &pathToImage) {
    QPixmap resultPixmap = MainWindow::getCircularPixmap(pathToImage, CHAT_IMAGE_SIZE);

    ui->avatar->setPixmap(resultPixmap);
    this->avatar = pathToImage;
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
