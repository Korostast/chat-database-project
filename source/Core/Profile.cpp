#include "MainWindow.h"
#include "ui_mainwindow.h"

void MainWindow::loadProfile(const UserInfo *user) {
    // TODO define
    ui->profile_avatar->setPixmap(QPixmap::fromImage(user->getAvatar())
                                          .scaled(150, 150, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    ui->profile_username->setText(user->getUsername());
    ui->profile_status->setText(user->getStatus());
    if (!user->getFirstName().isEmpty()) {
        ui->profile_first_name->setText(user->getFirstName());
        ui->profile_first_name_const->show();
        ui->profile_first_name->show();
    } else {
        ui->profile_first_name_const->hide();
        ui->profile_first_name->hide();
    }

    if (!user->getLastName().isEmpty()) {
        ui->profile_last_name->setText(user->getLastName());
        ui->profile_last_name_const->show();
        ui->profile_last_name->show();
    } else {
        ui->profile_last_name_const->hide();
        ui->profile_last_name->hide();
    }

    if (!user->getPhoneNumber().isEmpty()) {
        ui->profile_phone_number->setText(user->getPhoneNumber());
        ui->profile_phone_number_const->show();
        ui->profile_phone_number->show();
    } else {
        ui->profile_phone_number_const->hide();
        ui->profile_phone_number->hide();
    }

    currentChat = nullptr;
    currentState = PROFILE;
    ui->main_stacked_widget->setCurrentIndex(PROFILE_PAGE);
}


void MainWindow::profile_button_released() {
    loadProfile(currentUser);
}
