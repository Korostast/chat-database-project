#include "MainWindow.h"
#include "ChatDialog.h"
#include "ui_chatdialog.h"
#include "Defines.h"
#include "ChatMemberWidget.h"

// It will show ui of the chat info window only WORK FOR GROUP CHAT
void ChatDialog::setupCurrentChatUi(ChatWidget *chat) {
    ui->chat_dialog_avatar->setPixmap(AvatarEditor::getCircularPixmap(chat->getAvatar(), CHAT_DIALOG_AVATAR_IMAGE_SIZE));
    ui->chat_dialog_name->setText(chat->getName());
    ui->chat_dialog_count_members->setText(QString::number(chat->getCountMembers()));
    if (chat->getRole() == ADMIN) {
        ui->chat_dialog_name->setReadOnly(false);
        ui->chat_dialog_avatar->setCursor(Qt::PointingHandCursor);
    } else {
        ui->chat_dialog_name->setReadOnly(true);
        ui->chat_dialog_avatar->setCursor(Qt::ArrowCursor);
    }

    // Load members list TODO database
    // TEST
    QList<UserChatMember> members;
    members.push_back(UserChatMember(1, "Korostast", QImage(":chatDefaultImage"), ADMIN));
    members.push_back(UserChatMember(2, "Kriper2002", QImage(":chatDefaultImage"), PARTICIPANT));
    members.push_back(UserChatMember(3, "Nya", QImage(":chatDefaultImage"), VIEWER));

    // Cleaning
    ui->chat_dialog_members_list->clear();

    for (const auto& user : members) {
        auto *item = new QListWidgetItem;
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

        auto *widget = new ChatMemberWidget(this);
        widget->setId(user.id);
        widget->setName(user.username);
        widget->setAvatar(user.avatar);
        widget->setRole(user.role);

        item->setSizeHint(widget->sizeHint());
        ui->chat_dialog_members_list->addItem(item);
        ui->chat_dialog_members_list->setItemWidget(item, widget);
    }

}

void ChatDialog::changeChatName() {
    qDebug() << "editingFinished";
    setFocus();
    MainWindow::currentChat->setName(ui->chat_dialog_name->text());
    auto *mainWindow = qobject_cast<MainWindow *>(this->parentWidget());
    mainWindow->updateChat(MainWindow::currentChat->getId(), ui->chat_dialog_name->text(),
                           MainWindow::currentChat->getAvatar(), MainWindow::currentChat->getRole());

    // Send to db new chat name
}

void ChatDialog::openFileChooser() {
    if (MainWindow::currentChat->getRole() == ADMIN) {
        qDebug() << "Open file chooser";
        auto *mainWindow = qobject_cast<MainWindow *>(parentWidget());
        auto windowCentreX = mainWindow->x() + mainWindow->width() / 2 - CHOOSE_FILE_PAGE_WIDTH / 2;
        auto windowCentreY = mainWindow->y() + mainWindow->height() / 2 - CHOOSE_FILE_PAGE_HEIGHT / 2;
        mainWindow->avatarEditor->setGeometry(windowCentreX, windowCentreY,
                                              CHOOSE_FILE_PAGE_WIDTH, CHOOSE_FILE_PAGE_HEIGHT);
        mainWindow->avatarEditor->setFixedSize(CHOOSE_FILE_PAGE_WIDTH, CHOOSE_FILE_PAGE_HEIGHT);
        mainWindow->avatarEditor->setChooseFilePage();
        hide();
        mainWindow->avatarEditor->show();
    }
}

void ChatDialog::removeMemberFromUi(ChatMemberWidget *member) {
    int size = ui->chat_dialog_members_list->count();
    for (int i = 0; i < size; ++i) {
        auto *currentRow = ui->chat_dialog_members_list->item(i);
        auto *user = qobject_cast<ChatMemberWidget *>(ui->chat_dialog_members_list->itemWidget(currentRow));
        if (user->getId() == member->getId()) {
            auto *item = ui->chat_dialog_members_list->takeItem(i);
            delete item;
            break;
        }
    }
}

void ChatDialog::leaveChat() {
    // TODO database
    auto *mainWindow = qobject_cast<MainWindow *>(parent());
    QString content("%1 покинул беседу");
    content = content.arg(MainWindow::currentUser->getUsername());
    mainWindow->addMessage(0, MainWindow::currentChat->getId(), "", "", QImage(), content, SYSTEM_MESSAGE);
    close();

    // Remove chat from chats list and back to chat list
    mainWindow->removeChat(MainWindow::currentChat->getId());
}
