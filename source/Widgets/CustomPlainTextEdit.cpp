#include "CustomPlainTextEdit.h"
#include "UserMessageWidget.h"
#include "MainWindow.h"
#include <QApplication>
#include <QMenu>

CustomPlainTextEdit::CustomPlainTextEdit(QWidget *parent)
        : QPlainTextEdit(parent) {}

void CustomPlainTextEdit::keyPressEvent(QKeyEvent *event) {  // TODO eventFilter??
    if (event->key() == Qt::Key_Return && event->modifiers() == Qt::NoModifier) {
        emit returnKeyPressedEvent();
        event->accept();
    } else {
        QPlainTextEdit::keyPressEvent(event);
    }
}
