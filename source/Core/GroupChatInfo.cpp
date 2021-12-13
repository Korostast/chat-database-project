#include "MainWindow.h"
#include "ChatDialog.h"
#include "ui_chatdialog.h"
#include "Defines.h"
#include "ChatMemberWidget.h"
#include "../SqlInterface.h"
#include "ChatCreationFriendWidget.h"

// It will show ui of the chat info window only WORK FOR GROUP CHAT
void ChatDialog::setupCurrentChatUi(ChatWidget *chat) {
    ui->chat_dialog_stacked_widget->setCurrentIndex(0); // TODO define index
    ui->chat_dialog_avatar->setPixmap(
            AvatarEditor::getCircularPixmap(chat->getAvatar(), CHAT_DIALOG_AVATAR_IMAGE_SIZE));
    ui->chat_dialog_name->setText(chat->getName());
    ui->chat_dialog_count_members->setText(QString::number(chat->getCountMembers()));
    if (chat->getRole() == ADMIN) {
        ui->chat_dialog_name->setReadOnly(false);
        ui->chat_dialog_add_member_button->show();
        ui->chat_dialog_avatar->setCursor(Qt::PointingHandCursor);
    } else {
        ui->chat_dialog_name->setReadOnly(true);
        ui->chat_dialog_add_member_button->hide();
        ui->chat_dialog_avatar->setCursor(Qt::ArrowCursor);
    }

    // TODO database load members list
    QList<UserChatMember> members = sqlLoadChatMembers(MainWindow::currentChat->getId());

    // Cleaning
    ui->chat_dialog_members_list->clear();

    for (const auto &user: members) {
        auto *item = new QListWidgetItem;
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

        auto *widget = new ChatMemberWidget(this);
        widget->setId(user.id);
        widget->setName(user.username);
        widget->setAvatar(user.avatar);
        widget->setRole(user.role);
        if (MainWindow::currentChat->getRole() == ADMIN)
            widget->hideRoleLabelFromAdmin();
        else
            widget->showRoleLabel();

        item->setSizeHint(widget->sizeHint());
        ui->chat_dialog_members_list->addItem(item);
        ui->chat_dialog_members_list->setItemWidget(item, widget);
    }

}

// Admin change chat name
void ChatDialog::changeChatName() {
    qInfo() << QString("Name of chat with messageId - %1 changed from %2 to %3")
            .arg(MainWindow::currentChat->getId()).arg(MainWindow::currentChat->getName(),
                                                       ui->chat_dialog_name->text());
    setFocus();
    MainWindow::currentChat->setName(ui->chat_dialog_name->text());
    auto *mainWindow = qobject_cast<MainWindow *>(parentWidget());
    mainWindow->updateChat(MainWindow::currentChat->getId(), ui->chat_dialog_name->text(),
                           MainWindow::currentChat->getAvatar(), MainWindow::currentChat->getRole());

    // TODO database new chat name
    sqlChangeChatName(MainWindow::currentChat->getId(), ui->chat_dialog_name->text());

    // TODO system message
}

// Admin want to change avatar of the chat
void ChatDialog::openFileChooser() {
    if (MainWindow::currentChat == nullptr || MainWindow::currentChat->getRole() == ADMIN) {
        qInfo() << "File chooser opened";
        auto *mainWindow = qobject_cast<MainWindow *>(parentWidget());
        auto windowCentreX = mainWindow->x() + mainWindow->width() / 2 - CHOOSE_FILE_PAGE_WIDTH / 2;
        auto windowCentreY = mainWindow->y() + mainWindow->height() / 2 - CHOOSE_FILE_PAGE_HEIGHT / 2;
        mainWindow->avatarEditor->setGeometry(windowCentreX, windowCentreY,
                                              CHOOSE_FILE_PAGE_WIDTH, CHOOSE_FILE_PAGE_HEIGHT);
        mainWindow->avatarEditor->setFixedSize(CHOOSE_FILE_PAGE_WIDTH, CHOOSE_FILE_PAGE_HEIGHT);
        mainWindow->avatarEditor->setChooseFilePage();
        hide();
        mainWindow->avatarEditor->show();
    }
}

// Searching for widget of the chat member in the list
void ChatDialog::removeMemberFromUi(ChatMemberWidget *member) {
    int size = ui->chat_dialog_members_list->count();
    for (int i = 0; i < size; ++i) {
        auto *currentRow = ui->chat_dialog_members_list->item(i);
        auto *user = qobject_cast<ChatMemberWidget *>(ui->chat_dialog_members_list->itemWidget(currentRow));
        if (user->getId() == member->getId()) {
            delete ui->chat_dialog_members_list->takeItem(i);
            break;
        }
    }
}

// You leave the chat
void ChatDialog::leaveChat() {
    // TODO database leave chat
    sqlLeaveChat(MainWindow::currentUser->getId(), MainWindow::currentChat->getId());

    auto *mainWindow = qobject_cast<MainWindow *>(parent());
    QString content("%1 покинул беседу");
    content = content.arg(MainWindow::currentUser->getUsername());

    MessageInfo systemMessage(-1, content, nullptr, SYSTEM_MESSAGE, MainWindow::currentChat->getId(),
                              MainWindow::currentUser->getId());
    // TODO database send message
    int messageId = sqlSendMessage(systemMessage);
    mainWindow->addMessage(MainWindow::currentChat->getId(), MainWindow::currentUser->getId(), messageId, "", "",
                           QImage(), content,
                           SYSTEM_MESSAGE);
    close();

    // Remove chat from chats list and back to chat list
    mainWindow->removeChat(MainWindow::currentChat->getId());
}

void ChatDialog::loadAddMemberUi() {
    // TODO database load friends
    QList<UserInfo> friends = sqlLoadFriends(MainWindow::currentUser->getId());
    ui->chat_dialog_add_members_friend_list->clear();
    auto *mainWindow = qobject_cast<MainWindow *>(parentWidget());

    for (const auto &fr: friends)
        mainWindow->addToList<ChatCreationFriendWidget>(fr.getId(), fr.getUsername(), fr.getAvatar(),
                                                        ui->chat_dialog_add_members_friend_list);

    ui->chat_dialog_stacked_widget->setCurrentIndex(1); // TODO define index
}

void ChatDialog::addMembers() {
    // TODO database add members to chat
    // Check if at least one user is selected
    int countMembers = (int) ui->chat_dialog_add_members_friend_list->selectedItems().size();
    if (countMembers < 1)
        return;

    auto *mainWindow = qobject_cast<MainWindow *>(parentWidget());

    // Success
    std::vector<int> users;
    for (const auto &user: ui->chat_dialog_add_members_friend_list->selectedItems()) {
        const auto widget = qobject_cast<ChatCreationFriendWidget *>(
                ui->chat_dialog_add_members_friend_list->itemWidget(user));
        users.push_back(widget->getFriendId());

        QString content("%1 присоединился к беседе");
        content = content.arg(widget->getUsername());
        MessageInfo systemMessage(-1, content, nullptr, SYSTEM_MESSAGE, MainWindow::currentChat->getId(),
                                  MainWindow::currentUser->getId());
        // TODO database send message
        int messageId = sqlSendMessage(systemMessage);
        mainWindow->addMessage(MainWindow::currentChat->getId(), MainWindow::currentUser->getId(), messageId, "", "",
                               QImage(), content, SYSTEM_MESSAGE);
    }

    sqlAddMembers(MainWindow::currentChat->getId(), users);

    // Debug information
    for (auto userId: users) {
        printf("%d ", userId);
    }

    // Close chat dialog
    close();
}
