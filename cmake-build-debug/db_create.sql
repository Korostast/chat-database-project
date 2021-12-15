create table Chat
(
    id           int auto_increment
        primary key,
    chat_name    varchar(32)   not null,
    is_group     tinyint(1)    not null,
    thumbnail    blob          null,
    member_count int default 0 null
);

create table User
(
    id             int auto_increment
        primary key,
    username       varchar(16)   not null,
    profile_status varchar(200)  null,
    friend_count   int default 0 null,
    thumbnail      blob          null,
    constraint username
        unique (username)
);

create table Account
(
    id            int          not null
        primary key,
    password_hash varchar(32)  not null,
    email         varchar(320) not null,
    phone_number  varchar(15)  null,
    created_date  date         not null,
    first_name    varchar(32)  null,
    last_name     varchar(32)  null,
    constraint email
        unique (email),
    constraint phone_number
        unique (phone_number),
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
    user_role enum ('admin', 'moderator', 'participant', 'viewer') default 'participant' not null,
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
    update User U
    set U.friend_count = U.friend_count - 1
    where U.id in (OLD.first_id, OLD.second_id);
end;

create definer = admin@`%` trigger newFriendship
    after insert
    on Friend_List
    for each row
begin
    update User U
    set U.friend_count = U.friend_count + 1
    where U.id in (NEW.first_id, NEW.second_id);
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
    reply_id      int                                  null,
    constraint Message_ibfk_1
        foreign key (chat_id) references Chat (id)
            on update cascade on delete cascade,
    constraint Message_ibfk_2
        foreign key (user_id) references User (id)
            on update cascade on delete set null,
    constraint Message_ibfk_3
        foreign key (reply_id) references Message (id)
            on update cascade on delete set null
);

create index chat_id
    on Message (chat_id);

create fulltext index content
    on Message (content);

create index reply_id
    on Message (reply_id);

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
    definer = admin@`%` procedure acceptRequest(IN user_id_ int, IN requested_id_ int)
begin
    call deleteRequest(user_id_, requested_id_);
    call addFriend(user_id_, requested_id_);
end;

create
    definer = admin@`%` procedure addFriend(IN user_id_ int, IN friend_id_ int)
begin
    insert into Friend_List values (user_id_, friend_id_);
end;

create
    definer = admin@`%` procedure authenticateUser(IN id_ int, IN password_hash_ varchar(32))
begin
    select *
    from User U
             join Account A on U.id = A.id
    where U.id = id_ and A.password_hash = password_hash_;
end;

create
    definer = admin@`%` procedure createChat(IN chat_name_ varchar(32), IN is_group_ tinyint(1))
begin
    insert into Chat(chat_name, is_group) VALUES (chat_name_, is_group_);
end;

create
    definer = admin@`%` procedure deleteAccount(IN user_id_ int)
begin
    delete from User where id = user_id_;
end;

create
    definer = admin@`%` procedure deleteChat(IN id_ int)
begin
    delete from Chat where id = id_;
end;

create
    definer = admin@`%` procedure deleteFriend(IN user_id_ int, IN friend_id_ int)
begin
    delete
    from Friend_List
    where first_id = user_id_ and second_id = friend_id_
       or first_id = friend_id_ and second_id = user_id_;
end;

create
    definer = admin@`%` procedure deleteMessage(IN id_ int)
begin
    delete from Message where id = id_;
end;

create
    definer = admin@`%` procedure deleteMessageByPattern(IN chat_id_ int, IN pattern varchar(4000))
begin
    delete
    from Message
    where chat_id = chat_id_
      and content like concat('%', pattern, '%');
end;

create
    definer = admin@`%` procedure deleteRequest(IN user_id_ int, IN requested_id_ int)
begin
    delete from Request_List where user_id = user_id_ and requested_id = requested_id_;
end;

create
    definer = admin@`%` procedure editMessage(IN id_ int, IN content_ varchar(4000))
begin
    update Message
    set content = content_
    where id = id_;
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
    definer = admin@`%` procedure getChatList(IN user_id int)
begin
    select C.id,
           if(C.is_group, C.chat_name, (select U.username
                                        from Chat_to_User CtU2
                                                 join User U on U.id = CtU2.user_id
                                        where CtU2.chat_id = C.id
                                          and U.id != user_id)),
           C.thumbnail,
           C.is_group,
           C.member_count,
           CtU.user_role
    from Chat C
             join Chat_to_User CtU on C.id = CtU.chat_id
    where CtU.user_id = user_id;
end;

create
    definer = admin@`%` procedure getChatMembers(IN chat_id_ int)
begin
    select U.id, U.username
    from Chat_to_User CtU
             join User U on CtU.user_id = U.id
    where CtU.chat_id = chat_id_;
