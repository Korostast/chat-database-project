#include "SqlInterface.h"
#include "MainWindow.h"
#include "Defines.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDateTime>

QSqlException::QSqlException(std::string message) : msg(std::move(message)) {}

const char *QSqlException::what() const noexcept {
    return msg.c_str();
}

void dbConnect(const QString &dbName) {
    QFile qFile("db_access.txt");
    if (!qFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Could not find 'db_access.txt' file with access credentials";
        exit(100);
    }
    qDebug() << "Found database access credentials";
    QTextStream qTextStream(&qFile);

    QSqlDatabase db = QSqlDatabase::addDatabase(qTextStream.readLine());
    db.setHostName(qTextStream.readLine());
    db.setDatabaseName(dbName);
    db.setUserName(qTextStream.readLine());
    db.setPassword(qTextStream.readLine());

    if (!db.open()) {
        qCritical()
                << "Could not connect to the database\nPlease check your access credentials\n";
        exit(100);
    }
    qDebug() << "Connected to database" << dbName;
}

void dbClose() {
    QSqlDatabase::database().close();
    qDebug() << "Database connection closed";
}

UserInfo sqlRegister(const QString &username, const QString &email, const QString &password) {
    qDebug() << "Trying to register" << username;
    QSqlQuery q;
    QString qStr;

    // Check if user with such username exists
    qStr = QString("call existsUser('%1')")
            .arg(username);

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
    if (q.next())
        throw QSqlException("User with this username already exists");

    // Check if user with such email exists
    qStr = QString("call existsUser('%1')")
            .arg(email);

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
    if (q.next())
        throw QSqlException("User with this email already exists");

    // Register the account
    qStr = QString("call registerAccount('%1','%2','%3')")
            .arg(username)
            .arg(password)
            .arg(email);

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
    if (!q.next())
        throw QSqlException("Registration failed");
    UserInfo result(q.value("id").toInt(),
                    q.value("username").toString(),
                    QImage::fromData(q.value("thumbnail").toByteArray()),
                    q.value("profile_status").toString(),
                    q.value("email").toString());

    return result;
}

UserInfo sqlAuthenticate(const QString &password, const QString &emailOrUsername) {
    qDebug() << "Trying to authenticate" << emailOrUsername;
    QSqlQuery q;
    QString qStr;

    // Check if user exists
    qStr = QString("call existsUser('%1')")
            .arg(emailOrUsername);

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
    if (!q.next())
        throw QSqlException("User with this username or email does not exist");
    int userID = q.value(0).toInt();

    // Check credentials
    qStr = QString("call authenticateUser(%1, '%2')")
            .arg(userID)
            .arg(password);

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
    if (!q.next())
        throw QSqlException("The password is incorrect. Try again");
    UserInfo result(q.value("id").toInt(),
                    q.value("username").toString(),
                    QImage::fromData(q.value("thumbnail").toByteArray()),
                    q.value("profile_status").toString(),
                    q.value("email").toString());

    return result;
}

QList<QString> sqlLoadDatabaseList() {
    qDebug() << "Loading database list from 'information_schema'";

    QSqlQuery q;
    if (!q.exec(
            "select SCHEMA_NAME from information_schema.SCHEMATA where SCHEMA_NAME not in ('information_schema','mysql','performance_schema','sys')"))
        qWarning() << q.lastError().databaseText();

    QList<QString> result;
    while (q.next())
        result.append(q.value(0).toString());

    return result;
}

QList<PersonalChatInfo> sqlLoadPersonalChats(int userID) {
    qDebug() << "Loading personal chats for user" << userID;
    QString qStr;
    QSqlQuery q;

    qStr = QString("call getPersonalChats(%1)")
            .arg(userID);

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<PersonalChatInfo> result;
    while (q.next())
        result.append(PersonalChatInfo(q.value(0).toInt(),
                                       q.value(1).toInt(),
                                       q.value(2).toString(),
                                       QImage::fromData(q.value(3).toByteArray())
                      )
        );

    return result;
}

QList<GroupChatInfo> sqlLoadGroupChats(int userID) {
    qDebug() << "Loading group chats for user" << userID;
    QString qStr;
    QSqlQuery q;

    qStr = QString("call getGroupChats(%1)")
            .arg(userID);

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<GroupChatInfo> result;
    while (q.next())
        result.append(GroupChatInfo(q.value(0).toInt(),
                                    q.value(1).toString(),
                                    QImage::fromData(q.value(2).toByteArray()),
                                    q.value(3).toInt(),
                                    ROLE(q.value(4).toInt())
                      )
        );

    return result;
}

