#ifndef CHATDATABASEPROJECT_CHATDIALOG_H
#define CHATDATABASEPROJECT_CHATDIALOG_H

#include <QDialog>
#include "ChatWidget.h"
#include "ChatMemberWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class ChatDialog;
}
QT_END_NAMESPACE

class ChatDialog : public QDialog {
Q_OBJECT

public:
    Ui::ChatDialog *ui;

    explicit ChatDialog(QWidget *parent = nullptr);

    ~ChatDialog() override;

    void setupCurrentChatUI(ChatWidget *chat);

    void removeMemberFromUI(ChatMemberWidget *member);

    void openFileChooser();

private slots:

    void changeChatName();

    void leaveChat();

    void loadAddMemberUI();

    void addMembers();
};

#endif //CHATDATABASEPROJECT_CHATDIALOG_H