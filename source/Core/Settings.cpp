#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "../SqlInterface.h"

void MainWindow::settings_button_released() const {
    ui->main_stacked_widget->setCurrentIndex(SETTINGS_PAGE);
    currentState = SETTINGS;
}

void MainWindow::settings_save_button_released() {
    const QString firstname(ui->settings_firstname_edit->text());
    const QString lastname(ui->settings_lastname_edit->text());
    const QString phoneNumber(ui->settings_phonenumber_edit->text());
    const QString status(ui->settings_status_edit->text());

    if (currentUser->getFirstName() != firstname || currentUser->getLastName() != lastname ||
        currentUser->getPhoneNumber() != phoneNumber || currentUser->getStatus() != status) {
        sqlUpdateProfile(currentUser->getId(), firstname, lastname, phoneNumber, status, currentUser->getAvatar());
        currentUser->setFirstName(firstname);
        currentUser->setLastName(lastname);
        currentUser->setPhoneNumber(phoneNumber);
        currentUser->setStatus(status);
    }
}
