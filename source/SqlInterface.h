#ifndef CHATDATABASEPROJECT_SQLINTERFACE_H
#define CHATDATABASEPROJECT_SQLINTERFACE_H

#include "MainWindow.h"
#include "Defines.h"

// Функции отправляющие GET запросы в БД
QList<ChatInfo> loadChats(int userId);

QList<MessageInfo> loadMessages(int chatId);

UserInfo loadProfile(const QString& username);

QList<UserInfo> loadFriends(int userId);

QList<UserInfo> loadIncomingRequests(int userId);

QList<UserInfo> loadOutcomingRequests(int userId);

QList<UserInfo> peopleInSearch(const QString& substring);

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

#endif //CHATDATABASEPROJECT_SQLINTERFACE_H
