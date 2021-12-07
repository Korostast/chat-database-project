#include "OutcomingRequestWidget.h"
#include "ui_outcomingrequestwidget.h"
#include "AvatarEditor.h"
#include "MainWindow.h"

OutcomingRequestWidget::OutcomingRequestWidget(QWidget *parent) : ui(new Ui::OutcomingRequestWidget) {
    ui->setupUi(this);
    connect(ui->outcoming_request_remove, SIGNAL(released()), this, SLOT(remove_request_button_released()));
}

OutcomingRequestWidget::~OutcomingRequestWidget() {
    delete ui;
}

void OutcomingRequestWidget::setUsername(const QString &username) {
    AbstractFriendWidget::setUsername(username);
    ui->outcoming_request_username->setText(username);
}

void OutcomingRequestWidget::setAvatar(const QImage &avatar) {
    QPixmap resultPixmap = AvatarEditor::getCircularPixmap(avatar, FRIEND_IMAGE_SIZE);
    ui->outcoming_request_avatar->setPixmap(resultPixmap);
    AbstractFriendWidget::setAvatar(avatar);
}