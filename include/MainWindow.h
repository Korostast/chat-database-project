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

enum STATE {
    AUTHORIZATION,
    REGISTRATION,
    CHATS,
    MESSAGES,
    FRIENDS,
    PROFILE
};

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
Q_OBJECT

public:
    //TODO
    AvatarEditor *avatarEditor;

    static STATE currentState;

    static ChatWidget *currentChat;

    static UserInfo *currentUser;

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    void openChat();

    void updateChat(int id, const QString &name, const QImage &avatar, ROLE role);

    void putOnTop(int id);

    void addMessage(int chatId, int messageId, const QString &username, const QString &time, const QImage &avatar,
                    const QString &content, MESSAGE_TYPE type);

    void insertMessage(UserMessageWidget *message, int row);

    int deleteMessage(UserMessageWidget *message);

    int getNewEditTextHeight(const QSizeF &docSize, const QPlainTextEdit *textEdit, int &countLines);

    void removeChat(int id);

    CustomPlainTextEdit *getMessageTextEdit();

private:

    bool eventFilter(QObject *object, QEvent *event) override;

    Ui::MainWindow *ui;

    ChatDialog *chatDialog;

    QMap<int, QListWidgetItem *> chatMap;

    static QString checkAuthInput(const QString &username, const QString &password, const QString &email);

    void addChat(int id, const QString &name, const QImage &avatar, bool isGroup, int countMembers, ROLE role);

    static bool checkMessage(QString &content);

private slots:

    void sign_in_button_released();

    void register_button_released();

    void switch_auth_button_released();

    void switch_register_button_released();

    void messageTextChanged(QSizeF docSize);

    void chat_name_label_released();

    void chats_button_released();

    void sendMessage();
};

#endif // MAINWINDOW_H
