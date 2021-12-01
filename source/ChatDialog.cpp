#include <MainWindow.h>
#include "ChatDialog.h"
#include "ui_chatdialog.h"

ChatDialog::ChatDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ChatDialog) {
    ui->setupUi(this);
    connect(ui->chat_dialog_name, SIGNAL(returnPressed()), this, SLOT(changeChatName()));
}

ChatDialog::~ChatDialog() {
    delete ui;
}

void ChatDialog::setupCurrentChatUi(ChatForm *chat) {
    ui->chat_dialog_avatar->setPixmap(MainWindow::getCircularPixmap(chat->getAvatar(), 100)); // TODO define
    ui->chat_dialog_name->setText(chat->getName());
    ui->chat_dialog_count_members->setText(QString::number(chat->getCountMembers()));
    if (chat->getRole() == ADMIN) {
        ui->chat_dialog_name->setReadOnly(false);
    } else {
        ui->chat_dialog_name->setReadOnly(true);
    }
}

void ChatDialog::changeChatName() {
    qDebug() << "editingFinished";
    setFocus();
    MainWindow::currentChat->setName(ui->chat_dialog_name->text());
    auto *mainWindow = qobject_cast<MainWindow *>(this->parentWidget());
    mainWindow->updateChat(MainWindow::currentChat->getId(), MainWindow::currentChat->getName(),
                           MainWindow::currentChat->getAvatar(), UNCHANGED);

    // Send to db new chat name
}

