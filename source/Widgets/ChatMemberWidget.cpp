#include "ChatMemberWidget.h"
#include "AvatarEditor.h"
#include "MainWindow.h"
#include "ui_chatmemberwidget.h"

ChatMemberWidget::ChatMemberWidget(QWidget *parent)
        : QWidget(parent), ui(new Ui::ChatMemberWidget), id(-1), role(VIEWER) {
    ui->setupUi(this);
    connect(ui->chat_members_remove_button, SIGNAL(released()), this, SLOT(removeMember()));
    connect(ui->chat_members_roles_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMemberRole(int)));
}

ChatMemberWidget::~ChatMemberWidget() {
    delete ui;
}

int ChatMemberWidget::getId() const {
    return id;
}

void ChatMemberWidget::setId(int id) {
    ChatMemberWidget::id = id;
}

const QImage &ChatMemberWidget::getAvatar() const {
    return avatar;
}

void ChatMemberWidget::setAvatar(const QImage &avatar) {
    ChatMemberWidget::avatar = avatar;
    ui->chat_members_avatar->setPixmap(AvatarEditor::getCircularPixmap(avatar, CHAT_MEMBER_IMAGE_SIZE));
}

const QString &ChatMemberWidget::getName() const {
    return name;
}

void ChatMemberWidget::setName(const QString &name) {
    ChatMemberWidget::name = name;
    ui->chat_members_name->setText(name);
}

ROLE ChatMemberWidget::getRole() const {
    return role;
}

void ChatMemberWidget::setRole(ROLE role) {
    ChatMemberWidget::role = role;
    ui->chat_members_roles_combobox->setCurrentIndex(role);
    ui->chat_members_role->setText(ui->chat_members_roles_combobox->currentText());
    if (MainWindow::currentChat->getRole() < ADMIN || id == MainWindow::currentUser->getId())
        ui->chat_members_roles_combobox->hide();
    if (MainWindow::currentChat->getRole() < MODERATOR || id == MainWindow::currentUser->getId())
        ui->chat_members_remove_button->hide();
}

void ChatMemberWidget::removeMember() {
    // TODO database
    // TODO remove yourself from the chat
    auto *sender = qobject_cast<QPushButton *>(QObject::sender());
    QString content("%1 исключил %2");
    content = content.arg(MainWindow::currentUser->getUsername());
    if (sender == ui->chat_members_remove_button) {
        auto *chatMemberWidget = qobject_cast<ChatMemberWidget *>(sender->parent());
        content = content.arg(chatMemberWidget->getName());
        auto *chatDialog = qobject_cast<ChatDialog *>(sender->window());
        chatDialog->removeMemberFromUi(chatMemberWidget);

        auto *mainWindow = qobject_cast<MainWindow *>(chatDialog->parent());
        mainWindow->addMessage(0, MainWindow::currentChat->getId(), "", "", QImage(), content, SYSTEM_MESSAGE);
    }
}

void ChatMemberWidget::changeMemberRole(int index) {
    // TODO database
    ui->chat_members_roles_combobox->setCurrentIndex(index);
    ui->chat_members_role->setText(ui->chat_members_roles_combobox->currentText());
}
