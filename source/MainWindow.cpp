#include <QPainter>
#include <QPainterPath>
#include <QLayout>
#include <UserMessageForm.h>
#include <QGraphicsEffect>
#include "MainWindow.h"
#include "./ui_mainwindow.h"

ChatForm *MainWindow::currentChat = nullptr;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    currentState = AUTHORIZATION;
    currentUser = new UserInfo();
    currentChat = new ChatForm(); // parent?
    chatDialog = new ChatDialog(this);
    chatDialog->installEventFilter(this);

    // Setting up interface and show default page (authorization)
    ui->setupUi(this);
    ui->app_stacked_widget->setCurrentIndex(APP_PAGE);
    ui->authentification_stacked_widget->setCurrentIndex(AUTHORIZATION_PAGE);

    int defaultRowNumber = 1;
    ui->message_text_edit->setFixedHeight(getNewEditTextHeight(ui->message_text_edit->document()->size(),
                                                               ui->message_text_edit, defaultRowNumber));

    // Test
    for (int i = 0; i < 5; ++i) {
        addChat(i, QString::fromStdString(std::to_string(i)), nullptr, true);
    }

    addChat(5, "5", nullptr, true);
    addChat(6, "6", nullptr, true);

    updateChat(2, nullptr, nullptr);

    QSizePolicy sp_retain = ui->listWidget_4->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    ui->listWidget_4->setSizePolicy(sp_retain);

    QSizePolicy retain = ui->auth_register_error_label->sizePolicy();
    retain.setRetainSizeWhenHidden(true);
    ui->auth_register_error_label->setSizePolicy(retain);

    ui->listWidget_4->hide();
    ui->auth_register_error_label->hide();

    ui->stackedWidget_3->setCurrentIndex(1);

    connect(ui->sign_in_button, SIGNAL(released()), this, SLOT(sign_in_button_released()));
    connect(ui->register_button, SIGNAL(released()), this, SLOT(register_button_released()));
    connect(ui->switch_auth_button, SIGNAL(released()), this, SLOT(switch_auth_button_released()));
    connect(ui->switch_register_button, SIGNAL(released()), this, SLOT(switch_register_button_released()));
    connect(ui->chat_name_label, SIGNAL(released()), this, SLOT(chat_name_label_released()));
    //connect(ui->message_text_edit, SIGNAL(textChanged()), this, SLOT(messageTextChanged()));
    connect(ui->message_text_edit->document()->documentLayout(), SIGNAL(documentSizeChanged(QSizeF)),
            this, SLOT(messageTextChanged(QSizeF)));


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

void MainWindow::addChat(int id, const QString &name, const QString &avatar, bool isGroup) {
    auto *item = new QListWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

    auto *widget = new ChatForm(this);
    widget->setId(id);
    widget->setName(name);
    widget->setGroup(isGroup);

    // TODO avatars
    if (isGroup & avatar == nullptr)
        widget->setAvatar(":chatDefaultImage");

    item->setSizeHint(widget->sizeHint());

    ui->chat_list->insertItem(0, item);
    ui->chat_list->setItemWidget(item, widget);

    chatMap[id] = item;
}

// Change chat's attributes and move chat at the top of the list TODO optimization of first row
void MainWindow::updateChat(int id, const QString &name, const QString &avatar) {
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
}

QString MainWindow::checkAuthInput(const QString &username, const QString &password, const QString &email) {
    for (auto letter: username.toLatin1()) {
        if (!isalnum(letter)) {
            return "Username can consist only of latin characters and digits";
        }
    }
    if (password.isEmpty()) {
        return "Password field is empty";
    }
    for (auto letter: password.toLatin1()) {
        if (!isalnum(letter) && !QString("@-_&").contains(letter)) {
            return "Password can consist only of latin characters, digits and special symbols @-_&";
        }
    }
    if (email.isEmpty()) {
        return "Email field is empty";
    }
    if (email.indexOf('@') == -1) {
        return "Email is not correct";
    }
    return nullptr;
}

void MainWindow::sign_in_button_released() {
    qDebug() << "AUTH BUTTON CLICKED";

    QString password = ui->auth_password_edit->text();
    QString email = ui->auth_email_edit->text();
    QString error = checkAuthInput(nullptr, password, email);
    if (!error.isEmpty()) {
        ui->auth_register_error_label->setText(error);
        ui->auth_register_error_label->show();
        return;
    }


    // Call a function that send request to database and return string about error TODO
    //error = "User doesn't exist";
    if (!error.isEmpty()) {
        ui->auth_register_error_label->setText(error);
        ui->auth_register_error_label->show();
        return;
    }

    // Success
    // Loading data from database: user, account, chats, friends, requests TODO
    int id = 0;
    QString username = "Korostast";
    QString avatar = nullptr;
    QString phoneNumber = nullptr;
    QString firstName = nullptr;
    QString lastName = nullptr;
    currentUser = new UserInfo(id, username, email, avatar, phoneNumber, firstName, lastName);
    currentState = CHATS;

    // Change screen to chat list
    ui->app_stacked_widget->setCurrentIndex(APP_PAGE);
}

void MainWindow::register_button_released() {
    qDebug() << "REGISTER BUTTON CLICKED";

    QString username = ui->register_username_edit->text();
    QString password = ui->register_password_edit->text();
    QString email = ui->register_email_edit->text();
    QString error = checkAuthInput(username, password, email);
    if (!error.isEmpty()) {
        ui->auth_register_error_label->setText(error);
        ui->auth_register_error_label->show();
        return;
    }

    // Send request to database
    //error = "User is already exist";
    if (!error.isEmpty()) {
        ui->auth_register_error_label->setText(error);
        ui->auth_register_error_label->show();
        return;
    }

    // Success
    int id = 0; // from db
    currentUser = new UserInfo(id, username, email);

    // Change screen to chat list
    ui->app_stacked_widget->setCurrentIndex(APP_PAGE);
}

void MainWindow::switch_auth_button_released() {
    if (currentState == REGISTRATION) {
        qDebug() << "SWITCH TO AUTH";
        ui->switch_auth_button->setChecked(true);
        ui->authentification_stacked_widget->setCurrentIndex(AUTHENTICATION_PAGE);
        ui->switch_register_button->setChecked(false);

        ui->auth_register_error_label->clear();
        currentState = AUTHORIZATION;
    } else {
        ui->switch_auth_button->setChecked(true);
    }
}

void MainWindow::switch_register_button_released() {
    if (currentState == AUTHORIZATION) {
        qDebug() << "SWITCH TO REGISTER";
        ui->switch_register_button->setChecked(true);
        ui->authentification_stacked_widget->setCurrentIndex(REGISTRATION_PAGE);
        ui->switch_auth_button->setChecked(false);

        ui->auth_register_error_label->clear();
        currentState = REGISTRATION;
    } else {
        ui->switch_register_button->setChecked(true);
    }
}

void MainWindow::openChat() {
    qDebug() << "OPENED";
    qDebug() << "Chat id: " << currentChat->getId();

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

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::WindowDeactivate)
    {
        qDebug() << "Focus lost!";
        auto *dialog = qobject_cast<ChatDialog *>(object);
        dialog->hide();

        return true;
    }
    return false;
}

void MainWindow::chat_name_label_released() {
    qDebug() << "ChatDialog opened";
    chatDialog->show();

    // TODO it doesn't work property
//    QGraphicsBlurEffect *effect = new QGraphicsBlurEffect;
//    effect->setBlurRadius(5);
//    this->setGraphicsEffect(effect);
}
