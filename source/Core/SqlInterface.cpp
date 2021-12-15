#include "SqlInterface.h"
#include "MainWindow.h"
#include "Defines.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QDateTime>

void dbConnect() {
    QFile qFile("db_access.txt");
    if (!qFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Could not find 'db_access.txt' file with access credentials";
        exit(100);
    }
    qDebug() << "Found database access credentials";
    QTextStream qTextStream(&qFile);

    QSqlDatabase db = QSqlDatabase::addDatabase(qTextStream.readLine());
    db.setHostName(qTextStream.readLine());
    db.setDatabaseName(qTextStream.readLine());
    db.setUserName(qTextStream.readLine());
    db.setPassword(qTextStream.readLine());

    if (!db.open()) {
        qCritical()
                << "Could not connect to the database\nPlease check your internet connection, access credentials and database availability";
        exit(100);
    }
    qDebug() << "Database connection opened";
}

void dbClose() {
    QSqlDatabase::database().close();
    qDebug() << "Database connection closed";
}

QList<ChatInfo> sqlLoadChats(int userID) {
    qDebug() << "Loading chats for userID =" << userID;

    QString qStr = QString("call getChatList(%1)")
            .arg(userID);

    QSqlQuery q(qStr);
    q.exec();

    QList<ChatInfo> result;
    while (q.next())
        result.append(ChatInfo(q.value(0).toInt(),
                               q.value(1).toString(),
                               QImage::fromData(q.value(2).toByteArray()),
                               q.value(3).toBool(),
                               q.value(4).toInt(),
                               ROLE(q.value(5).toInt()))
        );

    return result;
}

QList<MessageInfo> sqlLoadMessages(int chatID) {
    qDebug() << "Loading messages for chatID =" << chatID;

    QString qStr = QString("call getMessages(%1)")
            .arg(chatID);

    QSqlQuery q(qStr);
    q.exec();

    QList<MessageInfo> result;
    while (q.next())
        result.append(MessageInfo(q.value(0).toInt(),
                                  q.value(1).toString(),
                                  q.value(2).toDateTime().toString("yyyy-MM-dd HH:mm:ss"),
                                  MESSAGE_TYPE(q.value(3).toInt()),
                                  q.value(4).toInt(),
                                  q.value(5).toInt(),
                                  q.value(6).toInt(),
                                  q.value(7).toString())
        );

    return result;
}

UserInfo sqlLoadProfile(int userID) {
    qDebug() << "Loading profile for userID = " << userID;

    QString qStr = QString("call getProfile(%1)")
            .arg(userID);

    QSqlQuery q(qStr);
    q.exec();

    if (!q.next()) {
        qWarning() << "No profile found for userID =" << userID;
        return UserInfo();
    }

    UserInfo result(q.value(0).toInt(),
                    q.value(1).toString(),
                    QImage::fromData(q.value(2).toByteArray()),
                    q.value(3).toString(),
                    q.value(4).toString(),
                    q.value(5).toString(),
                    q.value(6).toString(),
                    q.value(7).toString()
    );

    return result;
}

QList<UserInfo> sqlLoadFriends(int userID) {
    qDebug() << "Loading friends for userID =" << userID;

    QString qStr = QString("call getFriends(%1)")
            .arg(userID);

    QSqlQuery q(qStr);
    q.exec();

    QList<UserInfo> result;
    while (q.next())
        result.append(UserInfo(q.value(0).toInt(),
                               q.value(1).toString(),
                               QImage::fromData(q.value(2).toByteArray()))
        );

    return result;
}

QList<UserInfo> sqlLoadIncomingRequests(int userID) {
    qDebug() << "Loading incoming friend requests for userID =" << userID;

    QString qStr = QString("call getIncomingRequests(%1)")
            .arg(userID);

    QSqlQuery q(qStr);
    q.exec();

    QList<UserInfo> result;
    while (q.next())
        result.append(UserInfo(q.value(0).toInt(),
                               q.value(1).toString(),
                               QImage::fromData(q.value(2).toByteArray()))
        );

    return result;
}

QList<UserInfo> sqlLoadOutgoingRequests(int userID) {
    qDebug() << "Loading friends for userID =" << userID;

    QString qStr = QString("call getOutgoingRequests(%1)")
            .arg(userID);

    QSqlQuery q(qStr);
    q.exec();

    QList<UserInfo> result;
    while (q.next())
        result.append(UserInfo(q.value(0).toInt(),
                               q.value(1).toString(),
                               QImage::fromData(q.value(2).toByteArray()))
        );

    return result;
}

