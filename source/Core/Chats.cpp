#include <QPainter>
#include <QPainterPath>
#include <QLayout>
#include "UserMessageWidget.h"
#include "SystemMessageWidget.h"
#include <QGraphicsEffect>
#include <QDateTime>
#include <QScrollBar>
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "SqlInterface.h"
#include "ChatCreationFriendWidget.h"

// Add chat to the chats list
void
MainWindow::addChat(int id, const QString &name, const QImage &avatar, bool isGroup, int countMembers, ROLE role,
                    int friendID) {
    qInfo() << QString("Chat added: messageID - %1, username - %2, isGroup - %3, countMembers - %4 and your role - %5")
            .arg(id).arg(name).arg(isGroup).arg(countMembers).arg(role);

    auto *item = new QListWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

    auto *widget = new ChatWidget(this);
    widget->setID(id);
    widget->setName(name);
    widget->setAvatar(avatar, isGroup);
    widget->setGroup(isGroup);
    widget->setCountMembers(countMembers);
    widget->setRole(role);

    item->setSizeHint(widget->sizeHint());

    if (!isGroup) {
        widget->setFriendID(friendID);
        ui->personal_chat_list->insertItem(0, item);
        ui->personal_chat_list->setItemWidget(item, widget);
    } else {
        ui->group_chat_list->insertItem(0, item);
        ui->group_chat_list->setItemWidget(item, widget);
    }

    chatMap[id] = item;
}

// Brings the dialog to the top of the chats list
// This function is useless without caching system. However, don't delete it
void MainWindow::putOnTop(int id) {
    int rowNumber = ui->personal_chat_list->row(chatMap[id]);
    if (rowNumber == 0)
        return;

    QListWidgetItem *itemN = chatMap[id]->clone();
    auto *widget = qobject_cast<ChatWidget *>(ui->personal_chat_list->itemWidget(chatMap[id]));

    ui->personal_chat_list->insertItem(0, itemN);
    ui->personal_chat_list->setItemWidget(itemN, widget);
    chatMap[id] = itemN;

    delete ui->personal_chat_list->takeItem(rowNumber + 1);
}

// Change chat's attributes and move chat at the top of the list
void MainWindow::updateChat(int id, const QString &name, const QImage &avatar, ROLE role) {
    qInfo() << QString("Chat updated, new parameters: messageID - %1, username - %2 and your role - %3")
            .arg(id).arg(name).arg(role);

    //putOnTop(id);
    /*auto *widget = qobject_cast<ChatWidget *>(ui->personal_chat_list->itemWidget(chatMap[id]));

    widget->setName(name);
    if (id == currentChat->getID())
        ui->chat_avatar->setPixmap(AvatarEditor::getCircularPixmap(avatar, AVATAR_IN_CHAT_IMAGE_SIZE));
    widget->setAvatar(avatar);
    widget->setRole(role);*/
    if (id == currentChat->getID())
        ui->chat_avatar->setPixmap(AvatarEditor::getCircularPixmap(avatar, AVATAR_IN_CHAT_IMAGE_SIZE));
    ui->chat_name_label->setText(name);
}

// Load chat ui
void MainWindow::openChat() {
    qInfo() << QString("Opened chat: chatID - %1, name - %2, isGroup - %3, countMembers - %4 and your role - %5")
            .arg(currentChat->getID()).arg(currentChat->getName()).arg(currentChat->isGroup())
            .arg(currentChat->getCountMembers()).arg(currentChat->getRole());

    // TODO database searchMessages messages
    QList<MessageInfo> messages = sqlLoadMessages(currentChat->getID());

    // Clear all previous messages from list
    ui->messageList->clear();

    for (const auto &message: messages) {
        addMessage(message.chatID, message.userID, message.messageID, message.username, message.time,
                   message.avatar, message.content, message.type);
        qDebug() << "Message with date:" << message.time;
    }

    ui->messageList->scrollToBottom();
    ui->chat_avatar->setPixmap(AvatarEditor::getCircularPixmap(currentChat->getAvatar(), AVATAR_IN_CHAT_IMAGE_SIZE));
    ui->chat_name_label->setText(currentChat->getName());
    if (currentChat->isGroup()) {
        ui->chat_online_or_members_label->setText(QString("%1 участников")
                                                          .arg(QString::number(currentChat->getCountMembers())));
        ui->chat_online_or_members_label->show();
    } else {
        ui->chat_online_or_members_label->hide();
    }
    if (currentChat->getRole() == VIEWER) {
        ui->message_text_edit->hide();
        ui->send_message_button->hide();
    } else {
        ui->message_text_edit->show();
        ui->send_message_button->show();
    }

    currentState = MESSAGES;
    ui->chat_bar_stacked_widget->setCurrentIndex(0);
    ui->main_stacked_widget->setCurrentIndex(MESSAGES_PAGE);
    setFocusToTextEdit();
}

