#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QPlainTextEdit>
#include "UserInfo.h"
#include "ChatWidget.h"
#include "ChatDialog.h"
#include "AvatarEditor.h"
#include "Defines.h"
#include "CustomPlainTextEdit.h"
#include "UserMessageWidget.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
Q_OBJECT

public:
    AvatarEditor *avatarEditor;

    static STATE currentState;

    static ChatWidget *currentChat;

    static UserInfo *currentUser;

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    void openChat();

    void updateChat(int id, const QString &name, const QImage &avatar, ROLE role);

    void putOnTop(int id);

    void addMessage(int chatId, int userId, int messageId, const QString &username, const QString &time,
                    const QImage &avatar, const QString &content, MESSAGE_TYPE type);

    void insertMessage(UserMessageWidget *message, int row);

    int deleteMessage(UserMessageWidget *message) const;

    int getNewEditTextHeight(const QSizeF &docSize, const QPlainTextEdit *textEdit, int &countLines);

    void removeChat(int id);

    void setFocusToTextEdit() const;

    // Templated function for friend widget and incoming, outcoming requests
    // FRIENDS FOO
    template <typename T>
    void addToList(int friendId, const QString &username, const QImage &avatar, QListWidget *list){
        auto *item = new QListWidgetItem;

        auto *widget = new T(this);
        widget->setFriendId(friendId);
        widget->setUsername(username);

        // TODO avatars | done ?
        widget->setAvatar(avatar.isNull() ? QImage("chatDefaultImage") : avatar);

        item->setSizeHint(widget->sizeHint());

        list->insertItem(0, item);
        list->setItemWidget(item, widget);
    }

    template<typename T>
    static void removeFromList(int requestId, QListWidget *list);

    void addPersonInSearch(int personId, const QString &username, const QImage &avatar);

    void showProfile(const UserInfo *user) const;

    void setChatCreationAvatar(const QImage &avatar) const;

    Ui::MainWindow *ui;

    void tests();

private:

    bool eventFilter(QObject *object, QEvent *event) override;

    ChatDialog *chatDialog;

    QMap<int, QListWidgetItem *> chatMap;

    static QString checkAuthInput(const QString &username, const QString &password, const QString &email);

    void addChat(int id, const QString &name, const QImage &avatar, bool isGroup, int countMembers, ROLE role);

    static bool checkMessage(QString &content);

    void switch_friends_page(int page) const;

private slots:

    void sign_in_button_released() const;

    void register_button_released() const;

    void switch_auth_button_released() const;

    void switch_register_button_released() const;

    void messageTextChanged(QSizeF docSize);

    void chat_name_label_released();

    void chats_button_released();

    void profile_button_released() const;

    void friends_button_released();

    void settings_button_released() const;

    void search_people();

    void sendMessage();

    void chat_creation_open_ui();

    void group_chat_create();

    void settings_save_button_released();
};

#endif // MAINWINDOW_H
