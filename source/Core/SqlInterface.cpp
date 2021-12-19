#include "SqlInterface.h"
#include "MainWindow.h"
#include "Defines.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QDateTime>
#include <QBuffer>

QSqlException::QSqlException(std::string message) : msg(std::move(message)) {}

const char *QSqlException::what() const noexcept {
    return msg.c_str();
}

QString insertString(const QString &s) {
    if (s.isEmpty())
        return "null";
    else
        return QString("'%1'").arg(s);
}

QByteArray imageToBase64(const QImage &img) {
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "PNG");
    return ba.toBase64();
}

QImage base64ToImage(const QByteArray &ba) {
    return QImage::fromData(QByteArray::fromBase64(ba), "png");
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
        qCritical() << "Could not connect to the database\n"
                       "Please check your access credentials\n";
        exit(100);
    }
    qDebug() << "Connected to database" << dbName;
}

void dbClose() {
    QSqlDatabase::database().close();
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    qDebug() << "Database connection closed";
}

QString dbName() {
    return QSqlDatabase::database().databaseName();
}

UserInfo sqlRegisterUser(const QString &username, const QString &email, const QString &password) {
    qDebug() << "Registering" << username;
    QSqlQuery q;
    QString qStr;

    // Check if user with such username exists
    qStr = QString("call existsUser(%1)")
            .arg(insertString(username));
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
    if (q.next())
        throw QSqlException("User with this username already exists");

    // Check if user with such email exists
    qStr = QString("call existsUser(%1)")
            .arg(insertString(email));
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
    if (q.next())
        throw QSqlException("User with this email already exists");

    // Register the account
    qStr = QString("call registerUser(%1, %2, %3)")
            .arg(insertString(username))
            .arg(insertString(password))
            .arg(insertString(email));
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
    if (!q.next())
        throw QSqlException("Registration failed");

    UserInfo result(q.value("id").toInt(),
                    q.value("username").toString(),
                    QImage::fromData(q.value("avatar").toByteArray()));

    return result;
}

UserInfo sqlAuthenticateUser(const QString &password, const QString &emailOrUsername) {
    qDebug() << "Authenticating" << emailOrUsername;
    QSqlQuery q;
    QString qStr;

    // Check if user exists
    qStr = QString("call existsUser(%1)")
            .arg(insertString(emailOrUsername));
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
    if (!q.next())
        throw QSqlException("User with this username or email does not exist");

    int userID = q.value("id").toInt();

    // Check credentials
    qStr = QString("call authenticateUser(%1, %2)")
            .arg(userID)
            .arg(insertString(password));

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
    if (!q.next())
        throw QSqlException("The password is incorrect. Try again");

    UserInfo result(q.value("id").toInt(),
                    q.value("username").toString(),
                    QImage::fromData(q.value("avatar").toByteArray()));

    return result;
}