// Return new size of text edit
int MainWindow::getNewEditTextHeight(const QSizeF &docSize, const QPlainTextEdit *textEdit, int &countLines) {
    auto widgetMargins = contentsMargins();
    auto documentMargin = textEdit->document()->documentMargin();
    auto fontMetrics = QFontMetrics(textEdit->document()->defaultFont());
    countLines = int(round(docSize.height()));

    auto newHeight = widgetMargins.top() +
                     documentMargin +
                     (countLines + 1) * fontMetrics.height() +
                     textEdit->document()->documentMargin() +
                     widgetMargins.bottom();

    return int(round(newHeight));
}

// Text edit size adjustment
void MainWindow::messageTextChanged(QSizeF docSize) {
    auto *textEdit = ui->message_text_edit;
    static int countLines = 0;

    if (textEdit->height() < textEdit->baseSize().height() || int(round(docSize.height())) < countLines) {
        qreal newHeight = std::min(getNewEditTextHeight(docSize, textEdit, countLines), textEdit->baseSize().height());
        textEdit->setFixedHeight(int(round(newHeight)));
    }
}

// Function for editing messages. When we finished editing, original message replace with new one. I do it because Qt
// doesn't want to change size of the original widget for some reason
void MainWindow::insertMessage(UserMessageWidget *message, int row) {
    // If we need scroll message list to the bottom
    bool isBottom = false;
    auto *scrollBar = ui->messageList->verticalScrollBar();
    if (scrollBar->value() == scrollBar->maximum())
        isBottom = true;

    auto *item = new QListWidgetItem;
    auto *widget = new UserMessageWidget(this);

    widget->setChatID(message->getChatID());
    widget->setUserID(message->getUserID());
    widget->setMessageID(message->getMessageID());
    // TODO image of message widget | upd. Done ?
    const auto &avatar = message->getAvatar();
    widget->setAvatar(avatar);
    widget->setName(message->getName());
    widget->setTime(message->getTime());
    widget->setContent(message->getContent());


    item->setSizeHint(widget->sizeHint());
    ui->messageList->insertItem(row, item);
    ui->messageList->setItemWidget(item, widget);

    // Update scrollbar value
    if (currentState == MESSAGES && isBottom)
        ui->messageList->scrollToBottom();
}

// Add message to the message list
void
MainWindow::addMessage(int chatID, int userID, int messageID, const QString &username, const QString &time,
                       const QImage &avatar, const QString &content, MESSAGE_TYPE type) {
    // If we need scroll message list to the bottom
    bool isBottom = false;
    auto *scrollBar = ui->messageList->verticalScrollBar();

    qDebug() << QString("Current scroll bar value - %1; max value - %2").arg(QString::number(scrollBar->value()),
                                                                             QString::number(scrollBar->maximum()));

    if (scrollBar->value() == scrollBar->maximum())
        isBottom = true;

    auto *item = new QListWidgetItem;

    switch (type) {
        case USER_MESSAGE: {
            auto *widget = new UserMessageWidget(this);

            widget->setChatID(chatID);
            widget->setUserID(userID);
            widget->setMessageID(messageID);
            // TODO image of message widget | upd. Done ?
            widget->setAvatar(avatar);
            widget->setName(username);
            widget->setTime(time);
            widget->setContent(content);


            item->setSizeHint(widget->sizeHint());
            ui->messageList->addItem(item);
            ui->messageList->setItemWidget(item, widget);

            break;
        }
        case SYSTEM_MESSAGE: {
            auto *widget = new SystemMessageWidget(this);

            widget->setMessageID(messageID);
            widget->setUserID(userID);
            widget->setContent(content);

            item->setSizeHint(widget->sizeHint());
            ui->messageList->addItem(item);
            ui->messageList->setItemWidget(item, widget);

            break;
        }
        default: {
            qDebug() << "Unknown message type";
            return;
        }
    }

    // Update scrollbar value
    if (currentState == MESSAGES && isBottom)
        ui->messageList->scrollToBottom();

    //putOnTop(chatID); // TODO message chat ID | I don't remember what does it means
}

