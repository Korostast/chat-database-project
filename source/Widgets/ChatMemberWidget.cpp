#include "ChatMemberWidget.h"
#include "AvatarEditor.h"
#include "MainWindow.h"
#include "ui_chatmemberwidget.h"
#include "SqlInterface.h"

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

// Admin click to the remove member button
void ChatMemberWidget::removeMember() {
    // TODO database remove chat member
    // TODO remove yourself from the chat

    auto *sender = qobject_cast<QPushButton *>(QObject::sender());
    if (sender == ui->chat_members_remove_button) {
        QString content("%1 исключил %2");
        content = content.arg(MainWindow::currentUser->getUsername());
        auto *chatMemberWidget = qobject_cast<ChatMemberWidget *>(sender->parent());
        content = content.arg(chatMemberWidget->getName());

        sqlRemoveChatMember(chatMemberWidget->getId(), MainWindow::currentChat->getId());

        auto *chatDialog = qobject_cast<ChatDialog *>(sender->window());

        qInfo() << QString(
                "Admin remove user with messageId - %1, username - %2, role - %3 from chat with messageId - %4, username - %5")
                .arg(chatMemberWidget->getId()).arg(chatMemberWidget->getName()).arg(chatMemberWidget->getRole())
                .arg(MainWindow::currentChat->getId()).arg(MainWindow::currentChat->getName());

        chatDialog->removeMemberFromUi(chatMemberWidget);

        auto *mainWindow = qobject_cast<MainWindow *>(chatDialog->parent());
        // TODO database send message
        MessageInfo message(-1, content, nullptr, SYSTEM_MESSAGE, MainWindow::currentChat->getId(),
                            chatMemberWidget->getId());
        int messageId = sqlSendMessage(message);

        mainWindow->addMessage(MainWindow::currentChat->getId(), getId(), messageId, "", "", QImage(), content,
                               SYSTEM_MESSAGE);
    }
}

// Admin change chat member role
void ChatMemberWidget::changeMemberRole(int index) const {
    if (ui->chat_members_roles_combobox->hasFocus()) {// TODO database change chat member role
        sqlChangeRole(getId(), MainWindow::currentChat->getId(), index);

        qInfo() << QString("Admin change role of user with messageId - %1, username - %2. Old role - %3, new role - %4")
                .arg(getId()).arg(getName(), ui->chat_members_roles_combobox->currentText(),
                                  ui->chat_members_roles_combobox->itemText(index));

        ui->chat_members_roles_combobox->setCurrentIndex(index);
        ui->chat_members_role->setText(ui->chat_members_roles_combobox->currentText());
    }
}

void ChatMemberWidget::hideRoleLabelFromAdmin() const {
    ui->chat_members_role->hide();
}

void ChatMemberWidget::showRoleLabel() const {
    ui->chat_members_role->show();
}