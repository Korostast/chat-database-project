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
    PersonalChatInfo chat5(4, 1, "Личная беседа (не работает имя ещё)", QImage(":chatDefaultImage"));
    PersonalChatInfo chat6(5, 2, "Личная беседа", QImage(":chatDefaultImage"));

    return QList<PersonalChatInfo>({chat5, chat6});
}

QList<GroupChatInfo> sqlLoadGroupChats(int chatID) {
    GroupChatInfo chat1(0, "Самая первая обычная беседа", QImage(":chatDefaultImage"), 3);
    GroupChatInfo chat2(1, "Админская", QImage(":chatDefaultImage"), 4, ADMIN);
    GroupChatInfo chat3(2, "Зрительская", QImage(":chatDefaultImage"), 5, VIEWER);
    GroupChatInfo chat4(3, "Модераторская", QImage(":chatDefaultImage"), 10, MODERATOR);

    return QList<GroupChatInfo>({chat1, chat2, chat3, chat4});
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
                                  q.value(6).toInt(),
                                  q.value(7).toString())
        );

    return result;
}

QList<MessageInfo> sqlLoadSearchedMessages(int chatID, QString &request) {
    MessageInfo message1(0, "Hello world!", "2021-03-31 22:10",
                         USER_MESSAGE, 0, 10, -1, "Korostast");
    MessageInfo message2(1, "Hello world!", "2021-03-31 22:10",
                         USER_MESSAGE, 0, 10, -1, "Korostast");

    return QList<MessageInfo>({message1, message2});
}

QList<UserChatMember> sqlLoadChatMembers(int userID) {
    UserChatMember user1(0, "Lalala", QImage(":chatDefaultImage"), PARTICIPANT);
    UserChatMember user2(1, "Another one", QImage(":chatDefaultImage"), VIEWER);
    UserChatMember user3(2, "Second", QImage(":chatDefaultImage"), MODERATOR);

    return QList<UserChatMember>({user1, user2, user3});
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
                               QImage::fromData(q.value(2).toByteArray()))
        );

    return result;
}

QList<UserInfo> sqlLoadOutgoingRequests(int userID) {
    qDebug() << "Loading friends for userID =" << userID;

    QString qStr = QString("call getOutgoingRequests(%1)")
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

QList<std::pair<UserInfo, QString> > sqlPeopleInSearch(const QString &substring) {
    std::pair<UserInfo, QString> user1({UserInfo(0, "Lalala", QImage(":chatDefaultImage"))}, "Уже в друзьях");
    std::pair<UserInfo, QString> user2({UserInfo(1, "Another one", QImage(":chatDefaultImage"))}, nullptr);
    std::pair<UserInfo, QString> user3({UserInfo(2, "Second", QImage(":chatDefaultImage"))}, nullptr);
    std::pair<UserInfo, QString> user4({UserInfo(3, "This_is_request", QImage(":chatDefaultImage"))},
                                       "Вы уже послали запрос");
    std::pair<UserInfo, QString> user5({UserInfo(4, "Kriper2003", QImage(":chatDefaultImage"))}, nullptr);

    return QList<std::pair<UserInfo, QString> >({user1, user2, user3, user4, user5});
}

int sqlSendMessage(const MessageInfo &message) {
    qDebug() << "Sending message from" << message.userID << "to chat" << message.chatID;

    QString qStr = QString("call sendMessage(%1,%2,'%3',%4,%5)")
            .arg(message.chatID)
            .arg(message.userID)
            .arg(message.content)
            .arg(message.type)
            .arg(message.replyID);

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

void sqlChangeRole(int userID, int chatID, int newRole) {
    qDebug() << "Setting role of userID =" << userID << "in chatID =" << chatID << "to" << newRole;

    QString qStr = QString("call updateRole(%1,%2,%3)")
            .arg(chatID)
            .arg(userID)
            .arg(newRole);

    QSqlQuery q;
    if (!q.exec(qStr))
        qWarning() << q.lastError().databaseText();
}

void sqlUpdateChat(int chatID, const QString &newName, const QImage &newAvatar) {

}

void sqlAcceptFriendRequest(int currentUserID, int newFriendID) {
    qDebug() << "userID =" << currentUserID << "accepts friend request from" << newFriendID;

    QString qStr = QString("call acceptRequest(%1,%1)")
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

int sqlCreateChat(int adminID, const QString &chatName, const QImage &avatar, const std::vector<int> &participants) {
    return 99;
}

void sqlAddMembers(int chatID, std::vector<int> &newParticipants) {

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