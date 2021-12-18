#ifndef CHATDATABASEPROJECT_SQLINTERFACE_H
#define CHATDATABASEPROJECT_SQLINTERFACE_H

#include "MainWindow.h"
#include "Defines.h"

class QSqlException : std::exception {
    std::string msg;
public:
    explicit QSqlException(std::string message);

    [[nodiscard]] const char *what() const noexcept override;
};

// Create a database connection session
void dbConnect(const QString &dbName);

// Close the default database connection session
void dbClose();

// Returns the name of the currently used database
QString dbName();

UserInfo sqlRegisterUser(const QString &username, const QString &email, const QString &password);

UserInfo sqlAuthenticateUser(const QString &password, const QString &emailOrUsername);

QList<QString> sqlLoadDatabaseList();

QList<PersonalChatInfo> sqlLoadPersonalChats(int userID);

QList<GroupChatInfo> sqlLoadGroupChats(int userID);

QList<MessageInfo> sqlLoadMessages(int chatID);

QList<MessageInfo> sqlLoadSearchedMessages(int chatID, QString &pattern);

QList<UserChatMember> sqlLoadChatMembers(int chatID);

UserInfo sqlLoadProfile(int userID);

QList<UserInfo> sqlLoadFriends(int userID);

QList<UserInfo> sqlLoadIncomingRequests(int userID);

QList<UserInfo> sqlLoadOutgoingRequests(int userID);

QList<std::pair<UserInfo, QString> > sqlSearchUsersByPattern(int userID, const QString &pattern);

int sqlSendMessage(const MessageInfo &message);

void sqlEditMessage(int messageID, const QString &newContent);

void sqlDeleteMessage(int messageID);

void sqlDeleteMessagesByPattern(int chatID, const QString &pattern);

void sqlLeaveChat(int userID, int chatID);

void sqlRemoveChatMember(int userID, int chatID);

void sqlChangeRole(int chatID, int userID, int newRole);

void sqlUpdateChat(int chatID, const QString &newName, const QImage &newAvatar);

void sqlAcceptFriendRequest(int userID, int senderID);

void sqlDeclineFriendRequest(int userID, int declinedID);

void sqlCancelFriendRequest(int senderID, int requestedID);

void sqlSendFriendRequest(int userID, int requestedID);

void sqlRemoveFriend(int userID, int friendID);

int sqlCreateGroupChat(int creatorID, const QString &chatName,
                       const QImage &avatar, const std::vector<int> &participants);

void sqlAddMembers(int chatID, const std::vector<int> &participants);

bool sqlAdminAuth(const QString &password);

void sqlCreateDatabase(const QString &databaseName);

void sqlDeleteDatabase(const QString &databaseName);

void sqlChooseDatabase(const QString &databaseName);

void sqlUpdateProfile(int userId, const QString &firstname, const QString &lastname, const QString &phoneNumber,
                      const QString &status, const QImage &avatar);

void sqlUpdateAccount(int userId, const QString &username, const QString &password, const QString &email);

#endif //CHATDATABASEPROJECT_SQLINTERFACE_H