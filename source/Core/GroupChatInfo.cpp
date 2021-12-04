#include "MainWindow.h"
#include "ChatDialog.h"
#include "ui_chatdialog.h"

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