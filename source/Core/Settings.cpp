#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "SqlInterface.h"

void MainWindow::settings_button_released() const {
    UserInfo curUser = sqlLoadProfile(currentUser->getID());
    ui->settings_firstname_edit->setText(curUser.getFirstName());
    ui->settings_lastname_edit->setText(curUser.getLastName());
    ui->settings_phonenumber_edit->setText(curUser.getPhoneNumber());
    ui->settings_status_edit->setText(curUser.getStatus());
    ui->settings_username_edit->setText(curUser.getUsername());

    ui->settings_password_error_label->hide();
    ui->settings_username_error_label->hide();
    ui->settings_profile_info_error_label->hide();
    ui->settings_old_password_edit->clear();
    ui->settings_new_password_edit->clear();
    ui->settings_repeat_password_edit->clear();
    ui->main_stacked_widget->setCurrentIndex(SETTINGS_PAGE);
    currentState = SETTINGS;
}

bool MainWindow::checkPhoneNumber(const QString &phoneNumber) const {
    if (std::ranges::all_of(phoneNumber.begin(), phoneNumber.end(), [](QChar letter) { return letter.isDigit(); })) {
        return true;
    } else {
        ui->settings_profile_info_error_label->setText("Номер телефона может состоять только из цифр");
        ui->settings_profile_info_error_label->show();
        return false;
    }
}

bool MainWindow::checkName(const QString &name) const {
    if (std::ranges::all_of(name.begin(), name.end(), [](QChar letter) { return letter.isLetter(); })) {
        return true;
    } else {
        ui->settings_profile_info_error_label->setText("Имя может состоять только из букв");
        ui->settings_profile_info_error_label->show();
        return false;
    }
}

// TODO refactor
void MainWindow::settings_save_button_released() {
    qDebug() << "Trying to save settings..";
    const QString firstname(ui->settings_firstname_edit->text());
    const QString lastname(ui->settings_lastname_edit->text());
    const QString phoneNumber(ui->settings_phonenumber_edit->text());
    const QString status(ui->settings_status_edit->text());

    if (currentUser->getFirstName() != firstname || currentUser->getLastName() != lastname ||
        currentUser->getPhoneNumber() != phoneNumber || currentUser->getStatus() != status) {
        // Checks TODO
        if (!checkPhoneNumber(phoneNumber) || !checkName(firstname) || !checkName(lastname)) {
            qInfo() << "Settings are unsaved, because some fields are incorrect";
            return;
        }

        sqlUpdateOther(currentUser->getID(), status, phoneNumber, firstname, lastname);
        currentUser->setFirstName(firstname);
        currentUser->setLastName(lastname);
        currentUser->setPhoneNumber(phoneNumber);
        currentUser->setStatus(status);

        ui->settings_profile_info_error_label->hide();
    }
    qInfo() << "Settings are saved successfully";
}

// TODO refactor
void MainWindow::change_password() const {
    qDebug() << "Trying to save settings..";
    QString oldPassword(ui->settings_old_password_edit->text());
    QString newPassword(ui->settings_new_password_edit->text());
    QString repeatPassword(ui->settings_repeat_password_edit->text());
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
    try {
        sqlUpdatePassword(currentUser->getID(), newPassword);
    } catch (QSqlException &error) {
        ui->settings_password_error_label->setText(error.what());
        ui->settings_password_error_label->show();
        qInfo() << "Password is not unchanged: " << error.what();
        return;
    }

    // Success
    ui->settings_password_error_label->setText("Пароль успешно сменён");
    ui->settings_password_error_label->show();

    qInfo() << "Password is changed successfully";
}

void MainWindow::change_username() {
    qDebug() << "Trying to change username..";
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
        sqlUpdateUsername(currentUser->getID(), newUsername);
        currentUser->setUsername(newUsername);
    } catch (QSqlException &error) {
        ui->settings_username_error_label->setText(error.what());
        ui->settings_username_error_label->show();
        qInfo() << "Username is not changed: " << error.what();
        return;
    }

    // Success
    ui->settings_username_error_label->setText("Имя пользователя успешно сменено");
    ui->settings_username_error_label->show();
    qInfo() << "Username is changed successfully";
}

void MainWindow::change_password_return_pressed() const {
    QList<QLineEdit *> passwordEdits{ui->settings_old_password_edit, ui->settings_new_password_edit,
                                     ui->settings_repeat_password_edit};

    for (const auto &edit: passwordEdits) {
        if (edit->text().isEmpty()) {
            edit->setFocus();
            return;
        }
    }

    change_password();
}
