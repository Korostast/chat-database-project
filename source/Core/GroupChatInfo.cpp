#include "MainWindow.h"
#include "ChatDialog.h"
#include "ui_chatdialog.h"
#include "ui_mainwindow.h"
#include "Defines.h"
#include "ChatMemberWidget.h"
#include "SqlInterface.h"
#include "ChatCreationFriendWidget.h"

// It will show ui of the chat info window only WORK FOR GROUP CHAT
void ChatDialog::setupCurrentChatUI(ChatWidget *chat) {
    ui->chat_dialog_stacked_widget->setCurrentIndex(0); // TODO define index
    ui->chat_dialog_avatar->setPixmap(
            AvatarEditor::getCircularPixmap(chat->getAvatar(), CHAT_DIALOG_AVATAR_IMAGE_SIZE));
    ui->chat_dialog_name->setText(chat->getName());
    if (chat->getRole() == ADMIN) {
        ui->chat_dialog_name->setReadOnly(false);
        ui->chat_dialog_add_member_button->show();
        ui->chat_dialog_avatar->setCursor(Qt::PointingHandCursor);
    } else {
        ui->chat_dialog_name->setReadOnly(true);
        ui->chat_dialog_add_member_button->hide();
        ui->chat_dialog_avatar->setCursor(Qt::ArrowCursor);
    }

    // TODO database searchMessages members list
    QList<UserChatMember> members = sqlLoadChatMembers(MainWindow::currentChat->getID());
    ui->chat_dialog_count_members->setText(QString::number(members.count()) + " участников");

    // Cleaning
    ui->chat_dialog_members_list->clear();

    for (const auto &user: members) {
        auto *item = new QListWidgetItem;
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

        auto *widget = new ChatMemberWidget(this);
        widget->setID(user.id);
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
    qInfo() << QString("Name of chat with messageID - %1 changed from %2 to %3")
            .arg(MainWindow::currentChat->getID()).arg(MainWindow::currentChat->getName(),
                                                       ui->chat_dialog_name->text());
    setFocus();
    MainWindow::currentChat->setName(ui->chat_dialog_name->text());
    auto *mainWindow = qobject_cast<MainWindow *>(parentWidget());
    mainWindow->updateChat(MainWindow::currentChat->getID(), ui->chat_dialog_name->text(),
                           MainWindow::currentChat->getAvatar(), MainWindow::currentChat->getRole());

    // TODO database new chat name
    sqlUpdateChatName(MainWindow::currentChat->getID(), ui->chat_dialog_name->text());

    QString content = QString("%1 изменил название беседы на %2").arg(MainWindow::currentUser->getUsername(),
                                                                      ui->chat_dialog_name->text());
    MessageInfo systemMessage(-1, content, nullptr, SYSTEM_MESSAGE, MainWindow::currentChat->getID(),
                              MainWindow::currentUser->getID());
    // TODO database send message
    int messageId = sqlSendMessage(systemMessage);
    mainWindow->addMessage(MainWindow::currentChat->getID(), MainWindow::currentUser->getID(), messageId, "", "",
                           QImage(), content, SYSTEM_MESSAGE);
}

// Admin want to change avatar of the chat
void ChatDialog::openFileChooser() {
    STATE state = MainWindow::currentState;

    if (state == MY_PROFILE || state == CHAT_CREATION
        || (MainWindow::currentChat != nullptr && MainWindow::currentChat->getRole() == ADMIN)) {
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
void ChatDialog::removeMemberFromUI(ChatMemberWidget *member) {
    int size = ui->chat_dialog_members_list->count();
    for (int i = 0; i < size; ++i) {
        auto *currentRow = ui->chat_dialog_members_list->item(i);
        auto *user = qobject_cast<ChatMemberWidget *>(ui->chat_dialog_members_list->itemWidget(currentRow));
        if (user->getID() == member->getID()) {
            delete ui->chat_dialog_members_list->takeItem(i);
            break;
        }
    }
}

// You leave the chat
void ChatDialog::leaveChat() {
    // TODO database leave chat
    sqlLeaveChat(MainWindow::currentUser->getID(), MainWindow::currentChat->getID());

    auto *mainWindow = qobject_cast<MainWindow *>(parent());
    QString content("%1 покинул беседу");
    content = content.arg(MainWindow::currentUser->getUsername());

    MessageInfo systemMessage(-1, content, nullptr, SYSTEM_MESSAGE, MainWindow::currentChat->getID(),
                              MainWindow::currentUser->getID());
    // TODO database send message
    int messageId = sqlSendMessage(systemMessage);
    mainWindow->addMessage(MainWindow::currentChat->getID(), MainWindow::currentUser->getID(), messageId, "", "",
                           QImage(), content,
                           SYSTEM_MESSAGE);
    close();

    QMetaObject::invokeMethod(mainWindow->ui->chats_button, "released");

    mainWindow->chats_button_released();
    mainWindow->ui->main_stacked_widget->setCurrentIndex(GROUP_CHAT_LIST_PAGE);

    // Remove chat from chats list and back to chat list
    // TODO
    //mainWindow->removeChat(MainWindow::currentChat->getID());
}

void ChatDialog::loadAddMemberUI() {
    // TODO database searchMessages friends
    QList<UserInfo> friends = sqlLoadFriends(MainWindow::currentUser->getID());
    ui->chat_dialog_add_members_friend_list->clear();
    auto *mainWindow = qobject_cast<MainWindow *>(parentWidget());

    for (const auto &fr: friends)
        mainWindow->addToList<ChatCreationFriendWidget>(fr.getID(), fr.getUsername(), fr.getAvatar(),
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
    mainWindow->ui->chat_online_or_members_label->setText(
            QString::number(ui->chat_dialog_members_list->count() + countMembers) + " участников");

    // Success
    std::vector<int> users;
    for (const auto &user: ui->chat_dialog_add_members_friend_list->selectedItems()) {
        const auto widget = qobject_cast<ChatCreationFriendWidget *>(
                ui->chat_dialog_add_members_friend_list->itemWidget(user));
        users.push_back(widget->getFriendID());

        QString content("%1 присоединился к беседе");
        content = content.arg(widget->getUsername());
        MessageInfo systemMessage(-1, content, nullptr, SYSTEM_MESSAGE, MainWindow::currentChat->getID(),
                                  MainWindow::currentUser->getID());
        // TODO database send message
        int messageId = sqlSendMessage(systemMessage);
        mainWindow->addMessage(MainWindow::currentChat->getID(), MainWindow::currentUser->getID(), messageId, "", "",
                               QImage(), content, SYSTEM_MESSAGE);
    }

    sqlAddMembers(MainWindow::currentChat->getID(), users);

    // Debug information
    for (auto userId: users) {
        printf("%d ", userId);
    }

    // Close chat dialog
    close();
}