#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "ui_searchpeoplewidget.h"
#include "ui_friendwidget.h"
#include "ui_incomingrequestwidget.h"
#include "ui_outcomingrequestwidget.h"
#include "FriendWidget.h"
#include "IncomingRequestWidget.h"
#include "OutcomingRequestWidget.h"
#include "SearchPeopleWidget.h"
#include "SqlInterface.h"

// Templated function for friend widget and incoming, outcoming requests
template<typename T>
void MainWindow::removeFromList(int requestID, QListWidget *list) {
    for (int i = 0; i < list->count(); ++i) {
        auto *friendWidget = qobject_cast<T *>(list->itemWidget(list->item(i)));
        if (friendWidget->getFriendID() == requestID) {
            auto *item = list->takeItem(i);
            delete item;
            return;
        }
    }
}

template<typename T>
T *MainWindow::findInList(int requestID, QListWidget *list) {
    for (int i = 0; i < list->count(); ++i) {
        auto *friendWidget = qobject_cast<T *>(list->itemWidget(list->item(i)));
        if (friendWidget->getFriendID() == requestID) {
            return friendWidget;
        }
    }
    return nullptr;
}

// Add person widget to the search list
void MainWindow::addPersonInSearch(int personID, const QString &username, const QImage &avatar, const QString &status) {
    if (personID == currentUser->getID())
        return;

    auto *item = new QListWidgetItem;
    auto *widget = new SearchPeopleWidget(this);

    if (!status.isEmpty()) {
        if (status == "friend") {
            widget->ui->search_people_action->setText("Удалить из друзей");
            auto *friendWidget = MainWindow::findInList<FriendWidget>(personID, ui->actual_friends_list);
            if (friendWidget != nullptr)
                connect(widget->ui->search_people_action, &QPushButton::released, this, [friendWidget]() {
                    QMetaObject::invokeMethod(friendWidget->ui->friend_remove, "released");
                });
            else
                qWarning() << QString("Friend with id = %1 was not found").arg(personID);
        } else if (status == "incoming") {
            widget->ui->search_people_action->setText("Принять заявку");
            auto *incomingRequestWidget = MainWindow::findInList<IncomingRequestWidget>(personID,
                                                                                        ui->actual_friends_list);
            if (incomingRequestWidget != nullptr) {
                connect(widget->ui->search_people_action, &QPushButton::released, this, [incomingRequestWidget]() {
                    QMetaObject::invokeMethod(incomingRequestWidget->ui->incoming_request_accept, "released");
                });
            } else {
                qWarning() << QString("Incoming request with id = %1 was not found").arg(personID);
            }
        } else if (status == "outcoming") {
            widget->ui->search_people_action->setText("Отменить заявку");
            auto *outcomingRequestWidget = MainWindow::findInList<OutcomingRequestWidget>(personID,
                                                                                          ui->actual_friends_list);
            if (outcomingRequestWidget != nullptr) {
                connect(widget->ui->search_people_action, &QPushButton::released, this, [outcomingRequestWidget]() {
                    QMetaObject::invokeMethod(outcomingRequestWidget->ui->outcoming_request_remove, "released");
                });
            } else {
                qWarning() << QString("Friend with id = %1 was not found").arg(personID);
            }
        } else if (status == "stranger") {
            widget->ui->search_people_action->setText("Добавить в друзья");
            connect(widget->ui->search_people_action, &QPushButton::released, this, [widget]() {
                widget->add_friend_button_released();
            });
        }
    } else {
        widget->ui->search_people_action->hide();
        qWarning() << QString("Unknown state: %1").arg(status);
    }
    widget->setFriendID(personID);
    widget->setUsername(username);
    widget->setAvatar(avatar.isNull() ? QImage(":chatDefaultImage") : avatar);

    // TODO Check if this person is already in the list of outcoming_requests_list and friends list and incoming_requests_list ?


    item->setSizeHint(widget->sizeHint());

    ui->search_people_list->insertItem(0, item);
    ui->search_people_list->setItemWidget(item, widget);
}

void SearchPeopleWidget::add_friend_button_released() {
    // TODO database send friend request
    sqlSendFriendRequest(MainWindow::currentUser->getID(), getFriendID());

    // TODO change ui of search people widget
    ui->search_people_action->hide();
}

