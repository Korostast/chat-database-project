create table Chat
(
    id           int auto_increment
        primary key,
    chat_name    varchar(32)   not null,
    is_group     tinyint(1)    not null,
    avatar       longblob          null,
    member_count int default 0 null
);

create table User
(
    id       int auto_increment
        primary key,
    username varchar(16) not null,
    avatar   longblob    null,
    constraint username
        unique (username)
);

create table Account
(
    id           int           not null
        primary key,
    password     varchar(32)   not null,
    email        varchar(320)  not null,
    phone_number varchar(15)   null,
    created_date date          not null,
    first_name   varchar(32)   null,
    last_name    varchar(32)   null,
    friend_count int default 0 null,
    status       varchar(200)  null,
    constraint email
        unique (email),
    constraint Account_ibfk_1
        foreign key (id) references User (id)
            on update cascade on delete cascade
);

create index email_2
    on Account (email);

create index phone_number_2
    on Account (phone_number);

create table Chat_to_User
(
    chat_id   int                                                                        not null,
    user_id   int                                                                        not null,
    user_role enum ('viewer', 'participant', 'moderator', 'admin') default 'participant' null,
    primary key (chat_id, user_id),
    constraint Chat_to_User_ibfk_1
        foreign key (chat_id) references Chat (id)
            on update cascade on delete cascade,
    constraint Chat_to_User_ibfk_2
        foreign key (user_id) references User (id)
            on update cascade on delete cascade
);

create index user_id
    on Chat_to_User (user_id);

create definer = admin@`%` trigger userJoinedChat
    after insert
    on Chat_to_User
    for each row
begin
    update Chat C
    set C.member_count = C.member_count + 1
    where C.id = NEW.chat_id;
end;

create definer = admin@`%` trigger userLeftChat
    before delete
    on Chat_to_User
    for each row
begin
    update Chat C
    set C.member_count = C.member_count - 1
    where C.id = OLD.chat_id;
end;

create table Friend_List
(
    first_id  int not null,
    second_id int not null,
    primary key (first_id, second_id),
    constraint Friend_List_ibfk_1
        foreign key (first_id) references User (id)
            on update cascade on delete cascade,
    constraint Friend_List_ibfk_2
        foreign key (second_id) references User (id)
            on update cascade on delete cascade
);

create index second_id
    on Friend_List (second_id);

create definer = admin@`%` trigger brokenFriendship
    after delete
    on Friend_List
    for each row
begin
    declare commonChatID int;

    update Account A
    set A.friend_count = A.friend_count - 1
    where A.id in (OLD.first_id, OLD.second_id);

    set commonChatID = (select C.id
                        from Chat C
                        where not C.is_group
                          and 2 = (select count(*)
                                   from Chat_to_User CtU
                                   where CtU.chat_id = C.id
                                     and CtU.user_id in (OLD.first_id, OLD.second_id)));

    delete
    from Chat C
    where C.id = commonChatID;
end;

create definer = admin@`%` trigger newFriendship
    after insert
    on Friend_List
    for each row
begin
    declare newChatID int;

    update Account A
    set A.friend_count = A.friend_count + 1
    where A.id in (NEW.first_id, NEW.second_id);

    insert into Chat(chat_name, is_group)
    values ('<private>', false);

    set newChatID = last_insert_id();

    insert into Chat_to_User(chat_id, user_id)
    values (newChatID, NEW.first_id),
           (newChatID, NEW.second_id);
end;

create table Message
(
    id            int auto_increment
        primary key,
    content       varchar(4000)                        not null,
    sent_datetime datetime                             not null,
    content_type  enum ('user_message', 'system_info') not null,
    chat_id       int                                  not null,
    user_id       int                                  null,
    constraint Message_ibfk_1
        foreign key (chat_id) references Chat (id)
            on update cascade on delete cascade,
    constraint Message_ibfk_2
        foreign key (user_id) references User (id)
            on update cascade on delete set null
);

create index chat_id
    on Message (chat_id);

create fulltext index content
    on Message (content);

create index user_id
    on Message (user_id);

create table Request_List
(
    user_id      int not null,
    requested_id int not null,
    primary key (user_id, requested_id),
    constraint Request_List_ibfk_1
        foreign key (user_id) references User (id)
            on update cascade on delete cascade,
    constraint Request_List_ibfk_2
        foreign key (requested_id) references User (id)
            on update cascade on delete cascade
);

create index requested_id
    on Request_List (requested_id);

create index username_2
    on User (username);

create
    definer = admin@`%` procedure acceptFriendRequest(IN userID int, IN requestedID int)
begin
    call deleteFriendRequest(userID, requestedID);
    call addFriend(userID, requestedID);
end;

