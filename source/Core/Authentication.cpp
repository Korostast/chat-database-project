#include "MainWindow.h"
#include "./ui_mainwindow.h"

// If data is correct
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
//    if (!error.isEmpty()) {
//        ui->auth_register_error_label->setText(error);
//        ui->auth_register_error_label->show();
//        return;
//    }

    try {
        currentUser = new UserInfo(sqlAuthenticate(email, password));
    } catch (QString &error) {
        ui->auth_register_error_label->setText(error);
        ui->auth_register_error_label->show();
        return;
    }

    // Success
    // Loading data from database: user, account, chats, friends, requests TODO
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
    currentUser = new UserInfo(id, username, QImage(":chatDefaultImage"), email);

    // Change screen to chat list
    ui->app_stacked_widget->setCurrentIndex(APP_PAGE);
}

void MainWindow::switch_auth_button_released() {
    if (currentState == REGISTRATION) {
        qDebug() << "SWITCH TO AUTH";
        ui->switch_auth_button->setChecked(true);
        ui->authentification_stacked_widget->setCurrentIndex(AUTHENTICATION_PAGE);
        ui->switch_register_button->setChecked(false);
        ui->auth_password_edit->setFocus();

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
        ui->register_username_edit->setFocus();

        ui->auth_register_error_label->clear();
        currentState = REGISTRATION;
    } else {
        ui->switch_register_button->setChecked(true);
    }
}