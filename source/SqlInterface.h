#ifndef CHATDATABASEPROJECT_SQLINTERFACE_H
#define CHATDATABASEPROJECT_SQLINTERFACE_H

#include "MainWindow.h"
#include "Defines.h"

// Функции отправляющие GET запросы в БД
QList<ChatInfo> sqlLoadChats(int userId);

QList<MessageInfo> sqlLoadMessages(int chatId);

// Юзер открыл информацию о беседе, загружается список участников. Используется структура UserChatMember, потому что
// она содержит роль участника. Можно объединить с UserInfo, но вроде смысла нет
QList<UserChatMember> sqlLoadChatMembers(int chatId);

// Юзер открыл личную беседу. Нужно вернуть ID юзера, с которым он общается
int sqlGetPersonId(int chatId, int userId);

UserInfo sqlLoadProfile(int userId);

QList<UserInfo> sqlLoadFriends(int userId);

QList<UserInfo> sqlLoadIncomingRequests(int userId);

QList<UserInfo> sqlLoadOutcomingRequests(int userId);

QList<UserInfo> sqlPeopleInSearch(const QString& substring);

// TODO удалить комментарии. А лучше на англ перевести. А лучше теорвер поучить и дз сделать
// Функции, отправляющие POST запросы в БД
// Вызывается в функции 'sendMessage', когда текущий юзер отправляет сообщение. Возвращает messageId
// Сообщение вставляется в интерфейс уже ПОСЛЕ возвращения. Надо бы добавить анимацию загрузки хотя бы к курсору, но потом
int sqlSendMessage(const MessageInfo& message);

// Если сообщение было отредактировано
void sqlMessageEdited(int messageId, const QString &newContent);

// Админ или модератор удалил сообщение
void sqlDeleteMessage(int messageId);

// Текущий юзер нажал кнопку 'выйти из чата'
void sqlLeaveChat(int userId, int chatId);

// Админ исключает участника
void sqlRemoveChatMember(int userId, int chatId);

// Админ меняет роль юзера в чате
void sqlChangeRole(int userId, int chatId, int newRole);

// Админ поменял аватарку беседы (пока что для профиля нет функции)
void sqlUpdateChatAvatar(int chatId, QImage& newAvatar);

// Админ поменял название беседы
void sqlChangeChatName(int chatId, const QString &newName);

// Юзер принял заявку в друзья
void sqlAcceptFriendRequest(int currentUserId, int newFriendId);

// Юзер отклонил заявку в друзья
void sqlDeclineFriendRequest(int currentUserId, int notFriendId);

// Юзер отменил заявку в друзья
void sqlCancelFriendRequest(int currentUserId, int notFriendId);

// Юзер отправил заявку в друзья (в поиске людей кнопка, пока не определяет, есть ли друг или заявка уже)
void sqlSendFriendRequest(int userId, int targetUser);

// Юзер удалил друга
void sqlRemoveFriend(int userId, int friendId);

// Юзер с id = adminId создаёт групповой диалог. participants - список id участников (т.е. по умолчанию роль PARTICIPANT)
// Функция возвращает id беседы. Это нужно для того, чтобы лишний раз не загружать заново список диалогов после создания
int sqlCreateChat(int adminId, const QString &chatName, const QImage &avatar, const std::vector<int> &participants);

#endif //CHATDATABASEPROJECT_SQLINTERFACE_H