QList<MessageInfo> sqlLoadMessages(int chatID) {
    qDebug() << "Loading messages for chatID =" << chatID;

    QString qStr = QString("call getMessages(%1)")
            .arg(chatID);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<MessageInfo> result;
    while (q.next())
        result.append(MessageInfo(q.value(0).toInt(),
                                  q.value(1).toString(),
                                  q.value(2).toDateTime().toString("yyyy-MM-dd HH:mm:ss"),
                                  MESSAGE_TYPE(q.value(3).toInt()),
                                  q.value(4).toInt(),
                                  q.value(5).toInt(),
                                  q.value(6).toString()
                      )
        );

    return result;
}

QList<MessageInfo> sqlLoadSearchedMessages(int chatID, QString &request) {
    qDebug() << "Searching for messages in" << chatID << "with pattern" << request;

    QString qStr = QString("call searchMessage(%1, '%2')")
            .arg(chatID)
            .arg(request);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<MessageInfo> result;
    while (q.next())
        result.append(MessageInfo(q.value(0).toInt(),
                                  q.value(1).toString(),
                                  q.value(2).toDateTime().toString("yyyy-MM-dd HH:mm:ss"),
                                  MESSAGE_TYPE(q.value(3).toInt()),
                                  q.value(4).toInt(),
                                  q.value(5).toInt(),
                                  q.value(6).toString()
                      )
        );

    return result;
}

QList<UserChatMember> sqlLoadChatMembers(int chatID) {
    qDebug() << "Loading members of " << chatID;

    QString qStr = QString("call getChatMembers(%1)")
            .arg(chatID);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<UserChatMember> result;

    while (q.next())
        result.append(UserChatMember(q.value(0).toInt(),
                                     q.value(1).toString(),
                                     QImage::fromData(q.value(2).toByteArray()),
                                     ROLE(q.value(3).toInt())
                      )
        );

    return result;
}

UserInfo sqlLoadProfile(int userID) {
    qDebug() << "Loading profile for userID = " << userID;

    QString qStr = QString("call getProfile(%1)")
            .arg(userID);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

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

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

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

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<UserInfo> result;
    while (q.next())
        result.append(UserInfo(q.value(0).toInt(),
                               q.value(1).toString(),
                               QImage::fromData(q.value(2).toByteArray())
                      )
        );

    return result;
}

QList<UserInfo> sqlLoadOutgoingRequests(int userID) {
    qDebug() << "Loading outgoing friend requests for userID =" << userID;

    QString qStr = QString("call getOutgoingRequests(%1)")
            .arg(userID);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<UserInfo> result;
    while (q.next())
        result.append(UserInfo(q.value(0).toInt(),
                               q.value(1).toString(),
                               QImage::fromData(q.value(2).toByteArray())
                      )
        );

    return result;
}

QList<std::pair<UserInfo, QString> > sqlSearchUsers(int userID, const QString &substring) {
    qDebug() << "Searching for people for userID =" << userID;

    QString qStr = QString("call searchUsers(%1, '%2')")
            .arg(userID)
            .arg(substring);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<std::pair<UserInfo, QString> > result;
    while (q.next())
        result.append(std::make_pair(UserInfo(q.value(0).toInt(),
                                              q.value(1).toString(),
                                              QImage::fromData(q.value(2).toByteArray())), q.value(3).toString()
                      )
        );

    return result;
}

int sqlSendMessage(const MessageInfo &message) {
    qDebug() << "Sending message from" << message.userID << "to chat" << message.chatID;

    QString qStr = QString("call sendMessage(%1,%2,'%3','%4')")
            .arg(message.chatID)
            .arg(message.userID)
            .arg(message.content)
            .arg(message.type ? "system_info" : "user_message");

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    if (!q.next())
        qWarning() << "Failed to send the message";

    return q.value(0).toInt();
}