void MainWindow::switch_friends_page(int page) const {
    ui->friends_stacked_widget->setCurrentIndex(page);

    ui->switch_actual_friends->setChecked(page == ACTUAL_FRIENDS_PAGE);
    ui->switch_incoming_requests->setChecked(page == INCOMING_REQUESTS_PAGE);
    ui->switch_outcoming_requests->setChecked(page == OUTCOMING_REQUESTS_PAGE);
    ui->switch_search_people->setChecked(page == SEARCH_PEOPLE_PAGE);

    currentState = static_cast<STATE>(FRIENDS + page);
}

void FriendWidget::friend_remove_button_released() {
    // TODO database remove friend
    sqlRemoveFriend(MainWindow::currentUser->getID(), getFriendID());

    auto *mainWindow = qobject_cast<MainWindow *>(window());
    MainWindow::removeFromList<FriendWidget>(getFriendID(), mainWindow->ui->actual_friends_list);
    mainWindow->ui->switch_actual_friends->setText(
            QString("Друзья (%1)").arg(mainWindow->ui->incoming_requests_list->count()));
}

void IncomingRequestWidget::accept_request_button_released() {
    // TODO database add friend (accept request)
    sqlAcceptFriendRequest(MainWindow::currentUser->getID(), getFriendID());

    auto *mainWindow = qobject_cast<MainWindow *>(window());
    MainWindow::removeFromList<IncomingRequestWidget>(getFriendID(), mainWindow->ui->incoming_requests_list);
    mainWindow->ui->switch_incoming_requests->setText(
            QString("Входящие (%1)").arg(mainWindow->ui->incoming_requests_list->count()));
}

void IncomingRequestWidget::decline_request_button_released() {
    // TODO database
    sqlDeclineFriendRequest(MainWindow::currentUser->getID(), getFriendID());

    auto *mainWindow = qobject_cast<MainWindow *>(window());
    MainWindow::removeFromList<IncomingRequestWidget>(getFriendID(), mainWindow->ui->incoming_requests_list);
    mainWindow->ui->switch_incoming_requests->setText(
            QString("Входящие (%1)").arg(mainWindow->ui->incoming_requests_list->count()));
}

void OutcomingRequestWidget::remove_request_button_released() {
    // TODO database
    sqlCancelFriendRequest(MainWindow::currentUser->getID(), getFriendID());

    auto *mainWindow = qobject_cast<MainWindow *>(window());
    MainWindow::removeFromList<OutcomingRequestWidget>(getFriendID(), mainWindow->ui->outcoming_requests_list);
    mainWindow->ui->switch_outcoming_requests->setText(
            QString("Исходящие (%1)").arg(mainWindow->ui->outcoming_requests_list->count()));
}

void MainWindow::search_people() {
    QString input(ui->search_people_line->text());
    if (input.isEmpty())
        return;

    // TODO database search people
    QList<std::pair<UserInfo, QString> > users = sqlSearchUsers(currentUser->getID(), input);

    ui->search_people_list->clear();

    for (const auto &user: users)
        addPersonInSearch(user.first.getID(), user.first.getUsername(), user.first.getAvatar(), user.second);
}

void MainWindow::friends_button_released() {
    // TODO database searchMessages requests / friends
    int currentId = currentUser->getID();
    QList<UserInfo> friends = sqlLoadFriends(currentId);
    QList<UserInfo> incomingRequests = sqlLoadIncomingRequests(currentId);
    QList<UserInfo> outcomingRequests = sqlLoadOutgoingRequests(currentId);

    ui->actual_friends_list->clear();
    ui->incoming_requests_list->clear();
    ui->outcoming_requests_list->clear();

    ui->switch_actual_friends->setText(QString("Друзья (%1)").arg(friends.count()));
    ui->switch_incoming_requests->setText(QString("Входящие (%1)").arg(incomingRequests.count()));
    ui->switch_outcoming_requests->setText(QString("Исходящие (%1)").arg(outcomingRequests.count()));

    for (const auto &fr: friends)
        addToList<FriendWidget>(fr.getID(), fr.getUsername(), fr.getAvatar(), ui->actual_friends_list);
    for (const auto &in: incomingRequests)
        addToList<IncomingRequestWidget>(in.getID(), in.getUsername(), in.getAvatar(), ui->incoming_requests_list);
    for (const auto &out: outcomingRequests)
        addToList<OutcomingRequestWidget>(out.getID(), out.getUsername(), out.getAvatar(), ui->outcoming_requests_list);

    ui->main_stacked_widget->setCurrentIndex(FRIENDS_PAGE);
    currentChat = nullptr;
    currentState = FRIENDS;

    switch_friends_page(ACTUAL_FRIENDS_PAGE);

    qDebug() << "Current state changed to FRIENDS";
}