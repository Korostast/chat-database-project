#include "ChatMemberWidget.h"
#include "AvatarEditor.h"
#include "MainWindow.h"
#include "ui_chatmemberwidget.h"
#include "SqlInterface.h"
#include "ClickableLabel.h"
#include "ui_chatdialog.h"
#include "ui_mainWindow.h"

ChatMemberWidget::ChatMemberWidget(QWidget *parent)
        : QWidget(parent), ui(new Ui::ChatMemberWidget), id(-1), role(VIEWER) {
    ui->setupUi(this);
    auto *mainWindow = qobject_cast<MainWindow *>(parent->parent());
    connect(ui->chat_members_remove_button, SIGNAL(released()), this, SLOT(removeMember()));
    connect(ui->chat_members_roles_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMemberRole(int)));
    connect(ui->chat_members_avatar, &ClickableLabel::released, this, [this, mainWindow, parent]() {
        // TODO database searchMessages profile
        UserInfo user = sqlLoadProfile(getID());
        mainWindow->showProfile(&user);
        parent->close();
    });
    connect(ui->chat_members_name, &ClickableLabel::released, this, [this, mainWindow, parent]() {
        // TODO database searchMessages profile
        UserInfo user = sqlLoadProfile(getID());
        mainWindow->showProfile(&user);
        parent->close();
    });
}

ChatMemberWidget::~ChatMemberWidget() {
    delete ui;
}

int ChatMemberWidget::getID() const {
    return id;
}

void ChatMemberWidget::setID(int id) {
    ChatMemberWidget::id = id;
}

const QImage &ChatMemberWidget::getAvatar() const {
    return avatar;
}

void ChatMemberWidget::setAvatar(const QImage &avatar) {
    ChatMemberWidget::avatar = avatar.isNull() ? QImage(":user default avatar") : avatar;
    ui->chat_members_avatar->setPixmap(
            AvatarEditor::getCircularPixmap(ChatMemberWidget::avatar, CHAT_MEMBER_IMAGE_SIZE));
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
    bool oldState = ui->chat_members_roles_combobox->blockSignals(true);
    ui->chat_members_roles_combobox->setCurrentIndex(role);
    ui->chat_members_roles_combobox->blockSignals(oldState);

    ui->chat_members_role->setText(ui->chat_members_roles_combobox->currentText());
    if (MainWindow::currentChat->getRole() < ADMIN || id == MainWindow::currentUser->getID())
        ui->chat_members_roles_combobox->hide();
    if (MainWindow::currentChat->getRole() < MODERATOR || id == MainWindow::currentUser->getID()
        || getRole() >= MainWindow::currentChat->getRole())
        ui->chat_members_remove_button->hide();
}

// Admin click to the remove member button
void ChatMemberWidget::removeMember() {
    // TODO database remove chat member

    auto *sender = qobject_cast<QPushButton *>(QObject::sender());
    if (sender == ui->chat_members_remove_button) {
        QString content("%1 исключил %2");
        content = content.arg(MainWindow::currentUser->getUsername());
        auto *chatMemberWidget = qobject_cast<ChatMemberWidget *>(sender->parent());
        content = content.arg(chatMemberWidget->getName());

        sqlRemoveChatMember(chatMemberWidget->getID(), MainWindow::currentChat->getID());

        qInfo() << QString(
                "Admin remove user with userID - %1, username - %2, role - %3 from chat with chatID - %4, username - %5")
                .arg(chatMemberWidget->getID())
                .arg(chatMemberWidget->getName())
                .arg(chatMemberWidget->getRole())
                .arg(MainWindow::currentChat->getID())
                .arg(MainWindow::currentChat->getName()
                );

        auto *chatDialog = qobject_cast<ChatDialog *>(sender->window());
        chatDialog->removeMemberFromUI(chatMemberWidget);
        chatDialog->ui->chat_dialog_count_members->setText(
                QString::number(chatDialog->ui->chat_dialog_members_list->count()) + " участников");

        auto *mainWindow = qobject_cast<MainWindow *>(chatDialog->parent());
        mainWindow->ui->chat_online_or_members_label->setText(
                QString::number(chatDialog->ui->chat_dialog_members_list->count()) + " участников");


        // TODO database send message
        MessageInfo message(-1, content, nullptr, SYSTEM_MESSAGE, MainWindow::currentChat->getID(),
                            chatMemberWidget->getID());
        int messageId = sqlSendMessage(message);

        mainWindow->addMessage(MainWindow::currentChat->getID(), getID(), messageId, "", "", QImage(), content,
                               SYSTEM_MESSAGE);
    }
}

// Admin change chat member role
void ChatMemberWidget::changeMemberRole(int index) const {
    sqlChangeRole(MainWindow::currentChat->getID(), getID(), index);

    qInfo() << QString("Admin change role of user with messageID - %1, username - %2. Old role - %3, new role - %4")
            .arg(getID()).arg(getName(), ui->chat_members_roles_combobox->currentText(),
                              ui->chat_members_roles_combobox->itemText(index));

    ui->chat_members_roles_combobox->setCurrentIndex(index);
    ui->chat_members_role->setText(ui->chat_members_roles_combobox->currentText());
}

void ChatMemberWidget::hideRoleLabelFromAdmin() const {
    ui->chat_members_role->hide();
}

void ChatMemberWidget::showRoleLabel() const {
    ui->chat_members_role->show();
}