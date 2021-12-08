#ifndef CHATDATABASEPROJECT_MESSAGETEXTEDIT_H
#define CHATDATABASEPROJECT_MESSAGETEXTEDIT_H

#include <QPlainTextEdit>
#include <QWidget>
#include "CustomPlainTextEdit.h"
#include "UserMessageWidget.h"

class MessageTextEdit : public CustomPlainTextEdit {
Q_OBJECT

private:
    QString content;

public:

    explicit MessageTextEdit(QWidget *parent);

private slots:
    void showContextMenu(const QPoint &pos);

    void editMessage();

    void removeMessage(UserMessageWidget *message);

    void focusOutEvent(QFocusEvent *event) override;

    void editingFinish();
};

#endif //CHATDATABASEPROJECT_MESSAGETEXTEDIT_H
