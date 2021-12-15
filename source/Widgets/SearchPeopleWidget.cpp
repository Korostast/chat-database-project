#include "SearchPeopleWidget.h"
#include "ui_searchpeoplewidget.h"
#include "AvatarEditor.h"
#include "MainWindow.h"
#include "SqlInterface.h"

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
    QPixmap resultPixmap = AvatarEditor::getCircularPixmap(avatar, SEARCH_PEOPLE_IMAGE_SIZE);
    ui->search_people_avatar->setPixmap(resultPixmap);
    AbstractFriendWidget::setAvatar(avatar);
}

void SearchPeopleWidget::add_friend_button_released() {
    // TODO database send friend request
    sqlSendFriendRequest(MainWindow::currentUser->getId(), getFriendId());

    // TODO change ui of search people widget
    ui->search_people_add->hide();
}