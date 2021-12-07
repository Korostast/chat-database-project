#ifndef CHATDATABASEPROJECT_MESSAGETEXTEDIT_H
#define CHATDATABASEPROJECT_MESSAGETEXTEDIT_H

#include <QPlainTextEdit>
#include <QWidget>
#include "CustomPlainTextEdit.h"

class MessageTextEdit : public CustomPlainTextEdit {
Q_OBJECT

private:
    QString content;

public:

    explicit MessageTextEdit(QWidget *parent);

    const QString &getContent() const;

private slots:
    void showContextMenu(const QPoint &pos);

    void editMessage();

    void focusOutEvent(QFocusEvent *event) override;

    void editingFinish();
};

#endif //CHATDATABASEPROJECT_MESSAGETEXTEDIT_H
