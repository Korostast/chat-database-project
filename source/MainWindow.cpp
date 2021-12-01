#include <QPainter>
#include <QPainterPath>
#include <QLayout>
#include <UserMessageForm.h>
#include <QGraphicsEffect>
#include <QDateTime>
#include <QScrollBar>
#include "MainWindow.h"
#include "ChatDialog.h"
#include "./ui_mainwindow.h"

ChatForm *MainWindow::currentChat = nullptr;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    //TODO
    avatarEditor = new AvatarEditor(this);
    avatarEditor->loadImageIntoScene(":chatDefaultImage.png");
    avatarEditor->show();

    currentState = AUTHORIZATION;
    currentUser = new UserInfo();
    (currentChat = new ChatForm(this))->hide(); // TODO parent?
    //currentChat->hide();
    chatDialog = new ChatDialog(this);
    chatDialog->installEventFilter(this);

    // Setting up interface and show default page (authorization)
    ui->setupUi(this);
    ui->app_stacked_widget->setCurrentIndex(AUTHENTICATION_PAGE);
    ui->authentification_stacked_widget->setCurrentIndex(AUTHORIZATION_PAGE);
    ui->main_stacked_widget->setCurrentIndex(CHAT_LIST_PAGE);

    int defaultRowNumber = 1;
    ui->message_text_edit->setFixedHeight(getNewEditTextHeight(ui->message_text_edit->document()->size(),
                                                               ui->message_text_edit, defaultRowNumber));

    QSizePolicy sp_retain = ui->listWidget_4->sizePolicy(); // TODO refactor
    sp_retain.setRetainSizeWhenHidden(true);
    ui->listWidget_4->setSizePolicy(sp_retain);
    ui->listWidget_4->hide(); // TODO delete widget?

    QSizePolicy retain = ui->auth_register_error_label->sizePolicy();
    retain.setRetainSizeWhenHidden(true);
    ui->auth_register_error_label->setSizePolicy(retain);
    ui->auth_register_error_label->hide();


    // Test
    for (int i = 0; i < 5; ++i) {
        addChat(i, QString::fromStdString(std::to_string(i)), nullptr, true, 0, PARTICIPANT);
    }
    addChat(5, "5", nullptr, true, 0, VIEWER);
    addChat(6, "6", nullptr, true, 0, ADMIN);
    updateChat(2, nullptr, nullptr, PARTICIPANT);


    connect(ui->sign_in_button, SIGNAL(released()), this, SLOT(sign_in_button_released()));
    connect(ui->register_button, SIGNAL(released()), this, SLOT(register_button_released()));
    connect(ui->switch_auth_button, SIGNAL(released()), this, SLOT(switch_auth_button_released()));
    connect(ui->switch_register_button, SIGNAL(released()), this, SLOT(switch_register_button_released()));
    connect(ui->chat_name_label, SIGNAL(released()), this, SLOT(chat_name_label_released()));
    connect(ui->chats_button, SIGNAL(released()), this, SLOT(chats_button_released()));
    connect(ui->chat_back_button, SIGNAL(released()), this, SLOT(chats_button_released()));
    connect(ui->message_text_edit, SIGNAL(returnKeyPressedEvent()), this, SLOT(sendMessage()));
    connect(ui->auth_password_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->auth_email_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->register_username_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->register_password_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->register_email_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->message_text_edit->document()->documentLayout(), SIGNAL(documentSizeChanged(QSizeF)),
            this, SLOT(messageTextChanged(QSizeF)));


    // Test
    addMessage(0, "Korostast", "2021-03-31 22:10", nullptr, "Hello world!");
    addMessage(1, "Korostast", "2021-03-31 22:11", nullptr, "Hello world!");
    addMessage(2, "Korostast", "2021-03-31 23:59", nullptr, "Hello world! Hello world! "
                                                            "Hello world! Hello world! Hello world! Hello world! Hello world! "
                                                            "Hello world! Hello world! Hello world! Hello world! Hello world! "
                                                            "Hello world! Hello world! Hello world!");
}

MainWindow::~MainWindow() {
    delete ui;
}