void sqlMessageEdited(int messageID, const QString &newContent) {
    qDebug() << "Editing message with messageID =" << messageID;

    QString qStr = QString("call editMessage(%1,'%2')")
            .arg(messageID)
            .arg(newContent);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlDeleteMessage(int messageID) {
    qDebug() << "Deleting message with messageID =" << messageID;

    QString qStr = QString("call deleteMessage(%1)")
            .arg(messageID);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlDeleteMessagesByPattern(int chatID, const QString &pattern) {
    qDebug() << "Deleting messages in" << chatID << "witt pattern" << pattern;

    QString qStr = QString("call deleteMessageByPattern(%1, '%2')")
            .arg(chatID)
            .arg(pattern);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlLeaveChat(int userID, int chatID) {
    qDebug() << "User with userID =" << userID << "leaves chat with chatID =" << chatID;

    QString qStr = QString("call leaveChat(%1,%2)")
            .arg(chatID)
            .arg(userID);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlRemoveChatMember(int userID, int chatID) {
    qDebug() << "Redirected to sqlRemoveChatMember";
    sqlLeaveChat(userID, chatID);
}

void sqlChangeRole(int chatID, int userID, int newRole) {
    qDebug() << "Setting role of userID =" << userID << "in chatID =" << chatID << "to" << newRole;

    QString qStr = QString("call updateRole(%1,%2,%3)")
            .arg(chatID)
            .arg(userID)
            .arg(newRole + 1);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlUpdateChat(int chatID, const QString &newName, const QImage &newAvatar) {

}

void sqlAcceptFriendRequest(int currentUserID, int newFriendID) {
    qDebug() << "userID =" << currentUserID << "accepts friend request from" << newFriendID;

    QString qStr = QString("call acceptRequest(%1,%2)")
            .arg(newFriendID)
            .arg(currentUserID);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlDeclineFriendRequest(int currentUserID, int notFriendID) {
    qDebug() << "Redirected to sqlCancelFriendRequest";
    sqlCancelFriendRequest(notFriendID, currentUserID);
}

void sqlCancelFriendRequest(int currentUserID, int notFriendID) {
    qDebug() << "Deleting friend request between" << currentUserID << "and" << notFriendID;

    QString qStr = QString("call deleteRequest(%1,%2)")
            .arg(currentUserID)
            .arg(notFriendID);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlSendFriendRequest(int userID, int requestedID) {
    qDebug() << "Sending friend request from" << userID << "to" << requestedID;

    QString qStr = QString("call sendFriendRequest(%1,%2)")
            .arg(userID)
            .arg(requestedID);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlRemoveFriend(int userID, int friendID) {
    qDebug() << userID << "unfriended" << friendID;

    QString qStr = QString("call deleteFriend(%1,%2)")
            .arg(userID)
            .arg(friendID);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

int
sqlCreateGroupChat(int creatorID, const QString &chatName, const QImage &avatar, const std::vector<int> &participants) {
    qDebug() << "Creating new chat with the name" << chatName;

    // Step 1 - Create new entry in 'Chat'
    QString qStr = QString("call createGroupChat('%1', '%2')")
            .arg(chatName)
            .arg(QString(QByteArray::fromRawData((const char *) avatar.bits(), avatar.sizeInBytes())).replace("'",
                                                                                                              "''"));

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    q.next();
    int chatID = q.value(0).toInt();

    // Step 3 - Add creator to the chat
    sqlAddMembers(chatID, {creatorID});

    // Step 4 - Give the creator 'admin' role
    sqlChangeRole(chatID, creatorID, ADMIN);

    // Step 5 - Add all the invited members to the chat
    sqlAddMembers(chatID, participants);

    return chatID;
}

void sqlAddMembers(int chatID, const std::vector<int> &participants) {
    qDebug() << "Adding new members to" << chatID;
    QString qStr;
    QSqlQuery q;

    for (auto now: participants) {
        qStr = QString("call joinChat(%1, %2)")
                .arg(chatID)
                .arg(now);

        if (!q.exec(qStr))
            qWarning() << q.lastError().databaseText();
    }
}

void sqlUpdateProfile(int userID, const QString &firstname, const QString &lastname, const QString &phoneNumber,
                      const QString &status, const QImage &avatar) {

}

bool sqlAdminAuth(const QString &password) {
    return true;
}

void sqlCreateDatabase(const QString &databaseName) {
    qWarning() << "CREATING DATABASE" << databaseName;
    QSqlQuery q;
    QString qStr;

    qStr = QString("create database %1")
            .arg(databaseName);

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    sqlChooseDatabase(databaseName);

    QFile qFile("db_create.sql");
    if (!qFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Could not find 'db_create.sql'";
        exit(100);
    }

    qStr = qFile.readAll();

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlDeleteDatabase(const QString &databaseName) {
    qWarning() << "DELETING DATABASE" << databaseName;

    QString qStr = QString("drop database %1")
            .arg(databaseName);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlChooseDatabase(const QString &databaseName) {
    dbClose();
    dbConnect(databaseName);
}