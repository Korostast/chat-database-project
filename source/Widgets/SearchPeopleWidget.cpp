#include "SearchPeopleWidget.h"
#include "ui_searchpeoplewidget.h"
#include "AvatarEditor.h"
#include "MainWindow.h"
#include "SqlInterface.h"
#include "ClickableLabel.h"

SearchPeopleWidget::SearchPeopleWidget(QWidget *parent) : ui(new Ui::SearchPeopleWidget) {
    ui->setupUi(this);
    auto *mainWindow = qobject_cast<MainWindow *>(parent);
    connect(ui->search_people_username, &ClickableLabel::released, this, [this, mainWindow]() {
        // TODO database searchMessages profile
        UserInfo user = sqlLoadProfile(getFriendID());
        mainWindow->showProfile(&user);
    });
    connect(ui->search_people_avatar, &ClickableLabel::released, this, [this, mainWindow]() {
        // TODO database searchMessages profile
        UserInfo user = sqlLoadProfile(getFriendID());
        mainWindow->showProfile(&user);
    });
}

SearchPeopleWidget::~SearchPeopleWidget() {
    delete ui;
}

void SearchPeopleWidget::setUsername(const QString &username) {
    AbstractFriendWidget::setUsername(username);
    ui->search_people_username->setText(username);
}

void SearchPeopleWidget::setAvatar(const QImage &avatar) {
    AbstractFriendWidget::setAvatar(avatar);
    QPixmap resultPixmap = AvatarEditor::getCircularPixmap(getAvatar(), SEARCH_PEOPLE_IMAGE_SIZE);
    ui->search_people_avatar->setPixmap(resultPixmap);
}