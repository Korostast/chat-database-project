#include "SqlInterface.h"
#include "MainWindow.h"
#include "Defines.h"

QList<ChatInfo> sqlLoadChats(int userId) {
    ChatInfo chat1(0, "Самая первая обычная беседа", QImage(":chatDefaultImage"), true, 0);
    ChatInfo chat2(1, "Админская", QImage(":chatDefaultImage"), true, 0, ADMIN);
    ChatInfo chat3(2, "Зрительская", QImage(":chatDefaultImage"), true, 0, VIEWER);
    ChatInfo chat4(3, "Модераторская", QImage(":chatDefaultImage"), true, 0, MODERATOR);
    ChatInfo chat5(4, "Личная беседа (не работает имя ещё)", QImage(":chatDefaultImage"), false);
    ChatInfo chat6(5, "Личная беседа", QImage(":chatDefaultImage"), false);

    return QList<ChatInfo>({chat1, chat2, chat3, chat4, chat5, chat6});
}

QList<MessageInfo> sqlLoadMessages(int chatId) {
    MessageInfo message1(0, "Hello world!", "2021-03-31 22:10",
                         USER_MESSAGE, 0, 10, -1, "Korostast");
    MessageInfo message2(1, "Hello world!", "2021-03-31 22:10",
                         USER_MESSAGE, 0, 10, -1, "Korostast");
    QString test("Он белый\n"
                 "Пушистый\n"
                 "Мягкий\n"
                 "Падает красиво\n"
                 "Особенно когда в темноте, медленно-медленно и хлопьями, в ресницах застревающими\n"
                 "Он безумно красивый\n"
                 "И из него можно сделать снежок и запустить в какого-нибудь очень хорошего человека");
    MessageInfo message3(2, test, "2021-03-31 23:59",
                         USER_MESSAGE, 0, 10, -1, "Korostast");
    QString test2("Он белый"
                  "Пушистый"
                  "Мягкий"
                  "Падает красиво"
                  "Особенно когда в темноте, медленно-медленно и хлопьями, в ресницах застревающими"
                  "Он безумно красивый"
                  "И из него можно сделать снежок и запустить в какого-нибудь очень хорошего человека");
    MessageInfo message4(3, test2, "2021-03-31 23:59",
                         USER_MESSAGE, 0, 10, -1, "Korostast");
    QString test3(
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaa");
    MessageInfo message5(4, test3, "2021-03-31 23:59",
                         USER_MESSAGE, 0, 10, -1, "Korostast");

    return QList<MessageInfo>({message1, message2, message3, message4, message5});
}

QList<UserChatMember> sqlLoadChatMembers(int chatId) {
    UserChatMember user1(0, "Lalala", QImage(":chatDefaultImage"), PARTICIPANT);
    UserChatMember user2(1, "Another one", QImage(":chatDefaultImage"), VIEWER);
    UserChatMember user3(2, "Second", QImage(":chatDefaultImage"), MODERATOR);

    return QList<UserChatMember>({user1, user2, user3});
}

int sqlGetPersonId(int chatId, int userId) {
    return 0;
}

UserInfo sqlLoadProfile(int userId) {
    return UserInfo(10, "Someone", QImage(":chatDefaultImage"), "Sus status", nullptr, nullptr, nullptr, nullptr);
}

QList<UserInfo> sqlLoadFriends(int userId) {
    UserInfo user1(0, "Lalala", QImage(":chatDefaultImage"));
    UserInfo user2(1, "Another one", QImage(":chatDefaultImage"));
    UserInfo user3(2, "Second", QImage(":chatDefaultImage"));

    return QList<UserInfo>({user1, user2, user3});
}

QList<UserInfo> sqlLoadIncomingRequests(int userId) {
    UserInfo user1(3, "This_is_request", QImage(":chatDefaultImage"));
    UserInfo user2(4, "Kriper2003", QImage(":chatDefaultImage"));

    return QList<UserInfo>({user1, user2});
}

QList<UserInfo> sqlLoadOutcomingRequests(int userId) {
    UserInfo user1(5, "Masha", QImage(":chatDefaultImage"));
    UserInfo user2(6, "Java", QImage(":chatDefaultImage"));

    return QList<UserInfo>({user1, user2});
}

QList<UserInfo> sqlPeopleInSearch(const QString& substring) {
    UserInfo user1(0, "Lalala", QImage(":chatDefaultImage"));
    UserInfo user2(1, "Another one", QImage(":chatDefaultImage"));
    UserInfo user3(2, "Second", QImage(":chatDefaultImage"));
    UserInfo user4(3, "This_is_request", QImage(":chatDefaultImage"));
    UserInfo user5(4, "Kriper2003", QImage(":chatDefaultImage"));

    return QList<UserInfo>({user1, user2, user3, user4, user5});
}

int sqlSendMessage(const MessageInfo& message) {
    return 0;
}

void sqlMessageEdited(int messageId, const QString &newContent) {

}

void sqlDeleteMessage(int messageId) {

}

void sqlLeaveChat(int userId, int chatId) {

}

void sqlRemoveChatMember(int userId, int chatId) {

}

void sqlChangeRole(int userId, int chatId, int newRole) {

}

void sqlUpdateChatAvatar(int chatId, QImage& newAvatar) {

}

void sqlChangeChatName(int chatId, const QString &newName) {

}

void sqlAcceptFriendRequest(int currentUserId, int newFriendId) {

}

void sqlDeclineFriendRequest(int currentUserId, int notFriendId) {

}

void sqlCancelFriendRequest(int currentUserId, int notFriendId) {

}

void sqlSendFriendRequest(int userId, int targetUser) {

}

void sqlRemoveFriend(int userId, int friendId) {

}

int sqlCreateChat(int adminId, const QString &chatName, const QImage &avatar, const std::vector<int> &participants) {
    return 99;
}