end;

create
    definer = admin@`%` procedure getFriends(IN user_id int)
begin
    select U.id, U.username, U.thumbnail
    from Friend_List FL
             join User U on U.id = FL.first_id
    where user_id = FL.second_id
    union
    select U.id, U.username, U.thumbnail
    from Friend_List FL
             join User U on U.id = FL.second_id
    where user_id = FL.first_id;
end;

create
    definer = admin@`%` procedure getIncomingRequests(IN user_id int)
begin
    select U.id, U.username
    from Request_List RL
             join User U on U.id = RL.requested_id
    where user_id = RL.user_id;
end;

create
    definer = admin@`%` procedure getMessages(IN chat_id int)
begin
    select M.id, M.content, M.sent_datetime, M.content_type, M.chat_id, M.user_id, M.reply_id, U.username
    from Message M
             join User U on M.user_id = U.id
    where M.chat_id = chat_id;
end;

create
    definer = admin@`%` procedure getOutgoingRequests(IN user_id int)
begin
    select U.id, U.username
    from Request_List RL
             join User U on U.id = RL.user_id
    where user_id = RL.requested_id;
end;

create
    definer = admin@`%` procedure getProfile(IN userID int)
begin
    select U.id, U.username, U.thumbnail, U.profile_status, A.email, A.phone_number, A.first_name, A.last_name
    from User U
             join Account A on U.id = A.id
    where U.id = userID;
end;

create
    definer = admin@`%` procedure joinChat(IN chat_id_ int, IN user_id_ int)
begin
    insert into Chat_to_User(chat_id, user_id) values (chat_id_, user_id_);
end;

create
    definer = admin@`%` procedure leaveChat(IN chat_id_ int, IN user_id_ int)
begin
    delete from Chat_to_User where chat_id = chat_id_ and user_id = user_id_;
end;

create
    definer = admin@`%` procedure makeRequest(IN user_id_ int, IN requested_id_ int)
begin
    insert into Request_List values (user_id_, requested_id_);
end;

create
    definer = admin@`%` procedure registerAccount(IN username_ varchar(16), IN password_hash_ varchar(32),
                                                  IN email_ varchar(320))
begin
    declare new_id int;
    insert into User(username) values (username_);
    set new_id = (select U.id from User U order by U.id desc limit 1);
    insert into Account(id, password_hash, email, created_date) values (new_id, password_hash_, email_, now());
    select new_id;
end;

create
    definer = admin@`%` procedure searchMessage(IN chat_id int, IN message varchar(4000))
begin
    select *
    from Message M
             join User U on M.user_id = U.id
    where M.chat_id = chat_id
      and M.content like concat('%', message, '%');
end;

create
    definer = admin@`%` procedure searchUsers(IN userID int, IN username varchar(16))
begin
    select U.id,
           U.username,
           U.thumbnail,
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
    where U.username like concat('%', username, '%');
end;

create
    definer = admin@`%` procedure sendFriendRequest(IN userID int, IN requestedID int)
begin
    insert into Request_List
    values (userID, requestedID);
end;

create
    definer = admin@`%` procedure sendMessage(IN chat_id_ int, IN user_id_ int, IN content_ varchar(4000),
                                              IN content_type_ enum ('user_message', 'system_info'), IN reply_id_ int)
begin
    insert into Message(content, sent_datetime, content_type, chat_id, user_id, reply_id)
    values (content_, now(), content_type_, chat_id_, user_id_, reply_id_);
    select last_insert_id();
end;

create
    definer = admin@`%` procedure updateAccount(IN id_ int, IN password_hash_ varchar(32), IN email_ varchar(320),
                                                IN phone_number_ varchar(15))
begin
    update Account
    set password_hash = password_hash_,
        email         = email_,
        phone_number  = phone_number_
    where id = id_;
end;

create
    definer = admin@`%` procedure updateChatName(IN id_ int, IN chat_name_ varchar(32))
begin
    update Chat
    set chat_name = chat_name_
    where id = id_;
end;

create
    definer = admin@`%` procedure updateRole(IN chat_id_ int, IN user_id_ int,
                                             IN user_role_ enum ('admin', 'moderator', 'participant', 'viewer'))
begin
    update Chat_to_User
    set user_role = user_role_
    where chat_id = chat_id_
      and user_id = user_id_;
end;

create
    definer = admin@`%` procedure updateUser(IN id_ int, IN username_ varchar(16), IN profile_status_ varchar(200),
                                             IN thumbnail_ blob)
begin
    update User
    set username       = username_,
        profile_status = profile_status_,
        thumbnail      = thumbnail_
    where id = id_;
end;