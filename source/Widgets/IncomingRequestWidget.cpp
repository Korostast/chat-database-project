#include "IncomingRequestWidget.h"
#include "ui_incomingrequestwidget.h"
#include "AvatarEditor.h"
#include "MainWindow.h"

IncomingRequestWidget::IncomingRequestWidget(QWidget *parent) : ui(new Ui::IncomingRequestWidget) {
    ui->setupUi(this);
    connect(ui->incoming_request_accept, SIGNAL(released()), this, SLOT(accept_request_button_released()));
    connect(ui->incoming_request_decline, SIGNAL(released()), this, SLOT(decline_request_button_released()));
}

IncomingRequestWidget::~IncomingRequestWidget() {
    delete ui;
}

void IncomingRequestWidget::setUsername(const QString &username) {
    AbstractFriendWidget::setUsername(username);
    ui->incoming_request_username->setText(username);
}

void IncomingRequestWidget::setAvatar(const QImage &avatar) {
    QPixmap resultPixmap = AvatarEditor::getCircularPixmap(avatar, INCOMING_REQUESTS_IMAGE_SIZE);
    ui->incoming_request_avatar->setPixmap(resultPixmap);
    AbstractFriendWidget::setAvatar(avatar);
}