QPixmap MainWindow::getCircularPixmap(const QString &pathToImage, const int size) {
    QPixmap currentPixmap(pathToImage);
    currentPixmap = currentPixmap.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPixmap resultPixmap(QSize(size, size));
    resultPixmap.fill(Qt::transparent);

    QPainter painter(&resultPixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath p = QPainterPath();
    p.addRoundedRect(0, 0, size, size, size / 2.0, size / 2.0);
    painter.setClipPath(p);
    painter.drawPixmap(0, 0, currentPixmap);
    return resultPixmap;
}

void
MainWindow::addChat(int id, const QString &name, const QString &avatar, bool isGroup, int countMembers, ROLE role) {
    auto *item = new QListWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

    auto *widget = new ChatForm(this);
    widget->setId(id);
    widget->setName(name);
    widget->setGroup(isGroup);
    widget->setCountMembers(countMembers);
    widget->setRole(role);

    // TODO avatars
    if (isGroup & avatar == nullptr)
        widget->setAvatar(":chatDefaultImage");

    item->setSizeHint(widget->sizeHint());

    ui->chat_list->insertItem(0, item);
    ui->chat_list->setItemWidget(item, widget);

    chatMap[id] = item;
}

// Change chat's attributes and move chat at the top of the list TODO optimization of first row
void MainWindow::updateChat(int id, const QString &name, const QString &avatar, ROLE role) {
    int rowNumber = ui->chat_list->row(chatMap[id]);

    QListWidgetItem *itemN = chatMap[id]->clone();
    auto *widget = qobject_cast<ChatForm *>(ui->chat_list->itemWidget(chatMap[id]));

    ui->chat_list->insertItem(0, itemN);
    ui->chat_list->setItemWidget(itemN, widget);

    ui->chat_list->takeItem(rowNumber + 1);

    if (name != nullptr)
        widget->setName(name);
    if (avatar != nullptr)
        widget->setAvatar(avatar);
    if (role != UNCHANGED)
        widget->setRole(role);

    if (currentChat->getId() == id && ui->chat_name_label->text() != name)
        ui->chat_name_label->setText(name);
}

void MainWindow::openChat() {
    qDebug() << "OPENED";
    qDebug() << "Chat id: " << currentChat->getId();

    // Ask database about the chat? Load messages
    // Clear all previous messages from list

    ui->chat_avatar->setPixmap(getCircularPixmap(currentChat->getAvatar(), 30)); //TODO define
    ui->chat_name_label->setText(currentChat->getName());
    if (currentChat->isGroup()) {
        ui->chat_online_or_members_label->setText(QString::number(currentChat->getCountMembers()));
        ui->chat_online_or_members_label->show();
    } else {
        ui->chat_name_label->hide();
    }
    if (currentChat->getRole() == VIEWER) {
        ui->message_text_edit->hide();
        ui->send_message_button->hide();
    } else {
        ui->message_text_edit->show();
        ui->send_message_button->show();
    }

    currentState = MESSAGES;
    ui->main_stacked_widget->setCurrentIndex(MESSAGES_PAGE);
}

int MainWindow::getNewEditTextHeight(const QSizeF &docSize, const QPlainTextEdit *textEdit, int &countLines) {
    auto widgetMargins = contentsMargins();
    auto documentMargin = textEdit->document()->documentMargin();
    auto fontMetrics = QFontMetrics(textEdit->document()->defaultFont());
    countLines = int(round(docSize.height()));

    auto newHeight = widgetMargins.top() +
                     documentMargin +
                     (countLines + 1) * fontMetrics.height() +
                     textEdit->document()->documentMargin() +
                     widgetMargins.bottom();

    return int(round(newHeight));
}

void MainWindow::messageTextChanged(QSizeF docSize) {
    auto *textEdit = ui->message_text_edit;
    static int countLines = 0;

    if (textEdit->height() < textEdit->baseSize().height() || int(round(docSize.height())) < countLines) {
        qreal newHeight = getNewEditTextHeight(docSize, textEdit, countLines);
        textEdit->setFixedHeight(int(round(newHeight)));
        qDebug() << "Text edit height has changed";
    }
}

void MainWindow::addMessage(int id, const QString &username, const QString &time, const QString &avatar,
                            const QString &content) {
    auto *item = new QListWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

    auto *widget = new UserMessageForm(this);

    widget->setId(id);
    // TODO image of message widget
    if (avatar == nullptr)
        widget->setAvatar(":chatDefaultImage");
    else
        widget->setAvatar(avatar);
    widget->setName(username);
    widget->setTime(time);
    widget->setContent(content);

    item->setSizeHint(widget->sizeHint());

    ui->messageList->addItem(item);
    ui->messageList->setItemWidget(item, widget);
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::WindowDeactivate) {
        qDebug() << "Focus lost!";
        auto *dialog = qobject_cast<ChatDialog *>(object);
        dialog->hide();
        dialog->setFocus();

        return true;
    }
    if (event->type() == QKeyEvent::KeyPress) {
        qDebug() << "aasdasds";
    }
    return false;
}

void MainWindow::chat_name_label_released() {
    qDebug() << "ChatDialog opened";

    chatDialog->setupCurrentChatUi(currentChat);

    chatDialog->show();

    // TODO it doesn't work property
//    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect;
//    effect->setBlurRadius(5);
//    this->setGraphicsEffect(effect);
}

void MainWindow::chats_button_released() {
    ui->main_stacked_widget->setCurrentIndex(CHAT_LIST_PAGE);
    currentChat = nullptr;
    currentState = CHATS;
}

bool MainWindow::checkMessage(QString &content) { // TODO delete 2+ spaces and new lines
    QString tempString(content);
    content.clear();
    content.append(tempString.trimmed());
    if (content.isEmpty())
        return false;
    return true;
}

void MainWindow::sendMessage() {
    //addMessage(currentChat->getId(), currentUser->getUsername(), )
    QString messageText = ui->message_text_edit->toPlainText();
    if (checkMessage(messageText)) {
        addMessage(currentChat->getId(), currentUser->getUsername(),
                   QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"), currentUser->getAvatar(),
                   messageText);

        ui->message_text_edit->clear();

        auto *scrollBar = ui->messageList->verticalScrollBar();
        qDebug() << "Current scroll bar value:" << scrollBar->value() << " and Max value:" << scrollBar->maximum();
        if (scrollBar->maximum() == scrollBar->value() + 1)
            ui->messageList->scrollToBottom();
    }
}
