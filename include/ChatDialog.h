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
    explicit ChatDialog(QWidget *parent = nullptr);

    ~ChatDialog() override;

    void setupCurrentChatUi(ChatWidget *chat);

    void removeMemberFromUi(ChatMemberWidget *member);

    void openFileChooser();

private:

    Ui::ChatDialog *ui;

private slots:

    void changeChatName();

    void leaveChat();

    void loadAddMemberUi();

    void addMembers();
};

#endif //CHATDATABASEPROJECT_CHATDIALOG_H
