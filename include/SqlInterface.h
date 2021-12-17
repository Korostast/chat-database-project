#ifndef CHATDATABASEPROJECT_SQLINTERFACE_H
#define CHATDATABASEPROJECT_SQLINTERFACE_H

#include "MainWindow.h"
#include "Defines.h"

class QSqlException : std::exception {
    std::string msg;
public:
    explicit QSqlException(std::string message);

    const char *what() const noexcept override;
};

// Create a database connection session
// Errors are handled by the function
void dbConnect(const QString &dbName = "information_schema");

// Close the default database connection session
void dbClose();

// Authorization and registration
UserInfo sqlRegister(const QString &username, const QString &email, const QString &password);

UserInfo sqlAuthenticate(const QString &password, const QString &emailOrUsername);

// Функции отправляющие GET запросы в БД

// Админ или юзер открыл окно выбора базы данных (в структуре id и имя базы)
QList<QString> sqlLoadDatabaseList();

QList<PersonalChatInfo> sqlLoadPersonalChats(int userID);

QList<GroupChatInfo> sqlLoadGroupChats(int userID);

QList<MessageInfo> sqlLoadMessages(int chatID);

// Юзер ищет сообщения по паттерну request
QList<MessageInfo> sqlLoadSearchedMessages(int chatID, QString &request);

// Юзер открыл информацию о беседе, загружается список участников. Используется структура UserChatMember, потому что
// она содержит роль участника. Можно объединить с UserInfo, но вроде смысла нет
QList<UserChatMember> sqlLoadChatMembers(int chatID);

UserInfo sqlLoadProfile(int userID);

QList<UserInfo> sqlLoadFriends(int userID);

QList<UserInfo> sqlLoadIncomingRequests(int userID);

QList<UserInfo> sqlLoadOutgoingRequests(int userID);

QList<std::pair<UserInfo, QString> > sqlSearchUsers(int userID, const QString &substring);

// TODO удалить комментарии. А лучше на англ перевести
// Функции, отправляющие POST запросы в БД
// Вызывается в функции 'sendMessage', когда текущий юзер отправляет сообщение. Возвращает messageID
// Сообщение вставляется в интерфейс уже ПОСЛЕ возвращения. Надо бы добавить анимацию загрузки хотя бы к курсору, но потом
int sqlSendMessage(const MessageInfo &message);

// Если сообщение было отредактировано
void sqlMessageEdited(int messageID, const QString &newContent);

// Админ или модератор удалил сообщение
void sqlDeleteMessage(int messageID);

// Админ воспользовался поиском сообщений и решил удалить все найденные сообщения
void sqlDeleteMessagesByPattern(int chatID, const QString &pattern);

// Текущий юзер нажал кнопку 'выйти из чата'
void sqlLeaveChat(int userID, int chatID);

// Админ исключает участника
void sqlRemoveChatMember(int userID, int chatID);

// Админ меняет роль юзера в чате
void sqlChangeRole(int userID, int chatID, int newRole);

// Админ обновил имя или аватарку чата
void sqlUpdateChat(int chatID, const QString &newName, const QImage &newAvatar);

// Юзер принял заявку в друзья
void sqlAcceptFriendRequest(int currentUserID, int newFriendID);

// Юзер отклонил заявку в друзья
void sqlDeclineFriendRequest(int currentUserID, int notFriendID);

// Юзер отменил заявку в друзья
void sqlCancelFriendRequest(int currentUserID, int notFriendID);

// Юзер отправил заявку в друзья (в поиске людей кнопка, пока не определяет, есть ли друг или заявка уже)
void sqlSendFriendRequest(int userID, int requestedID);

// Юзер удалил друга
void sqlRemoveFriend(int userID, int friendID);

// Юзер с id = adminID создаёт групповой диалог. Participants - список id участников (т.е. по умолчанию роль PARTICIPANT)
// Функция возвращает id беседы. Это нужно для того, чтобы лишний раз не загружать заново список диалогов после создания
int sqlCreateChat(int adminID, const QString &chatName, const QImage &avatar, const std::vector<int> &participants);

// Админ беседы с id = chatID добавил новых участников в беседу. Id новых участников перечислены в newParticipants
void sqlAddMembers(int chatID, std::vector<int> &newParticipants);

// Юзер изменил один или более атрибутов аккаунта. userID не изменяется
void sqlUpdateProfile(int userID, const QString &firstname, const QString &lastname, const QString &phoneNumber,
                      const QString &status, const QImage &avatar);

// Админ авторизуется в окне создания баз данных. Возвращает true, если получилось авторизоваться, false в ином случае
bool sqlAdminAuth(const QString &password);

// Админ создаёт базу данных
void sqlCreateDatabase(const QString &databaseName);

// Админ удаляет базу данных
void sqlDeleteDatabase(const QString &databaseName);

// Юзер выбрал базу данных
void sqlChooseDatabase(const QString &databaseName);

// Юзер сменил пароль. Проверка на корректность проведена
void sqlChangePassword(int userId, const QString &newPassword);

// Юзер сменил имя пользователя. Проверка на корректность имени проведена
// Функция должна выкидывать исключение, если такое имя уже используется
void sqlChangeUsername(int userId, const QString &newUsername);

#endif //CHATDATABASEPROJECT_SQLINTERFACE_H