create
    definer = admin@`%` procedure addFriend(IN userID int, IN friendID int)
begin
    insert into Friend_List
    values (userID, friendID);
end;

create
    definer = admin@`%` procedure authenticateUser(IN userID int, IN password varchar(32))
begin
    select U.id, U.username, U.avatar, A.status, A.email
    from User U
             join Account A on U.id = A.id
    where U.id = userID
      and A.password = password;
end;

create
    definer = admin@`%` procedure checkPassword(IN userID int, IN password varchar(32))
begin
    select password = (select A.password from Account A where id = userID) as result;
end;

create
    definer = admin@`%` procedure createGroupChat(IN chatName varchar(32), IN avatar longblob)
begin
    insert into Chat(chat_name, avatar, is_group)
    values (chatName, avatar, true);
    select last_insert_id();
end;

create
    definer = admin@`%` procedure deleteChat(IN chatID int)
begin
    delete
    from Chat
    where id = chatID;
end;

create
    definer = admin@`%` procedure deleteFriend(IN userID int, IN friendID int)
begin
    delete
    from Friend_List
    where first_id = userID and second_id = friendID
       or first_id = friendID and second_id = userID;
end;

create
    definer = admin@`%` procedure deleteFriendRequest(IN userID int, IN requestedID int)
begin
    delete
    from Request_List
    where user_id = userID
      and requested_id = requestedID;
end;

create
    definer = admin@`%` procedure deleteMessage(IN messageID int)
begin
    delete
    from Message
    where id = messageID;
end;

create
    definer = admin@`%` procedure deleteMessagesByPattern(IN chatID int, IN pattern varchar(4000))
begin
    delete
    from Message
    where chat_id = chatID
      and content like concat('%', pattern, '%');
end;

create
    definer = admin@`%` procedure deleteUser(IN userID int)
begin
    delete
    from User
    where id = userID;
end;

create
    definer = admin@`%` procedure editMessage(IN messageID int, IN newContent varchar(4000))
begin
    update Message
    set content = newContent
    where id = messageID;
end;

create
    definer = admin@`%` procedure existsUser(IN handle varchar(320))
begin
    select U.id
    from User U
             join Account A on U.id = A.id
    where handle in (U.username, A.email);
end;

create
    definer = admin@`%` procedure getChatMembers(IN chatID int)
begin
    select U.id, U.username, U.avatar, CtU.user_role - 1 as user_role
    from Chat_to_User CtU
             join User U on CtU.user_id = U.id
    where CtU.chat_id = chatID;
end;

create
    definer = admin@`%` procedure getFriends(IN userID int)
begin
    select U.id, U.username, U.avatar
    from Friend_List FL
             join User U on U.id = FL.first_id
    where userID = FL.second_id
    union
    select U.id, U.username, U.avatar
    from Friend_List FL
             join User U on U.id = FL.second_id
    where userID = FL.first_id;
end;

create
    definer = admin@`%` procedure getGroupChats(IN userID int)
begin
    select C.id, C.chat_name, C.avatar, C.member_count, CtU.user_role - 1 as user_role
    from Chat C
             join Chat_to_User CtU on C.id = CtU.chat_id
    where CtU.user_id = userID
      and C.is_group;
end;

create
    definer = admin@`%` procedure getIncomingRequests(IN userID int)
begin
    select U.id, U.username, U.avatar
    from Request_List RL
             join User U on U.id = RL.user_id
    where userID = RL.requested_id;
end;

create
    definer = admin@`%` procedure getMessages(IN chatID int)
begin
    select M.id,
           M.content,
           M.sent_datetime,
           M.content_type - 1 as content_type,
           M.chat_id,
           M.user_id,
           U.username,
           U.avatar
    from Message M
             join User U on M.user_id = U.id
    where M.chat_id = chatID
    order by M.sent_datetime;
end;

create
    definer = admin@`%` procedure getOutgoingRequests(IN userID int)
begin
    select U.id, U.username, U.avatar
    from Request_List RL
             join User U on U.id = RL.requested_id
    where userID = RL.user_id;
end;

create
    definer = admin@`%` procedure getPersonalChats(IN userID int)
begin
    select C.id,
           (select U.id
            from Chat_to_User CtU2
                     join User U on U.id = CtU2.user_id
            where CtU2.chat_id = C.id
              and U.id != userID) as friend_id,
           (select U.username
            from Chat_to_User CtU2
                     join User U on U.id = CtU2.user_id
            where CtU2.chat_id = C.id
              and U.id != userID) as friend_username,
           (select U.avatar
            from Chat_to_User CtU2
                     join User U on U.id = CtU2.user_id
            where CtU2.chat_id = C.id
              and U.id != userID) as friend_avatar
    from Chat C
             join Chat_to_User CtU on C.id = CtU.chat_id
    where CtU.user_id = userID
      and not C.is_group;
