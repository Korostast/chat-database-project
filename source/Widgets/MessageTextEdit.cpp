#include <QMenu>
#include "MessageTextEdit.h"
#include "UserMessageWidget.h"
#include "MainWindow.h"

MessageTextEdit::MessageTextEdit(QWidget *parent) : CustomPlainTextEdit(parent) {
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));
    connect(this, SIGNAL(returnKeyPressedEvent()), this, SLOT(editingFinish()));
}

void MessageTextEdit::showContextMenu(const QPoint &pos) {
    qDebug() << "Context menu called";

    auto *userMessageWidget = qobject_cast<UserMessageWidget *>(this->parentWidget());

    // If the initiator is me, then create context menu
    auto *sender = QObject::sender();
    auto *mainWindow = qobject_cast<MainWindow *>(this->window());

    if (userMessageWidget->getName() == MainWindow::currentUser->getUsername()) {
        QMenu contextMenu("Context Menu", this);
        QAction editAction("Редактировать", this);
        connect(&editAction, SIGNAL(triggered()), this, SLOT(editMessage()));
        contextMenu.addAction(&editAction);
        contextMenu.exec(mapToGlobal(pos));
    }
}

void MessageTextEdit::editMessage() {
    qDebug() << "Editing message";
    setTextInteractionFlags(Qt::TextEditable | Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    selectAll();
}

void MessageTextEdit::focusOutEvent(QFocusEvent *event) {
    QPlainTextEdit::focusOutEvent(event);
    if (textInteractionFlags() & Qt::TextEditable) {
        auto *userMessageWidget = qobject_cast<UserMessageWidget *>(parentWidget());
        setPlainText(userMessageWidget->getContent());
        setTextInteractionFlags(textInteractionFlags() & ~Qt::TextEditable);
        qDebug() << "Focus lost while editing. We are sorry. Return to old content";
    }
}

void MessageTextEdit::editingFinish() {
    if (textInteractionFlags() & Qt::TextEditable) {
        // TODO database
        qDebug() << "Text edit focusOutEvent";
        setTextInteractionFlags(textInteractionFlags() & ~Qt::TextEditable);
        //setPlainText(QString("%1\n\t\t\t\t[изменено]").arg(toPlainText()));
        auto *mainWindow = qobject_cast<MainWindow *>(window());
        //int defaultRowNumber = 0;
        //auto newHeight = mainWindow->getNewEditTextHeight(document()->size(), this, defaultRowNumber);
        auto *userMessageWidget = qobject_cast<UserMessageWidget *>(parentWidget());    // TODO LOCAL VARIABLE COPY
        // TODO constructor
        UserMessageWidget message;
        message.setChatId(userMessageWidget->getChatId());
        message.setMessageId(userMessageWidget->getMessageId());
        message.setName(userMessageWidget->getName());
        message.setTime(userMessageWidget->getTime());
        message.setAvatar(userMessageWidget->getAvatar());
        message.setContent(QString("%1\n%2").arg(toPlainText(), "[изменено]"));
        userMessageWidget->setContent(message.getContent());
        //setFixedHeight(300);
        //mainWindow->deleteMessage()
        int rowNumber = mainWindow->deleteMessage(userMessageWidget);
        qDebug() << userMessageWidget->getContent();
        mainWindow->insertMessage(&message, rowNumber);
    }
}

const QString &MessageTextEdit::getContent() const {
    return content;
}

void MessageTextEdit::setContent(const QString &content) {
    MessageTextEdit::content = content;
}
