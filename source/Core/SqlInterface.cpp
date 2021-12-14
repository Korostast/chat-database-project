#include "SqlInterface.h"
#include "MainWindow.h"
#include "Defines.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <sstream>
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

    std::stringstream sout;
    sout << "call getChatList(" << userID << ")";

    QSqlQuery q(QString::fromStdString(sout.str()));
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

    std::stringstream sout;
    sout << "call getMessages(" << chatID << ")";

    QSqlQuery q(QString::fromStdString(sout.str()));
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


    QString test3(
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaa");
    MessageInfo message5(4, test3, "2021-03-31 23:59",
                         USER_MESSAGE, 0, 10, -1, "Korostast");

    return result;
}

UserInfo sqlLoadProfile(int userID) {
    return UserInfo(1, "Someone", QImage(":chatDefaultImage"), "Sus status", nullptr, nullptr, nullptr, nullptr);
}

QList<UserInfo> sqlLoadFriends(int userID) {
    qDebug() << "Loading friends for userID =" << userID;

    std::stringstream sout;
    sout << "call getFriends(" << userID << ")";

    QSqlQuery q(QString::fromStdString(sout.str()));
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
    UserInfo user1(3, "This_is_request", QImage(":chatDefaultImage"));
    UserInfo user2(4, "Kriper2003", QImage(":chatDefaultImage"));

    return QList<UserInfo>({user1, user2});
}

QList<UserInfo> sqlLoadOutgoingRequests(int userID) {
    UserInfo user1(5, "Masha", QImage(":chatDefaultImage"));
    UserInfo user2(6, "Java", QImage(":chatDefaultImage"));

    return QList<UserInfo>({user1, user2});
}

QList<UserInfo> sqlPeopleInSearch(const QString &substring) {
    UserInfo user1(0, "Lalala", QImage(":chatDefaultImage"));
    UserInfo user2(1, "Another one", QImage(":chatDefaultImage"));
    UserInfo user3(2, "Second", QImage(":chatDefaultImage"));
    UserInfo user4(3, "This_is_request", QImage(":chatDefaultImage"));
    UserInfo user5(4, "Kriper2003", QImage(":chatDefaultImage"));

    return QList<UserInfo>({user1, user2, user3, user4, user5});
}

int sqlSendMessage(const MessageInfo &message) {
    return 0;
}

void sqlMessageEdited(int messageID, const QString &newContent) {

}

void sqlDeleteMessage(int messageID) {

}

void sqlLeaveChat(int userID, int chatID) {

}

void sqlRemoveChatMember(int userID, int chatID) {

}

void sqlChangeRole(int userID, int chatID, int newRole) {

}

void sqlUpdateChatAvatar(int chatID, QImage &newAvatar) {

}

void sqlChangeChatName(int chatID, const QString &newName) {

}

void sqlAcceptFriendRequest(int currentUserID, int newFriendID) {

}

void sqlDeclineFriendRequest(int currentUserID, int notFriendID) {

}

void sqlCancelFriendRequest(int currentUserID, int notFriendID) {

}

void sqlSendFriendRequest(int userID, int targetUser) {

}

void sqlRemoveFriend(int userID, int friendID) {

}

int sqlGetPersonID(int chatID, int userID) {
    return 0;
}

int sqlCreateChat(int chatID, QString chatName, QImage thumbnail, std::vector<int> users) {
    return 0;
}

QList<UserChatMember> sqlLoadChatMembers(int chatID) {
    return QList<UserChatMember> {};
}

void sqlAddMembers(int chatID, std::vector<int> users) {

}