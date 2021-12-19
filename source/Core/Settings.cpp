#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "SqlInterface.h"
#include <QScrollBar>

void errorLabelSetText(QLabel *label, const QString &error, bool isError) {
    label->setText(error);

    if (isError)
        label->setStyleSheet("background: #4a181a; "
                             "border-radius: 10px;");
    else
        label->setStyleSheet("background: #015302;"
                             "border-radius: 10px;");

    label->show();
}

void MainWindow::settings_button_released() {
    // Clear temp variables
    tempImage = QImage();

    UserInfo curUser = sqlLoadProfile(currentUser->getID());
    ui->settings_firstname_edit->setText(curUser.getFirstName());
    ui->settings_lastname_edit->setText(curUser.getLastName());
    ui->settings_phonenumber_edit->setText(curUser.getPhoneNumber());
    ui->settings_status_edit->setText(curUser.getStatus());
    ui->settings_username_edit->setText(curUser.getUsername());

    ui->settings_password_error_label->hide();
    ui->settings_username_error_label->hide();
    ui->settings_profile_info_error_label->hide();
    ui->settings_save_label->hide();
    ui->settings_old_password_edit->clear();
    ui->settings_new_password_edit->clear();
    ui->settings_repeat_password_edit->clear();
    ui->main_stacked_widget->setCurrentIndex(SETTINGS_PAGE);
    currentState = SETTINGS;
}

bool MainWindow::checkPhoneNumber(const QString &phoneNumber) const {
    for (const auto &letter: phoneNumber) {
        if (!(letter.isDigit() || letter == '+' || letter == '-') ) {
            errorLabelSetText(ui->settings_profile_info_error_label, "Номер телефона может состоять только из цифр",
                              true);
            auto *verticalScrollBar = ui->settings_scrollarea->verticalScrollBar();
            verticalScrollBar->setValue(verticalScrollBar->minimum());
            ui->settings_phonenumber_edit->setFocus();
            return false;
        }
    }
    return true;
}

bool MainWindow::checkName(const QString &name, bool isFirstname) const {
    for (const auto &letter: name) {
        if (!letter.isLetter()) {
            errorLabelSetText(ui->settings_profile_info_error_label, "Имя может состоять только из букв", true);
            auto *verticalScrollBar = ui->settings_scrollarea->verticalScrollBar();
            verticalScrollBar->setValue(verticalScrollBar->minimum());
            if (isFirstname)
                ui->settings_firstname_edit->setFocus();
            else
                ui->settings_lastname_edit->setFocus();
            return false;
        }
    }
    return true;
}

// TODO refactor
void MainWindow::settings_save_button_released() {
    qDebug() << "Trying to save settings..";
    const QString firstname(ui->settings_firstname_edit->text());
    const QString lastname(ui->settings_lastname_edit->text());
    const QString phoneNumber(ui->settings_phonenumber_edit->text());
    const QString status(ui->settings_status_edit->text());

    // Checks TODO
    if (!checkPhoneNumber(phoneNumber) || !checkName(firstname, true) || !checkName(lastname, false)) {
        qInfo() << "Settings are unsaved, because some fields are incorrect";
        return;
    }

    sqlUpdateOther(currentUser->getID(), status, phoneNumber, firstname, lastname);

    ui->settings_profile_info_error_label->hide();
    errorLabelSetText(ui->settings_save_label, "Настройки сохранены", false);

    qInfo() << "Settings are saved successfully";
}

// TODO refactor
void MainWindow::change_password() const {
    qDebug() << "Trying to save settings..";
    QString oldPassword(ui->settings_old_password_edit->text());
    QString newPassword(ui->settings_new_password_edit->text());
    QString repeatPassword(ui->settings_repeat_password_edit->text());

    for (auto letter: newPassword.toLatin1()) {
        if (!isalnum(letter) && !QString("@-_&").contains(letter)) {
            errorLabelSetText(ui->settings_password_error_label, "Пароль не изменён, так как пароль может состоять "
                                                                 "только из латинских букв, цифр и знаков @-_&", true);
            ui->settings_new_password_edit->setFocus();
            return;
        }
    }
    if (newPassword == oldPassword) {
        ui->settings_password_error_label->setText("Пароль не может быть изменён на старый");
        errorLabelSetText(ui->settings_password_error_label, "Пароль не может быть изменён на старый", true);
        ui->settings_new_password_edit->setFocus();
        return;
    }
    if (newPassword != repeatPassword) {
        ui->settings_password_error_label->setText("Пароль не изменён, так как новый пароль повторен неправильно");
        errorLabelSetText(ui->settings_password_error_label, "Пароль не изменён, так как новый пароль "
                                                             "повторен неправильно", true);
        ui->settings_repeat_password_edit->setFocus();
        return;
    }

    // TODO database change password
    try {
        sqlUpdatePassword(currentUser->getID(), oldPassword, newPassword);
    } catch (QSqlException &error) {
        errorLabelSetText(ui->settings_password_error_label, error.what(), true);
        ui->settings_old_password_edit->setFocus();
        qInfo() << "Password is not unchanged: " << error.what();
        return;
    }

    // Success
    errorLabelSetText(ui->settings_password_error_label, "Пароль успешно сменён", false);
    qInfo() << "Password is changed successfully";
}

void MainWindow::change_username() {
    qDebug() << "Trying to change username..";
    QString newUsername = ui->settings_username_edit->text();
    if (newUsername.isEmpty()) {
        errorLabelSetText(ui->settings_username_error_label, "Имя пользователя не может быть пустым", true);
        ui->settings_username_error_label->setFocus();
        return;
    }
    if (currentUser->getUsername() == newUsername) {
        errorLabelSetText(ui->settings_username_error_label, "Имя пользователя не может быть сменено на старое", true);
        ui->settings_username_error_label->setFocus();
        return;
    }
    for (auto letter: newUsername.toLatin1()) {
        if (!isalnum(letter)) {
            errorLabelSetText(ui->settings_username_error_label, "Имя пользователя не изменено, потому что "
                                                                 "допустимы только латинские буквы и цифры", true);
            ui->settings_username_error_label->setFocus();
            return;
        }
    }

    // TODO database change username
    try {
        sqlUpdateUsername(currentUser->getID(), newUsername);
        currentUser->setUsername(newUsername);
    } catch (QSqlException &error) {
        errorLabelSetText(ui->settings_username_error_label, error.what(), true);
        ui->settings_username_error_label->setFocus();
        qInfo() << "Username is not changed: " << error.what();
        return;
    }

    // Success
    errorLabelSetText(ui->settings_username_error_label, "Имя пользователя успешно сменено", false);
    ui->current_user_name_label->setText(newUsername);
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