// Click on username of the chat in the chat ui (open information about chat OR user profile)
// TODO user profile | almost done
void MainWindow::chat_name_label_released() {
    qDebug() << "ChatDialog opened";

    if (MainWindow::currentChat->isGroup()) {
        chatDialog->setupCurrentChatUI(currentChat);
        chatDialog->show();
    } else {
        // TODO database searchMessages profile
        UserInfo user = sqlLoadProfile(currentChat->getFriendID());
        showProfile(&user);
    }

    // TODO it doesn't work properly | but don't remove it, maybe this will be helpful in the future
    /*QGraphicsBlurEffect *effect = new QGraphicsBlurEffect;
    effect->setBlurRadius(5);
    this->setGraphicsEffect(effect);*/
}

// If message is correct
bool MainWindow::checkMessage(QString &content) { // TODO delete 2+ spaces and new lines
    QString tempString(content);
    content.clear();
    content.append(tempString.trimmed());
    if (content.isEmpty())
        return false;
    return true;
}

// User press enter
void MainWindow::sendMessage() {
    QString messageText = ui->message_text_edit->toPlainText();

    if (checkMessage(messageText)) {
        MessageInfo message(-1, messageText, nullptr, USER_MESSAGE, currentChat->getID(), currentUser->getID());
        // TODO database sendMessage
        int messageId = sqlSendMessage(message);
        addMessage(currentChat->getID(), currentUser->getID(), messageId, currentUser->getUsername(),
                   QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"), currentUser->getAvatar(),
                   messageText, USER_MESSAGE);

        ui->message_text_edit->clear();

    }

    setFocusToTextEdit();
}

// Obvious TODO chats_button_released is not needed here
void MainWindow::removeChat(int id) {
    int rowNumber = ui->personal_chat_list->row(chatMap[id]);
    delete ui->personal_chat_list->takeItem(rowNumber);
    chatMap.remove(id);
    //chats_button_released();
}

// It is used in editing message and deleting too
int MainWindow::deleteMessage(UserMessageWidget *message) const {
    int size = ui->messageList->count();
    for (int i = 0; i < size; ++i) {
        auto *widget = qobject_cast<UserMessageWidget *>(ui->messageList->itemWidget(ui->messageList->item(i)));

        if (widget == nullptr)
            continue;

        if (message->getMessageID() == widget->getMessageID()) {
            delete ui->messageList->takeItem(i);
            return i;
        }
    }
    return -1;
}

void MainWindow::setFocusToTextEdit() const {
    ui->message_text_edit->setFocus();
}

// Return to chat list
void MainWindow::chats_button_released() {
    // Clear temp variables
    tempImage = QImage();

    // TODO database searchMessages chats
    QList<PersonalChatInfo> personalChats = sqlLoadPersonalChats(currentUser->getID());
    QList<GroupChatInfo> groupChats = sqlLoadGroupChats(currentUser->getID());

    ui->personal_chat_list->clear();
    ui->group_chat_list->clear();

    for (const auto &chat: personalChats)
        addChat(chat.chatID, chat.name, chat.avatar, chat.group, chat.countMembers, chat.role, chat.friendID);

    for (const auto &chat: groupChats)
        addChat(chat.chatID, chat.name, chat.avatar, chat.group, chat.countMembers, chat.role, -1);

    if (currentChat != nullptr && currentChat->isGroup())
        ui->main_stacked_widget->setCurrentIndex(GROUP_CHAT_LIST_PAGE);
    else
        ui->main_stacked_widget->setCurrentIndex(PERSONAL_CHAT_LIST_PAGE);

    currentChat = nullptr;
    currentState = CHATS;
    qDebug() << "Current state changed to CHATS";
}

