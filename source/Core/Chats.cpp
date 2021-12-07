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

// Add chat to the chats list
void
MainWindow::addChat(int id, const QString &name, const QImage &avatar, bool isGroup, int countMembers, ROLE role) {
    auto *item = new QListWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

    auto *widget = new ChatWidget(this);
    widget->setId(id);
    widget->setName(name);
    widget->setGroup(isGroup);
    widget->setCountMembers(countMembers);
    widget->setRole(role);

    // TODO avatars
    widget->setAvatar(avatar);

    item->setSizeHint(widget->sizeHint());

    ui->chat_list->insertItem(0, item);
    ui->chat_list->setItemWidget(item, widget);

    chatMap[id] = item;
}

void MainWindow::putOnTop(int id) {
    int rowNumber = ui->chat_list->row(chatMap[id]);
    if (rowNumber == 0)
        return;

    QListWidgetItem *itemN = chatMap[id]->clone();
    auto *widget = qobject_cast<ChatWidget *>(ui->chat_list->itemWidget(chatMap[id]));

    ui->chat_list->insertItem(0, itemN);
    ui->chat_list->setItemWidget(itemN, widget);
    chatMap[id] = itemN;

    auto *item = ui->chat_list->takeItem(rowNumber + 1);
    delete item;
}

// Change chat's attributes and move chat at the top of the list
void MainWindow::updateChat(int id, const QString &name, const QImage &avatar, ROLE role) {
    qDebug() << id;
    putOnTop(id);
    auto *widget = qobject_cast<ChatWidget *>(ui->chat_list->itemWidget(chatMap[id]));
    //qDebug() << widget->getName();

    widget->setName(name);
    if (id == currentChat->getId())
        ui->chat_avatar->setPixmap(AvatarEditor::getCircularPixmap(avatar, AVATAR_IN_CHAT_IMAGE_SIZE));
    widget->setAvatar(avatar);

    widget->setRole(role);

    ui->chat_name_label->setText(name);
}

// Load chat ui
void MainWindow::openChat() {
    qDebug() << "OPENED";
    qDebug() << "Chat messageId: " << currentChat->getId();

    // TODO load messages database
    // Clear all previous messages from list
    //ui->messageList->clear();

    ui->messageList->scrollToBottom();
    ui->chat_avatar->setPixmap(AvatarEditor::getCircularPixmap(currentChat->getAvatar(), AVATAR_IN_CHAT_IMAGE_SIZE));
    ui->chat_name_label->setText(currentChat->getName());
    if (currentChat->isGroup()) {
        ui->chat_online_or_members_label->setText(QString::number(currentChat->getCountMembers()));
        ui->chat_online_or_members_label->show();
    } else {
        ui->chat_name_label->hide();
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
        qDebug() << "Text edit height has changed, countLines =" << countLines;
    }
}

void MainWindow::insertMessage(UserMessageWidget *message, int row) {
    // If we need scroll message list to the bottom
    bool isBottom = false;
    auto *scrollBar = ui->messageList->verticalScrollBar();
    qDebug() << "Current scroll bar value:" << scrollBar->value() << " and Max value:" << scrollBar->maximum();
    if (scrollBar->value() == scrollBar->maximum())
        isBottom = true;

    auto *item = new QListWidgetItem;
    //item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

    auto *widget = new UserMessageWidget(this);

    widget->setChatId(message->getChatId());
    widget->setMessageId(message->getMessageId());
    // TODO image of message widget
    widget->setAvatar(message->getAvatar());
    widget->setName(message->getName());
    widget->setTime(message->getTime());
    widget->setContent(message->getContent());


    item->setSizeHint(widget->sizeHint());
    ui->messageList->insertItem(row, item);
    //ui->messageList->addItem(item);
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
    qDebug() << "Current scroll bar value:" << scrollBar->value() << " and Max value:" << scrollBar->maximum();
    if (scrollBar->value() == scrollBar->maximum())
        isBottom = true;

    auto *item = new QListWidgetItem;
    //item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

    switch (type) {
        case USER_MESSAGE: {
            auto *widget = new UserMessageWidget(this);

            widget->setChatId(chatId);
            widget->setMessageId(messageId);
            // TODO image of message widget
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

    putOnTop(chatId); // TODO message chat ID
}

// Click on name of the chat in the chat ui (open information about chat OR user profile)
// TODO user profile
void MainWindow::chat_name_label_released() {
    qDebug() << "ChatDialog opened";

    if (MainWindow::currentChat->isGroup())
        chatDialog->setupCurrentChatUi(currentChat);
    //else
    //  open user profile
    chatDialog->show();

    // TODO it doesn't work properly
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

    static int messageId = 10;

    if (checkMessage(messageText)) { // TODO MESSAGE ID!!!
        addMessage(currentChat->getId(), messageId++, currentUser->getUsername(),
                   QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"), currentUser->getAvatar(),
                   messageText, USER_MESSAGE);

        ui->message_text_edit->clear();

        // TODO sendMessage database
    }
}

void MainWindow::removeChat(int id) {
    int rowNumber = ui->chat_list->row(chatMap[id]);
    auto *item = ui->chat_list->takeItem(rowNumber);
    delete item;
    chatMap.remove(id);
    chats_button_released();
}

CustomPlainTextEdit *MainWindow::getMessageTextEdit() {
    return ui->message_text_edit;
}

int MainWindow::deleteMessage(UserMessageWidget *message) {
    int size = ui->messageList->count();
    for (int i = 0; i < size; ++i) {
        auto *widget = qobject_cast<UserMessageWidget *>(ui->messageList->itemWidget(ui->messageList->item(i)));
        if (message->getMessageId() == widget->getMessageId()) {
            auto *item = ui->messageList->takeItem(i);
            delete item;
            return i;
        }
    }
    return -1;
}

void MainWindow::setFocusToTextEdit() {
    ui->message_text_edit->setFocus();
}

// Return to chat list
void MainWindow::chats_button_released() {
    ui->main_stacked_widget->setCurrentIndex(CHAT_LIST_PAGE);
    currentChat = nullptr;
    currentState = CHATS;
}