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

// Add chat to the chats list
void
MainWindow::addChat(int id, const QString &name, const QImage &avatar, bool isGroup, int countMembers, ROLE role) {
    qInfo() << QString("Chat added: id - %1, username - %2, isGroup - %3, countMembers - %4 and your role - %5")
            .arg(id).arg(name).arg(isGroup).arg(countMembers).arg(role);

    auto *item = new QListWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

    auto *widget = new ChatWidget(this);
    widget->setId(id);
    widget->setName(name);
    widget->setAvatar(avatar);
    widget->setGroup(isGroup);
    widget->setCountMembers(countMembers);
    widget->setRole(role);

    // TODO  If the chat is personal, we have to find its username
    if (!isGroup) {

    }

    item->setSizeHint(widget->sizeHint());

    ui->chat_list->insertItem(0, item);
    ui->chat_list->setItemWidget(item, widget);

    chatMap[id] = item;
}

// Brings the dialog to the top of the chats list
// This function is useless without caching system. However, don't delete it
void MainWindow::putOnTop(int id) {
    int rowNumber = ui->chat_list->row(chatMap[id]);
    if (rowNumber == 0)
        return;

    QListWidgetItem *itemN = chatMap[id]->clone();
    auto *widget = qobject_cast<ChatWidget *>(ui->chat_list->itemWidget(chatMap[id]));

    ui->chat_list->insertItem(0, itemN);
    ui->chat_list->setItemWidget(itemN, widget);
    chatMap[id] = itemN;

    delete ui->chat_list->takeItem(rowNumber + 1);
}

// Change chat's attributes and move chat at the top of the list
void MainWindow::updateChat(int id, const QString &name, const QImage &avatar, ROLE role) {
    qInfo() << QString("Chat updated, new parameters: id - %1, username - %2 and your role - %3")
            .arg(id).arg(name).arg(role);

    putOnTop(id);
    auto *widget = qobject_cast<ChatWidget *>(ui->chat_list->itemWidget(chatMap[id]));

    widget->setName(name);
    if (id == currentChat->getId())
        ui->chat_avatar->setPixmap(AvatarEditor::getCircularPixmap(avatar, AVATAR_IN_CHAT_IMAGE_SIZE));
    widget->setAvatar(avatar);
    widget->setRole(role);
    ui->chat_name_label->setText(name);
}

// Load chat ui
void MainWindow::openChat() {
    qInfo() << QString("Opened chat: id - %1, username - %2, isGroup - %3, countMembers - %4 and your role - %5")
            .arg(currentChat->getId()).arg(currentChat->getName()).arg(currentChat->isGroup())
            .arg(currentChat->getCountMembers()).arg(currentChat->getRole());

    // TODO database load messages
    QList<MessageInfo> messages = sqlLoadMessages(currentChat->getId());

    // Clear all previous messages from list
    ui->messageList->clear();

    for (const auto &message : messages)
        addMessage(message.chatId, message.messageId, message.username, message.time, message.avatar, message.content,
                   message.type);

    ui->messageList->scrollToBottom();
    ui->chat_avatar->setPixmap(AvatarEditor::getCircularPixmap(currentChat->getAvatar(), AVATAR_IN_CHAT_IMAGE_SIZE));
    ui->chat_name_label->setText(currentChat->getName());
    if (currentChat->isGroup()) {
        ui->chat_online_or_members_label->setText(QString::number(currentChat->getCountMembers()));
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

    widget->setChatId(message->getChatId());
    widget->setMessageId(message->getMessageId());
    // TODO image of message widget | upd. Done ?
    const auto &avatar = message->getAvatar();
    widget->setAvatar(avatar.isNull() ? QImage(":chatDefaultImage") : avatar);
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
MainWindow::addMessage(int chatId, int messageId, const QString &username, const QString &time, const QImage &avatar,
                       const QString &content, MESSAGE_TYPE type) {
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

            widget->setChatId(chatId);
            widget->setMessageId(messageId);
            // TODO image of message widget | upd. Done ?
            widget->setAvatar(avatar.isNull() ? QImage(":chatDefaultImage") : avatar);
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

            widget->setId(messageId);
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

    //putOnTop(chatId); // TODO message chat ID | I don't remember what does it means
}

// Click on username of the chat in the chat ui (open information about chat OR user profile)
// TODO user profile | almost done
void MainWindow::chat_name_label_released() {
    qDebug() << "ChatDialog opened";

    if (MainWindow::currentChat->isGroup()) {
        chatDialog->setupCurrentChatUi(currentChat);
        chatDialog->show();
    } else {
        // TODO database load profile
        //UserInfo user(1, "lasdlfasd", QImage(":chatDefaultImage"), nullptr);
        UserInfo user = sqlLoadProfile(currentChat->getName());
        showProfile(&user);
    }

    // TODO it doesn't work properly | but don't remove it, maybe this will be helpful in the future
//    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect;
//    effect->setBlurRadius(5);
//    this->setGraphicsEffect(effect);
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

    //static int messageId = 10; // TODO MESSAGE ID!!! It's a test

    if (checkMessage(messageText)) {
        MessageInfo message(-1, messageText, nullptr, USER_MESSAGE, currentChat->getId(), currentUser->getId());
        // TODO database sendMessage
        int messageId = sqlSendMessage(message);
        addMessage(currentChat->getId(), messageId, currentUser->getUsername(),
                   QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"), currentUser->getAvatar(),
                   messageText, USER_MESSAGE);

        ui->message_text_edit->clear();

    }

    setFocusToTextEdit();
}

// Obvious
void MainWindow::removeChat(int id) {
    int rowNumber = ui->chat_list->row(chatMap[id]);
    delete ui->chat_list->takeItem(rowNumber);
    chatMap.remove(id);
    chats_button_released();
}

// It is used in editing message and deleting too
int MainWindow::deleteMessage(UserMessageWidget *message) const {
    int size = ui->messageList->count();
    for (int i = 0; i < size; ++i) {
        auto *widget = qobject_cast<UserMessageWidget *>(ui->messageList->itemWidget(ui->messageList->item(i)));
        if (message->getMessageId() == widget->getMessageId()) {
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
void MainWindow::chats_button_released()  {
    // TODO database load chats
    QList<ChatInfo> chats = sqlLoadChats(currentUser->getId());

    ui->chat_list->clear();

    for (const auto &chat : chats)
        addChat(chat.id, chat.name, chat.avatar, chat.group, chat.countMembers, chat.role);

    ui->main_stacked_widget->setCurrentIndex(CHAT_LIST_PAGE);
    currentChat = nullptr;
    currentState = CHATS;
    qDebug() << "Current state changed to CHATS";
}