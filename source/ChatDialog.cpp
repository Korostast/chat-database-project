#include <MainWindow.h>
#include "ChatDialog.h"
#include "ui_chatdialog.h"

ChatDialog::ChatDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ChatDialog) {
    ui->setupUi(this);
    connect(ui->chat_dialog_name, SIGNAL(returnPressed()), this, SLOT(changeChatName()));
    connect(ui->chat_dialog_avatar, SIGNAL(released()), this, SLOT(openFileChooser()));
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
        auto *mainWindow = qobject_cast<MainWindow *>(this->parentWidget());
        auto windowCentreX = mainWindow->x() + mainWindow->width() / 2 - CHOOSE_FILE_PAGE_WIDTH / 2;
        auto windowCentreY = mainWindow->y() + mainWindow->height() / 2 - CHOOSE_FILE_PAGE_HEIGHT / 2;
        mainWindow->avatarEditor->setGeometry(windowCentreX, windowCentreY,
                                              CHOOSE_FILE_PAGE_WIDTH, CHOOSE_FILE_PAGE_HEIGHT);
        mainWindow->avatarEditor->setFixedSize(CHOOSE_FILE_PAGE_WIDTH, CHOOSE_FILE_PAGE_HEIGHT);
        mainWindow->avatarEditor->setChooseFilePage();
        mainWindow->avatarEditor->show();
    }
}