end;

create
    definer = admin@`%` procedure getProfile(IN userID int)
begin
    select U.id,
           U.username,
           U.avatar,
           A.status,
           A.email,
           A.phone_number,
           A.first_name,
           A.last_name
    from User U
             join Account A on U.id = A.id
    where U.id = userID;
end;

create
    definer = admin@`%` procedure joinChat(IN chatID int, IN userID int)
begin
    insert into Chat_to_User(chat_id, user_id)
    values (chatID, userID);
end;

create
    definer = admin@`%` procedure leaveChat(IN chatID int, IN userID int)
begin
    delete
    from Chat_to_User
    where chat_id = chatID
      and user_id = userID;
end;

create
    definer = admin@`%` procedure registerUser(IN username varchar(16), IN password varchar(32), IN email varchar(320))
begin
    declare newID int;

    insert into User(username) values (username);
    set newID = last_insert_id();
    insert into Account(id, password, email, created_date) values (newID, password, email, now());

    select U.id, U.username, U.avatar, A.status, A.email
    from User U
             join Account A on U.id = A.id
    where U.id = newID;
end;

create
    definer = admin@`%` procedure searchMessagesByPattern(IN chatID int, IN pattern varchar(4000))
begin
    select M.id,
           M.content,
           M.sent_datetime,
           M.content_type - 1 as content_type,
           M.chat_id,
           M.user_id,
           U.username,
           U.avatar
    from Message M
             join User U on M.user_id = U.id
    where M.chat_id = chatID
      and M.content like concat('%', pattern, '%');
end;

create
    definer = admin@`%` procedure searchUsersByPattern(IN userID int, IN pattern varchar(16))
begin
    select U.id,
           U.username,
           U.avatar,
           (case
                when exists(select *
                            from Friend_List FL
                            where U.id = FL.first_id and userID = FL.second_id
                               or U.id = FL.second_id and FL.first_id)
                    then 'friend'
                when exists(select *
                            from Request_List RL
                            where U.id = RL.user_id
                              and userID = RL.requested_id)
                    then 'incoming'
                when exists(select *
                            from Request_List RL
                            where U.id = RL.requested_id
                              and userID = RL.user_id)
                    then 'outgoing'
                else 'stranger'
               end) as relation
    from User U
    where U.username like concat('%', pattern, '%');
end;

create
    definer = admin@`%` procedure sendFriendRequest(IN userID int, IN requestedID int)
begin
    insert into Request_List
    values (userID, requestedID);
end;

create
    definer = admin@`%` procedure sendMessage(IN chatID int, IN userID int, IN content varchar(4000),
                                              IN contentType enum ('user_message', 'system_info'))
begin
    insert into Message(content, sent_datetime, content_type, chat_id, user_id)
    values (content, now(), contentType, chatID, userID);

    select last_insert_id() as id;
end;

create
    definer = admin@`%` procedure updateAvatar(IN userID int, IN newAvatar longblob)
begin
    update User U
    set U.avatar = newAvatar
    where U.id = userID;
end;

create
    definer = admin@`%` procedure updateChatAvatar(IN chatID int, IN newAvatar longblob)
begin
    update Chat C
    set C.avatar = newAvatar
    where C.id = chatID;
end;

create
    definer = admin@`%` procedure updateChatName(IN chatID int, IN newName varchar(32))
begin
    update Chat C
    set C.chat_name = newName
    where C.id = chatID;
end;

create
    definer = admin@`%` procedure updateEmail(IN userID int, IN newEmail varchar(320))
begin
    update Account A
    set A.email = newEmail
    where A.id = userID;
end;

create
    definer = admin@`%` procedure updateOther(IN userID int, IN newStatus varchar(200), IN newPhoneNumber varchar(15),
                                              IN newFirstName varchar(32), IN newLastName varchar(32))
begin
    update Account A
    set A.status       = newStatus,
        A.phone_number = newPhoneNumber,
        A.first_name   = newFirstName,
        A.last_name    = newLastName
    where A.id = userID;
end;

create
    definer = admin@`%` procedure updatePassword(IN userID int, IN newPassword varchar(32))
begin
    update Account A
    set A.password = newPassword
    where A.id = userID;
end;

create
    definer = admin@`%` procedure updateRole(IN chatID int, IN userID int,
                                             IN newRole enum ('viewer', 'participant', 'moderator', 'admin'))
begin
    update Chat_to_User
    set user_role = newRole
    where chat_id = chatID
      and user_id = userID;
end;

create
    definer = admin@`%` procedure updateUsername(IN userID int, IN newUsername varchar(16))
begin
    update User U
    set U.username = newUsername
    where U.id = userID;
end;