void MainWindow::chat_creation_open_ui() {
    // TODO database searchMessages friends
    QList<UserInfo> friends = sqlLoadFriends(currentUser->getID());
    ui->chat_creation_friends_list->clear();
    ui->chat_creation_name_edit->clear();
    ui->chat_creation_avatar->setPixmap(AvatarEditor::getCircularPixmap(QImage(":chat default image"),
                                                                        CHAT_CREATION_CHAT_IMAGE_SIZE));
    for (const auto &fr: friends)
        addToList<ChatCreationFriendWidget>(fr.getID(), fr.getUsername(), fr.getAvatar(),
                                            ui->chat_creation_friends_list);

    ui->main_stacked_widget->setCurrentIndex(CHAT_CREATION_PAGE);
    currentChat = nullptr;
    currentState = CHAT_CREATION;
    qDebug() << "Current state changed to CHAT_CREATION";
}

void MainWindow::group_chat_create() {
    // TODO database create chat
    QString chatName(ui->chat_creation_name_edit->text());
    if (chatName.isEmpty())
        return;

    // Check if at least one user is selected
    int countMembers = (int) ui->chat_creation_friends_list->selectedItems().size();

    // Success
    std::vector<int> users;
    for (const auto &user: ui->chat_creation_friends_list->selectedItems())
        users.push_back(qobject_cast<ChatCreationFriendWidget *>(
                ui->chat_creation_friends_list->itemWidget(user))->getFriendID());

    int chatID = sqlCreateGroupChat(currentUser->getID(), chatName, tempImage, users);
    addChat(chatID, chatName, tempImage, true, countMembers, ADMIN, -1);

    // Debug information
    for (auto userId: users)
        printf("%d ", userId);

    // Clear temp variables
    tempImage = QImage();

    // Return to chat list ui
    currentState = CHATS;
    chats_button_released();
    ui->main_stacked_widget->setCurrentIndex(GROUP_CHAT_LIST_PAGE);
}

void MainWindow::loadSearchInterface() {
    ui->messages_search_edit->setText(ui->chat_bar_search_edit->text());

    searchMessages();

    currentState = SEARCH_MESSAGES;
    if (currentChat->getRole() >= MODERATOR)
        ui->messages_search_remove_messages->show();
    else
        ui->messages_search_remove_messages->hide();

    ui->main_stacked_widget->setCurrentIndex(MESSAGES_SEARCH_PAGE);
}

void MainWindow::searchMessages() {
    QString request(ui->messages_search_edit->text());
    if (request.isEmpty())
        return;

    // TODO database search messages
    ui->messages_search_list->clear();
    QList<MessageInfo> messages = sqlLoadSearchedMessages(currentChat->getID(), request);
    for (const auto &message: messages) {
        auto *item = new QListWidgetItem;
        if (message.type == USER_MESSAGE) {
            auto *widget = new UserMessageWidget(this);

            widget->setChatID(message.chatID);
            widget->setUserID(message.userID);
            widget->setMessageID(message.messageID);
            widget->setAvatar(message.avatar);
            widget->setName(message.username);
            widget->setTime(message.time);
            widget->setContent(message.content);

            item->setSizeHint(widget->sizeHint());
            ui->messages_search_list->addItem(item);
            ui->messages_search_list->setItemWidget(item, widget);
        } else if (message.type == SYSTEM_MESSAGE) {
            auto *widget = new SystemMessageWidget(this);

            widget->setMessageID(message.messageID);
            widget->setUserID(message.userID);
            widget->setContent(message.content);

            item->setSizeHint(widget->sizeHint());
            ui->messageList->addItem(item);
            ui->messageList->setItemWidget(item, widget);
        }
    }
}

void MainWindow::deleteMessagesInSearch() const {
    QString pattern(ui->messages_search_edit->text());
    if (pattern.isEmpty())
        return;

    // TODO database delete messages by pattern
    sqlDeleteMessagesByPattern(currentChat->getID(), pattern);
    ui->messages_search_list->clear();
}