// TODO the function should receive userID as an argument to calculate relation status
QList<UserInfo> sqlPeopleInSearch(const QString &substring) {
    qDebug() << "Searching for users with username like -" << substring;

    QString qStr = QString("call searchUsers('%1')")
            .arg(substring);

    QSqlQuery q(qStr);
    q.exec();

    QList<UserInfo> result;
    while (q.next())
        result.append(UserInfo(q.value(0).toInt(),
                               q.value(1).toString(),
                               QImage::fromData(q.value(2).toByteArray()))
        );

    return result;
}

int sqlSendMessage(const MessageInfo &message) {
    qDebug() << "Sending message from" << message.userId << "to chat" << message.chatId;

    QString qStr = QString("call sendMessage(%1,%2,'%3',%4,%5)")
            .arg(message.chatId)
            .arg(message.userId)
            .arg(message.content)
            .arg(message.type)
            .arg(message.replyId);

    QSqlQuery q(qStr);
    q.exec();

    if (!q.next())
        qWarning() << "Failed to send the message";

    return q.value(0).toInt();
}

void sqlMessageEdited(int messageID, const QString &newContent) {
    qDebug() << "Editing message with messageID =" << messageID;

    QString qStr = QString("call editMessage(%1,'%2')")
            .arg(messageID)
            .arg(newContent);

    QSqlQuery q(qStr);
    q.exec();
}

void sqlDeleteMessage(int messageID) {
    qDebug() << "Deleting message with messageID =" << messageID;

    QString qStr = QString("call deleteMessage(%1)")
            .arg(messageID);

    QSqlQuery q(qStr);
    q.exec();
}

void sqlLeaveChat(int userID, int chatID) {
    qDebug() << "User with userID =" << userID << "leaves chat with chatID =" << chatID;

    QString qStr = QString("call leaveChat(%1,%2)")
            .arg(chatID)
            .arg(userID);

    QSqlQuery q(qStr);
    q.exec();
}

void sqlRemoveChatMember(int userID, int chatID) {
    qDebug() << "Redirected to sqlRemoveChatMember";
    sqlLeaveChat(userID, chatID);
}

void sqlChangeRole(int userID, int chatID, int newRole) {
    qDebug() << "Setting role of userID =" << userID << "in chatID =" << chatID << "to" << newRole;

    QString qStr = QString("call updateRole(%1,%2,%3)")
            .arg(chatID)
            .arg(userID)
            .arg(newRole);

    QSqlQuery q(qStr);
    q.exec();
}

void sqlUpdateChatAvatar(int chatID, QImage &newAvatar) {

}

void sqlChangeChatName(int chatID, const QString &newName) {

}

void sqlAcceptFriendRequest(int currentUserID, int newFriendID) {
    qDebug() << "userID =" << currentUserID << "accepts friend request from" << newFriendID;

    QString qStr = QString("call acceptRequest(%1,%1)")
            .arg(newFriendID)
            .arg(currentUserID);

    QSqlQuery q(qStr);
    q.exec();
}

void sqlDeclineFriendRequest(int currentUserID, int notFriendID) {
    qDebug() << "Redirected to sqlCancelFriendRequest";
}

void sqlCancelFriendRequest(int currentUserID, int notFriendID) {
    qDebug() << "Deleting friend request between" << currentUserID << "and" << notFriendID;

    QString qStr = QString("call deleteRequest(%1,%2)")
            .arg(currentUserID)
            .arg(notFriendID);

    QSqlQuery q(qStr);
    q.exec();
}

void sqlSendFriendRequest(int userID, int targetUser) {
    qDebug() << "Sending friend request from"<<userID<<"to"<<targetUser;

    QString qStr = QString("call sendFriendRequest(%1,%2)")
            .arg(userID)
            .arg(targetUser);

    QSqlQuery q(qStr);
    q.exec();
}

void sqlRemoveFriend(int userID, int friendID) {
    qDebug() <<userID<<"unfriended"<<friendID;

    QString qStr = QString("call deleteFriend(%1,%2)")
            .arg(userID)
            .arg(friendID);

    QSqlQuery q(qStr);
    q.exec();
}

int sqlCreateChat(int chatID, const QString &chatName, const QImage &thumbnail, const std::vector<int> &users) {
    return 0;
}

QList<UserChatMember> sqlLoadChatMembers(int chatID) {
    return QList<UserChatMember>{};
}

void sqlAddMembers(int chatID, const std::vector<int> &users) {

}