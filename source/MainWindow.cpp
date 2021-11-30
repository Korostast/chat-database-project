#include "MainWindow.h"
#include "./ui_mainwindow.h"
#include "ChatForm.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    currentState = AUTHORIZATION;
    currentUser = new UserInfo();

    ui->setupUi(this);
    ui->app_stacked_widget->setCurrentIndex(AUTHENTICATION_PAGE);
    ui->authentification_stacked_widget->setCurrentIndex(AUTHORIZATION_PAGE);

    // Test
    for (int i = 0; i < 5; ++i) {
        addChat(i, "Hello world!", nullptr, true);
    }

    addChat(5, "Last Item", nullptr, true);
    addChat(6, "LastLast Item", nullptr, true);

    updateChat(2, "Hello!!!!!!!", nullptr);

    QSizePolicy sp_retain = ui->listWidget_4->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    ui->listWidget_4->setSizePolicy(sp_retain);

    QSizePolicy retain = ui->auth_register_error_label->sizePolicy();
    retain.setRetainSizeWhenHidden(true);
    ui->auth_register_error_label->setSizePolicy(retain);

    ui->listWidget_4->hide();
    ui->auth_register_error_label->hide();

    connect(ui->sign_in_button, SIGNAL(released()), this, SLOT(sign_in_button_released()));
    connect(ui->register_button, SIGNAL(released()), this, SLOT(register_button_released()));
    connect(ui->switch_auth_button, SIGNAL(released()), this, SLOT(switch_auth_button_released()));
    connect(ui->switch_register_button, SIGNAL(released()), this, SLOT(switch_register_button_released()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addChat(int id, const QString &name, const QString &avatar, bool isGroup) {
    auto *item = new QListWidgetItem;
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    auto *widget = new ChatForm(this);
    widget->setChatName(name);

    if (isGroup & avatar == nullptr)
        widget->setChatAvatar(":chatDefaultImage", CHAT_IMAGE_SIZE);

    item->setSizeHint(widget->sizeHint());

    ui->chat_list->addItem(item);
    ui->chat_list->setItemWidget(item, widget);

    chatMap[id] = item;
}

// Change chat's attributes and move chat at the top of the list
void MainWindow::updateChat(int id, const QString &name, const QString &avatar) {
    QListWidgetItem *itemN = chatMap[id]->clone();
    auto *widget = qobject_cast<ChatForm *>(ui->chat_list->itemWidget(chatMap[id]));

    ui->chat_list->insertItem(0, itemN);
    ui->chat_list->setItemWidget(itemN, widget);

    ui->chat_list->takeItem(id + 1);

    if (name != nullptr)
        widget->setChatName(name);
    if (avatar != nullptr)
        widget->setChatAvatar(avatar, CHAT_IMAGE_SIZE);
}

QString MainWindow::checkPasswordAndEmail(const QString &password, const QString &email) {
    qDebug() << email.indexOf('@');
    if (password.isEmpty()) {
        return "Password field is empty";
    }
    if (email.isEmpty()) {
        return "Email field is empty";
    }
    if (email.indexOf('@') == -1) {
        qDebug() << email.indexOf('@');
        return "Email is not correct";
    }
    return nullptr;
}

void MainWindow::sign_in_button_released() {
    qDebug() << "AUTH BUTTON CLICKED";

    QString password = ui->auth_password_edit->text();
    QString email = ui->auth_email_edit->text();
    QString error = checkPasswordAndEmail(password, email);
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

    QString username = ui->register_password_edit->text();
    QString password = ui->register_password_edit->text();
    QString email = ui->register_email_edit->text();
    QString error = checkPasswordAndEmail(password, email);
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



