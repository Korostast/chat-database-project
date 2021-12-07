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

#define AUTHENTICATION_PAGE 0
#define APP_PAGE 1

#define AUTHORIZATION_PAGE 0
#define REGISTRATION_PAGE 1

#define CHAT_LIST_PAGE 0
#define MESSAGES_PAGE 1
#define PROFILE_PAGE 2
#define FRIENDS_PAGE 3

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
    UserChatMember(int i, const QString &string, QImage image, ROLE role) {
        this->id = i;
        this->username = string;
        this->avatar = std::move(image);
        this->role = role;
    }

    int id;
    QString username;
    QImage avatar;
    ROLE role;
};

#endif //CHATDATABASEPROJECT_DEFINES_H
