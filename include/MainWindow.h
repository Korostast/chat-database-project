#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define APP_PAGE 1

#define AUTHENTICATION_PAGE 0

#define AUTHORIZATION_PAGE 0

#define REGISTRATION_PAGE 1

#include <QMainWindow>
#include <QListWidgetItem>
#include <QPlainTextEdit>
#include "UserInfo.h"
#include "ChatForm.h"
#include "ChatDialog.h"

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
    STATE currentState;

    static ChatForm *currentChat;

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    static QPixmap getCircularPixmap(const QString &pathToImage, int size);

    static void openChat();

    bool eventFilter(QObject *object, QEvent *event) override;

private:

    Ui::MainWindow *ui;

    ChatDialog *chatDialog;

    UserInfo *currentUser;

    QMap<int, QListWidgetItem *> chatMap;

    void addChat(int id, const QString &name, const QString &avatar, bool isGroup);

    void addMessage(int id, const QString &username, const QString &time, const QString &avatar, const QString &content);

    void updateChat(int id, const QString &name, const QString &avatar);

    static QString checkAuthInput(const QString &username, const QString &password, const QString &email);

    int getNewEditTextHeight(const QSizeF &docSize, const QPlainTextEdit *textEdit, int &countLines);

private slots:

    void sign_in_button_released();

    void register_button_released();

    void switch_auth_button_released();

    void switch_register_button_released();

    void messageTextChanged(QSizeF docSize);

    void chat_name_label_released();
};

#endif // MAINWINDOW_H