QList<QString> sqlLoadDatabaseList() {
    qDebug() << "Loading database list from 'information_schema'";
    QSqlQuery q;

    if (!q.exec("select SCHEMA_NAME "
                "from information_schema.SCHEMATA "
                "where SCHEMA_NAME not in ('information_schema','mysql','performance_schema','sys')"))
        qWarning() << q.lastError().databaseText();

    QList<QString> result;
    while (q.next())
        result.append(q.value("SCHEMA_NAME").toString());

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
        result.append(PersonalChatInfo(q.value("id").toInt(),
                                       q.value("friend_id").toInt(),
                                       q.value("friend_username").toString(),
                                       QImage::fromData(q.value("friend_avatar").toByteArray())
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
        result.append(GroupChatInfo(q.value("id").toInt(),
                                    q.value("chat_name").toString(),
                                    base64ToImage(q.value("avatar").toByteArray()),
                                    q.value("member_count").toInt(),
                                    ROLE(q.value("user_role").toInt())
                      )
        );

    return result;
}

QList<MessageInfo> sqlLoadMessages(int chatID) {
    qDebug() << "Loading messages in chat" << chatID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call getMessages(%1)")
            .arg(chatID);
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<MessageInfo> result;
    while (q.next())
        result.append(MessageInfo(q.value("id").toInt(),
                                  q.value("content").toString(),
                                  q.value("sent_datetime").toDateTime().toString("yyyy-MM-dd HH:mm:ss"),
                                  MESSAGE_TYPE(q.value("content_type").toInt()),
                                  q.value("chat_id").toInt(),
                                  q.value("user_id").toInt(),
                                  q.value("username").toString(),
                                  base64ToImage(q.value("avatar").toByteArray())
                      )
        );

    return result;
}

QList<MessageInfo> sqlLoadSearchedMessages(int chatID, QString &pattern) {
    qDebug() << "Searching for messages in chat" << chatID << "with pattern" << pattern;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call searchMessagesByPattern(%1, %2)")
            .arg(chatID)
            .arg(insertString(pattern));
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<MessageInfo> result;
    while (q.next())
        result.append(MessageInfo(q.value("id").toInt(),
                                  q.value("content").toString(),
                                  q.value("sent_datetime").toDateTime().toString("yyyy-MM-dd HH:mm:ss"),
                                  MESSAGE_TYPE(q.value("content_type").toInt()),
                                  q.value("chat_id").toInt(),
                                  q.value("user_id").toInt(),
                                  q.value("username").toString(),
                                  base64ToImage(q.value("avatar").toByteArray())
                      )
        );

    return result;
}

QList<UserChatMember> sqlLoadChatMembers(int chatID) {
    qDebug() << "Loading members of chat" << chatID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call getChatMembers(%1)")
            .arg(chatID);
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<UserChatMember> result;
    while (q.next())
        result.append(UserChatMember(q.value("id").toInt(),
                                     q.value("username").toString(),
                                     base64ToImage(q.value("avatar").toByteArray()),
                                     ROLE(q.value("user_role").toInt())
                      )
        );

    return result;
}

UserInfo sqlLoadProfile(int userID) {
    qDebug() << "Loading profile for user" << userID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call getProfile(%1)")
            .arg(userID);
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
    if (!q.next()) {
        qWarning() << "No profile found for" << userID;
        return UserInfo();
    }

    UserInfo result(q.value("id").toInt(),
                    q.value("username").toString(),
                    base64ToImage(q.value("avatar").toByteArray()),
                    q.value("status").toString(),
                    q.value("email").toString(),
                    q.value("phone_number").toString(),
                    q.value("first_name").toString(),
                    q.value("last_name").toString()
    );

    return result;
}


QList<UserInfo> sqlLoadFriends(int userID) {
    qDebug() << "Loading friends for user" << userID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call getFriends(%1)")
            .arg(userID);
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<UserInfo> result;
    while (q.next())
        result.append(UserInfo(q.value("id").toInt(),
                               q.value("username").toString(),
                               base64ToImage(q.value("avatar").toByteArray()))
        );

    return result;
}

QList<UserInfo> sqlLoadIncomingRequests(int userID) {
    qDebug() << "Loading incoming friend requests for user" << userID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call getIncomingRequests(%1)")
            .arg(userID);
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<UserInfo> result;
    while (q.next())
        result.append(UserInfo(q.value("id").toInt(),
                               q.value("username").toString(),
                               base64ToImage(q.value("avatar").toByteArray())
                      )
        );

    return result;
}

QList<UserInfo> sqlLoadOutgoingRequests(int userID) {
    qDebug() << "Loading outgoing friend requests for user" << userID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call getOutgoingRequests(%1)")
            .arg(userID);
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<UserInfo> result;
    while (q.next())
        result.append(UserInfo(q.value("id").toInt(),
                               q.value("username").toString(),
                               base64ToImage(q.value("avatar").toByteArray())
                      )
        );

    return result;
}

QList<std::pair<UserInfo, QString> > sqlSearchUsersByPattern(int userID, const QString &pattern) {
    qDebug() << "Searching for users with pattern" << pattern;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call searchUsersByPattern(%1, %2)")
            .arg(userID)
            .arg(insertString(pattern));
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    QList<std::pair<UserInfo, QString> > result;
    while (q.next())
        result.append(std::make_pair(UserInfo(q.value("id").toInt(),
                                              q.value("username").toString(),
                                              base64ToImage(q.value("avatar").toByteArray())),
                                     q.value("relation").toString()
                      )
        );

    return result;
}

int sqlSendMessage(const MessageInfo &message) {
    qDebug() << "Sending message from user" << message.userID << "to chat" << message.chatID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call sendMessage(%1, %2, %3, %4)")
            .arg(message.chatID)
            .arg(message.userID)
            .arg(insertString(message.content))
            .arg(message.type ? "system_info" : "user_message");
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
    if (!q.next())
        qWarning() << "Failed to send the message";

    return q.value("id").toInt();
}

void sqlEditMessage(int messageID, const QString &newContent) {
    qDebug() << "Editing message" << messageID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call editMessage(%1, %2)")
            .arg(messageID)
            .arg(insertString(newContent));
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlDeleteMessage(int messageID) {
    qDebug() << "Deleting message" << messageID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call deleteMessage(%1)")
            .arg(messageID);
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlDeleteMessagesByPattern(int chatID, const QString &pattern) {
    qDebug() << "Deleting messages in chat" << chatID << "witt pattern" << pattern;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call deleteMessageByPattern(%1, %2)")
            .arg(chatID)
            .arg(insertString(pattern));
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlLeaveChat(int userID, int chatID) {
    qDebug() << "Removing user" << userID << "from chat" << chatID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call leaveChat(%1, %2)")
            .arg(chatID)
            .arg(userID);
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlRemoveChatMember(int userID, int chatID) {
    qDebug() << "Redirecting to sqlLeaveChat";
    sqlLeaveChat(userID, chatID);
}

void sqlChangeRole(int chatID, int userID, int newRole) {
    qDebug() << "Setting role of user" << userID << "in chat" << chatID << "to" << newRole;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call updateRole(%1, %2, %3)")
            .arg(chatID)
            .arg(userID)
            .arg(newRole + 1);
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlAcceptFriendRequest(int userID, int senderID) {
    qDebug() << "User" << userID << "accepts request from user" << senderID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call acceptFriendRequest(%1, %2)")
            .arg(senderID)
            .arg(userID);
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlDeclineFriendRequest(int userID, int declinedID) {
    qDebug() << "Redirecting to sqlCancelFriendRequest";
    sqlCancelFriendRequest(declinedID, userID);
}

void sqlCancelFriendRequest(int senderID, int requestedID) {
    qDebug() << "Deleting friend request from user" << senderID << "to user" << requestedID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call deleteFriendRequest(%1, %2)")
            .arg(senderID)
            .arg(requestedID);
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlSendFriendRequest(int userID, int requestedID) {
    qDebug() << "Sending friend request from user" << userID << "to user" << requestedID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call sendFriendRequest(%1, %2)")
            .arg(userID)
            .arg(requestedID);
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlRemoveFriend(int userID, int friendID) {
    qDebug() << "User" << userID << "unfriends user" << friendID;
    QSqlQuery q;
    QString qStr;

    qStr = QString("call deleteFriend(%1,%2)")
            .arg(userID)
            .arg(friendID);
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

int sqlCreateGroupChat(int creatorID, const QString &chatName,
                       const QImage &avatar, const std::vector<int> &participants) {
    qDebug() << "Creating new chat with the name" << chatName;
    QSqlQuery q;
    QString qStr;

    // Step 1 - Create new entry in 'Chat'
    qStr = QString("call createGroupChat(%1, '%2')")
            .arg(insertString(chatName))
            .arg(imageToBase64(avatar));
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
    qDebug() << "Adding new members to chat" << chatID;
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

bool sqlAdminAuth(const QString &password) {
    return true;
}

void sqlCreateDatabase(const QString &databaseName) {
    qWarning() << "CREATING DATABASE" << databaseName;
    QSqlQuery q;
    QString qStr;

    // Step 1 - Create the database itself
    qStr = QString("create database %1")
            .arg(insertString(databaseName));
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();

    // Step 2 - Switch current connection the new database
    sqlChooseDatabase(databaseName);

    // Step 3 - Populate the database with tables, functions and triggers
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
            .arg(insertString(databaseName));

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlChooseDatabase(const QString &databaseName) {
    dbClose();
    dbConnect(databaseName);
}

void sqlUpdateChatName(int chatID, const QString &newName) {
    qDebug() << "Updating avatar of chat with id =" << chatID;

    QSqlQuery q;
    QString qStr;

    qStr = QString("call updateChatName(%1, %2)")
            .arg(chatID)
            .arg(insertString(newName));

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlUpdateChatAvatar(int chatID, const QImage &newAvatar) {
    qDebug() << "Updating avatar of chat with id =" << chatID;

    QSqlQuery q;
    QString qStr;

    qStr = QString("call updateChatAvatar(%1, '%2')")
            .arg(chatID)
            .arg(imageToBase64(newAvatar));

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlUpdateUsername(int userID, const QString &newUsername) {
    qDebug() << "Trying to change username of user with id =" << userID;

    QSqlQuery q;
    QString qStr;

    qStr = QString("call updateUsername(%1, %2)")
            .arg(userID)
            .arg(insertString(newUsername));
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlUpdatePassword(int userID, const QString &newPassword) {
    qDebug() << "Trying to change password of user with id =" << userID;

    QSqlQuery q;
    QString qStr;

    // Check that old password and new password do not match
    qStr = QString("call checkPassword(%1, %2)")
            .arg(userID)
            .arg(insertString(newPassword));

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
    if (q.value("result").toBool())
        throw QSqlException("Password is not changed because it cannot be the same as the old password");

    // Update password
    qStr = QString("call updatePassword(%1, %2)")
            .arg(userID)
            .arg(insertString(newPassword));

    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlUpdateAvatar(int userID, const QImage &newAvatar) {
    qDebug() << "Updating avatar of user with id =" << userID;

    QSqlQuery q;
    QString qStr;

    // Check that old password and new password do not match
    qStr = QString("call updateAvatar(%1, '%2')")
            .arg(userID)
            .arg(imageToBase64(newAvatar));
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlUpdateOther(int userID, const QString &newStatus, const QString &newPhoneNumber, const QString &newFirstName,
                    const QString &newLastName) {
    qDebug() << "Updating other information of user with id =" << userID;

    QSqlQuery q;
    QString qStr;

    // Check that old password and new password do not match
    qStr = QString("call updateOther(%1, %2, %3, %4, %5)")
            .arg(userID)
            .arg(insertString(newStatus),
                 insertString(newPhoneNumber),
                 insertString(newFirstName),
                 insertString(newLastName));
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}