#include <QMenu>
#include "MessageTextEdit.h"
#include "UserMessageWidget.h"
#include "MainWindow.h"
#include "SqlInterface.h"

MessageTextEdit::MessageTextEdit(QWidget *parent) : CustomPlainTextEdit(parent) {
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(this, SIGNAL(returnKeyPressedEvent()), this, SLOT(editingFinish()));
}

// When we right-click on the message, context menu show
void MessageTextEdit::showContextMenu(const QPoint &pos) {
    qDebug() << "Context menu called";

    auto *userMessageWidget = qobject_cast<UserMessageWidget *>(this->parentWidget());

    // If the initiator is me, then create context menu
    auto *sender = QObject::sender();
    auto *mainWindow = qobject_cast<MainWindow *>(this->window());

    QMenu contextMenu("Context Menu", this);
    bool isShow = false;
    QAction editAction("Редактировать", this);
    QAction removeAction("Удалить", this);
    if (userMessageWidget->getName() == MainWindow::currentUser->getUsername()) {
        connect(&editAction, SIGNAL(triggered()), this, SLOT(editMessage()));
        contextMenu.addAction(&editAction);
        isShow = true;
    }
    if (MainWindow::currentChat->getRole() > MODERATOR ||
        userMessageWidget->getName() == MainWindow::currentUser->getUsername()) {
        connect(&removeAction, &QAction::triggered, this, [this, &userMessageWidget]() {
            removeMessage(userMessageWidget);
        });
        contextMenu.addAction(&removeAction);
        isShow = true;
    }
    if (isShow) {
        contextMenu.exec(mapToGlobal(pos));
    }
}

// We clicked 'edit'
void MessageTextEdit::editMessage() {
    qDebug() << "Editing message";
    setTextInteractionFlags(Qt::TextEditable | Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    selectAll();
}

// We clicked 'remove'
void MessageTextEdit::removeMessage(UserMessageWidget *message) {
    // TODO database delete message
    sqlDeleteMessage(message->getMessageId());

    qDebug() << QString("Removing message with messageId").arg(message->getMessageId());
    auto *mainWindow = qobject_cast<MainWindow *>(window());
    mainWindow->deleteMessage(message);
}

// If we clicked somewhere else, we should cancel editing
void MessageTextEdit::focusOutEvent(QFocusEvent *event) {
    QPlainTextEdit::focusOutEvent(event);
    if (textInteractionFlags() & Qt::TextEditable) {
        auto *userMessageWidget = qobject_cast<UserMessageWidget *>(parentWidget());
        setPlainText(userMessageWidget->getContent());
        setTextInteractionFlags(textInteractionFlags() & ~Qt::TextEditable);
        qDebug() << "Focus lost while editing. We are sorry. Return to old content";
    }
}

// We coped with editing functionality!
void MessageTextEdit::editingFinish() {
    if (textInteractionFlags() & Qt::TextEditable) {
        // If text is unchanged
        auto *userMessageWidget = qobject_cast<UserMessageWidget *>(parentWidget());
        auto *mainWindow = qobject_cast<MainWindow *>(window());
        if (toPlainText() == userMessageWidget->getContent()) {
            qInfo() << "Text edit is not changed";
            setTextInteractionFlags(textInteractionFlags() & ~Qt::TextEditable);

            QTextCursor cursor = textCursor();
            cursor.clearSelection();
            setTextCursor(cursor);

            mainWindow->setFocusToTextEdit();
            return;
        }

        qInfo() << QString("Edited message with messageId - %1. Old content: %2. New content: %3")
                .arg(userMessageWidget->getMessageId()).arg(userMessageWidget->getContent(), toPlainText());

        setTextInteractionFlags(textInteractionFlags() & ~Qt::TextEditable);

        UserMessageWidget message;
        message.setChatId(userMessageWidget->getChatId());
        message.setMessageId(userMessageWidget->getMessageId());
        message.setName(userMessageWidget->getName());
        message.setTime(userMessageWidget->getTime());
        message.setAvatar(userMessageWidget->getAvatar());
        message.setContent(QString("%1\n%2").arg(toPlainText(), "[изменено]"));
        userMessageWidget->setContent(message.getContent());

        // TODO database message is edited
        sqlMessageEdited(message.getMessageId(), message.getContent());

        int rowNumber = mainWindow->deleteMessage(userMessageWidget);
        qDebug() << userMessageWidget->getContent();
        mainWindow->insertMessage(&message, rowNumber);
    }
}