#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define APP_PAGE 1

#define AUTHENTICATION_PAGE 0

#define AUTHORIZATION_PAGE 0

#define REGISTRATION_PAGE 1

#include <QMainWindow>
#include <QListWidgetItem>
#include "UserInfo.h"

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

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    UserInfo *currentUser;

    void addChat(int id, const QString &name, const QString &avatar, bool isGroup);

    void updateChat(int id, const QString &name, const QString &avatar);

    static QString checkPasswordAndEmail(const QString &password, const QString &email);

    QMap<int, QListWidgetItem *> chatMap;

private slots:

    void sign_in_button_released();

    void register_button_released();

    void switch_auth_button_released();

    void switch_register_button_released();
};

#endif // MAINWINDOW_H
