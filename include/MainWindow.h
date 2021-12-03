#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define APP_PAGE 1

#define AUTHENTICATION_PAGE 0

#define AUTHORIZATION_PAGE 0

#define REGISTRATION_PAGE 1

#define CHAT_LIST_PAGE 0

#define MESSAGES_PAGE 1

#define CHOOSE_FILE_PAGE_WIDTH 705
#define CHOOSE_FILE_PAGE_HEIGHT 325

#define EDITOR_PAGE_WIDTH 900
#define EDITOR_PAGE_HEIGHT 700

#include <QMainWindow>
#include <QListWidgetItem>
#include <QPlainTextEdit>
#include "UserInfo.h"
#include "ChatForm.h"
#include "ChatDialog.h"
#include "AvatarEditor.h"

enum STATE {
    AUTHORIZATION,
    REGISTRATION,
    CHATS,
    MESSAGES,
    FRIENDS
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

    static ChatForm *currentChat;

    static UserInfo *currentUser;

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    static QPixmap getCircularPixmap(const QImage &image, int size);

    void openChat();

    bool eventFilter(QObject *object, QEvent *event) override;

    void updateChat(int id, const QString &name, const QImage &avatar, ROLE role);

private:

    Ui::MainWindow *ui;

    ChatDialog *chatDialog;

    QMap<int, QListWidgetItem *> chatMap;

    void addChat(int id, const QString &name, const QImage &avatar, bool isGroup, int countMembers, ROLE role);

    void addMessage(int id, const QString &username, const QString &time, const QImage &avatar, const QString &content);

    static QString checkAuthInput(const QString &username, const QString &password, const QString &email);

    int getNewEditTextHeight(const QSizeF &docSize, const QPlainTextEdit *textEdit, int &countLines);

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
