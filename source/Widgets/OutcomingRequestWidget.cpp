#include "OutcomingRequestWidget.h"
#include "ui_outcomingrequestwidget.h"
#include "AvatarEditor.h"
#include "MainWindow.h"
#include "ClickableLabel.h"
#include "SqlInterface.h"

OutcomingRequestWidget::OutcomingRequestWidget(QWidget *parent) : ui(new Ui::OutcomingRequestWidget) {
    ui->setupUi(this);
    auto *mainWindow = qobject_cast<MainWindow *>(parent);
    connect(ui->outcoming_request_remove, SIGNAL(released()), this, SLOT(remove_request_button_released()));
    connect(ui->outcoming_request_username, &ClickableLabel::released, this, [this, mainWindow]() {
        // TODO database searchMessages profile
        UserInfo user = sqlLoadProfile(getFriendID());
        mainWindow->showProfile(&user);
    });
    connect(ui->outcoming_request_avatar, &ClickableLabel::released, this, [this, mainWindow]() {
        // TODO database searchMessages profile
        UserInfo user = sqlLoadProfile(getFriendID());
        mainWindow->showProfile(&user);
    });
}

OutcomingRequestWidget::~OutcomingRequestWidget() {
    delete ui;
}

void OutcomingRequestWidget::setUsername(const QString &username) {
    AbstractFriendWidget::setUsername(username);
    ui->outcoming_request_username->setText(username);
}

void OutcomingRequestWidget::setAvatar(const QImage &avatar) {
    QPixmap resultPixmap = AvatarEditor::getCircularPixmap(avatar, OUTCOMING_REQUESTS_IMAGE_SIZE);
    ui->outcoming_request_avatar->setPixmap(resultPixmap);
    AbstractFriendWidget::setAvatar(avatar);
}