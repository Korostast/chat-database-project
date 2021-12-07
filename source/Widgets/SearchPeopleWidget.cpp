#include "SearchPeopleWidget.h"
#include "ui_searchpeoplewidget.h"
#include "AvatarEditor.h"
#include "MainWindow.h"

SearchPeopleWidget::SearchPeopleWidget(QWidget *parent) : ui(new Ui::SearchPeopleWidget) {
    ui->setupUi(this);
    connect(ui->search_people_add, SIGNAL(released()), this, SLOT(add_friend_button_released()));
}

SearchPeopleWidget::~SearchPeopleWidget() {
    delete ui;
}

void SearchPeopleWidget::setUsername(const QString &username) {
    AbstractFriendWidget::setUsername(username);
    ui->search_people_username->setText(username);
}

void SearchPeopleWidget::setAvatar(const QImage &avatar) {
    QPixmap resultPixmap = AvatarEditor::getCircularPixmap(avatar, FRIEND_IMAGE_SIZE);
    ui->search_people_avatar->setPixmap(resultPixmap);
    AbstractFriendWidget::setAvatar(avatar);
}

void SearchPeopleWidget::add_friend_button_released() {
    // TODO database send friend request

}
