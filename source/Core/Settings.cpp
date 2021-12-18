#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "SqlInterface.h"

void MainWindow::settings_button_released() const {
    ui->settings_firstname_edit->setText(currentUser->getFirstName());
    ui->settings_lastname_edit->setText(currentUser->getLastName());
    ui->settings_phonenumber_edit->setText(currentUser->getPhoneNumber());
    ui->settings_status_edit->setText(currentUser->getStatus());
    ui->settings_username_edit->setText(currentUser->getUsername());
    ui->settings_password_error_label->hide();
    ui->settings_username_error_label->hide();
    ui->settings_old_password_edit->clear();
    ui->settings_new_password_edit->clear();
    ui->settings_repeat_password_edit->clear();
    ui->settings_username_edit->clear();
    ui->main_stacked_widget->setCurrentIndex(SETTINGS_PAGE);
    currentState = SETTINGS;
}

void checkPhoneNumber(const QString& phoneNumber) {
    // TODO
}

void checkName(const QString& name) {
    // TODO
}

// TODO refactor
void MainWindow::settings_save_button_released() {
    const QString firstname(ui->settings_firstname_edit->text());
    const QString lastname(ui->settings_lastname_edit->text());
    const QString phoneNumber(ui->settings_phonenumber_edit->text());
    const QString status(ui->settings_status_edit->text());

    if (currentUser->getFirstName() != firstname || currentUser->getLastName() != lastname ||
        currentUser->getPhoneNumber() != phoneNumber || currentUser->getStatus() != status) {
        // Checks TODO
        checkPhoneNumber(phoneNumber);
        checkName(firstname);
        checkName(lastname);

        sqlUpdateProfile(currentUser->getID(), firstname, lastname, phoneNumber, status, currentUser->getAvatar());
        currentUser->setFirstName(firstname);
        currentUser->setLastName(lastname);
        currentUser->setPhoneNumber(phoneNumber);
        currentUser->setStatus(status);
    }
}

// TODO refactor
void MainWindow::change_password() {
    QString oldPassword(ui->settings_old_password_edit->text());
    QString newPassword(ui->settings_new_password_edit->text());
    QString repeatPassword(ui->settings_repeat_password_edit->text());
    if (oldPassword != currentPassword) {
        ui->settings_password_error_label->setText("Пароль не изменён, так как прежний пароль введён неправильно");
        ui->settings_password_error_label->show();
        return;
    }
    if (newPassword.isEmpty()) {
        ui->settings_password_error_label->setText("Пароль не изменён, "
                                                   "так как новый пароль пустой"); // TODO at least six characters
        ui->settings_password_error_label->show();
        return;
    }
    for (auto letter: newPassword.toLatin1()) {
        if (!isalnum(letter) && !QString("@-_&").contains(letter)) {
            ui->settings_password_error_label->setText("Пароль не изменён, так как пароль может состоять только из "
                                                       "латинских букв, цифр и знаков @-_&");
            ui->settings_password_error_label->show();
            return;
        }
    }
    if (newPassword == oldPassword) {
        ui->settings_password_error_label->setText("Пароль не может быть изменён на старый");
        ui->settings_password_error_label->show();
        return;
    }
    if (newPassword != repeatPassword) {
        ui->settings_password_error_label->setText("Пароль не изменён, так как новый пароль повторен неправильно");
        ui->settings_password_error_label->show();
        return;
    }


    // TODO database change password
    currentPassword = newPassword;
    sqlUpdateAccount(currentUser->getID(), currentUser->getUsername(), newPassword, currentUser->getEmail());

    // Success
    ui->settings_password_error_label->setText("Пароль успешно сменён");
    ui->settings_password_error_label->show();
}

void MainWindow::change_username() {
    QString newUsername = ui->settings_username_edit->text();
    if (currentUser->getUsername() == newUsername) {
        ui->settings_username_error_label->setText("Имя пользователя не может быть сменено на старое");
        ui->settings_username_error_label->show();
        return;
    }
    if (newUsername.isEmpty()) {
        ui->settings_username_error_label->setText("Имя пользователя не может быть пустым");
        ui->settings_username_error_label->show();
        return;
    }
    for (auto letter: newUsername.toLatin1()) {
        if (!isalnum(letter)) {
            ui->settings_username_error_label->setText("Имя пользователя не изменено, потому что допустимы только "
                                                       "латинские буквы и цифры");
            ui->settings_username_error_label->show();
            return;
        }
    }

    // TODO database change username
    try {
        sqlUpdateAccount(currentUser->getID(), newUsername, currentPassword, currentUser->getEmail());
        currentUser->setUsername(newUsername);
    } catch (QSqlException &error) {
        ui->settings_username_error_label->setText(error.what());
        ui->settings_username_error_label->show();
        return;
    }

    // Success
    ui->settings_username_error_label->setText("Имя пользователя успешно сменено");
    ui->settings_username_error_label->show();
}
