#ifndef CHATDATABASEPROJECT_SQLINTERFACE_H
#define CHATDATABASEPROJECT_SQLINTERFACE_H

#include "MainWindow.h"
#include "Defines.h"

// Create a database connection session
// Errors are handled by the function
void dbConnect();

// Close the default database connection session
void dbClose();

QList<ChatInfo> sqlLoadChats(int userID);

QList<MessageInfo> sqlLoadMessages(int chatID);

UserInfo sqlLoadProfile(const QString &username);

QList<UserInfo> sqlLoadFriends(int userID);

QList<UserInfo> sqlLoadIncomingRequests(int userID);

QList<UserInfo> sqlLoadOutgoingRequests(int userId);

QList<UserInfo> sqlPeopleInSearch(const QString &substring);

int sqlSendMessage(const MessageInfo &message);

void sqlMessageEdited(int messageId, const QString &newContent);

void sqlDeleteMessage(int messageId);

void sqlLeaveChat(int userId, int chatId);

void sqlRemoveChatMember(int userId, int chatId);

void sqlChangeRole(int userId, int chatId, int newRole);

void sqlUpdateChatAvatar(int chatId, QImage &newAvatar);

void sqlChangeChatName(int chatId, const QString &newName);

void sqlAcceptFriendRequest(int currentUserId, int newFriendId);

void sqlDeclineFriendRequest(int currentUserId, int notFriendId);

void sqlCancelFriendRequest(int currentUserId, int notFriendId);

void sqlSendFriendRequest(int userId, int targetUser);

void sqlRemoveFriend(int userId, int friendId);

#endif //CHATDATABASEPROJECT_SQLINTERFACE_H