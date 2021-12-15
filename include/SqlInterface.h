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

UserInfo sqlLoadProfile(int userID);

QList<UserInfo> sqlLoadFriends(int userID);

QList<UserInfo> sqlLoadIncomingRequests(int userID);

QList<UserInfo> sqlLoadOutgoingRequests(int userID);

QList<UserInfo> sqlPeopleInSearch(const QString &substring);

int sqlSendMessage(const MessageInfo &message);

void sqlMessageEdited(int messageID, const QString &newContent);

void sqlDeleteMessage(int messageID);

void sqlLeaveChat(int userID, int chatID);

void sqlRemoveChatMember(int userID, int chatID);

void sqlChangeRole(int userID, int chatID, int newRole);

void sqlUpdateChatAvatar(int chatID, QImage &newAvatar);

void sqlChangeChatName(int chatID, const QString &newName);

void sqlAcceptFriendRequest(int currentUserID, int newFriendID);

void sqlDeclineFriendRequest(int currentUserID, int notFriendID);

void sqlCancelFriendRequest(int currentUserID, int notFriendID);

void sqlSendFriendRequest(int userID, int targetUser);

void sqlRemoveFriend(int userID, int friendID);

int sqlGetPersonID(int chatID, int userID);

int sqlCreateChat(int chatID, const QString& chatName, const QImage& thumbnail, const std::vector<int>& users);

QList<UserChatMember> sqlLoadChatMembers(int chatID);

void sqlAddMembers(int chatID, const std::vector<int>& users);

#endif //CHATDATABASEPROJECT_SQLINTERFACE_H