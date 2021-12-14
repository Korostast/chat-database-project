#ifndef CHATDATABASEPROJECT_DEFINES_H
#define CHATDATABASEPROJECT_DEFINES_H

#define CHAT_IMAGE_SIZE 50
#define USER_MESSAGE_IMAGE_SIZE 40
#define AVATAR_IN_CHAT_IMAGE_SIZE 30
#define CHAT_DIALOG_AVATAR_IMAGE_SIZE 100
#define CHAT_MEMBER_IMAGE_SIZE 40
#define FRIEND_IMAGE_SIZE 70
#define INCOMING_REQUESTS_IMAGE_SIZE 70
#define OUTCOMING_REQUESTS_IMAGE_SIZE 70
#define SEARCH_PEOPLE_IMAGE_SIZE 70
#define PROFILE_IMAGE_SIZE 150
#define CHAT_CREATION_FRIEND_IMAGE_SIZE 70
#define CHAT_CREATION_CHAT_IMAGE_SIZE 50

#define AUTHENTICATION_PAGE 0
#define APP_PAGE 1

#define AUTHORIZATION_PAGE 0
#define REGISTRATION_PAGE 1

#define CHAT_LIST_PAGE 0
#define MESSAGES_PAGE 1
#define PROFILE_PAGE 2
#define FRIENDS_PAGE 3
#define CHAT_CREATION_PAGE 4
#define SETTINGS_PAGE 5

#define ACTUAL_FRIENDS_PAGE 0
#define INCOMING_REQUESTS_PAGE 1
#define OUTCOMING_REQUESTS_PAGE 2
#define SEARCH_PEOPLE_PAGE 3

#define CHOOSE_FILE_PAGE_WIDTH 705
#define CHOOSE_FILE_PAGE_HEIGHT 325

#define EDITOR_PAGE_WIDTH 900
#define EDITOR_PAGE_HEIGHT 700

#define MESSAGE_LINE_WIDTH 435

#include <QImage>
#include <utility>

enum STATE {
    AUTHORIZATION,
    REGISTRATION,
    CHATS,
    MESSAGES,
    PROFILE,
    MY_PROFILE,
    FRIENDS,
    CHAT_CREATION,
    SETTINGS
};

enum ROLE {
    VIEWER,
    PARTICIPANT,
    MODERATOR,
    ADMIN
};

enum MESSAGE_TYPE {
    USER_MESSAGE,
    SYSTEM_MESSAGE
};

struct UserChatMember {
    UserChatMember(int id, QString username, QImage avatar, ROLE role)
            : id(id), username(std::move(username)),
              avatar(std::move(avatar)), role(role) {}

    int id;
    QString username;
    QImage avatar;
    ROLE role;
};

struct ChatInfo {
    ChatInfo(int id, QString name, QImage avatar = QImage(":chatDefaultImage"), bool group = false,
             int countMembers = 2, ROLE role = PARTICIPANT)
            : id(id), name(std::move(name)), avatar(std::move(avatar)), group(group), countMembers(countMembers),
              role(role) {}

    int id;
    QString name;
    QImage avatar;
    bool group;
    int countMembers;
    ROLE role;
};

struct MessageInfo {
    MessageInfo(int messageId, QString content, QString time, MESSAGE_TYPE type, int chatId, int userId,
                int replyId = -1, QString username = nullptr, QImage avatar = QImage(":chatDefaultImage"))
            : chatId(chatId), userId(userId), replyId(replyId), messageId(messageId), username(std::move(username)),
              time(std::move(time)), avatar(std::move(avatar)), content(std::move(content)), type(type) {}

    int messageId;
    QString content;
    QString time;
    MESSAGE_TYPE type;
    int chatId;
    int userId;
    int replyId;
    QString username;
    QImage avatar;
};

#endif //CHATDATABASEPROJECT_DEFINES_H
