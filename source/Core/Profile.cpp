#include "MainWindow.h"
#include "ui_mainwindow.h"

// We opened a profile of the user. Awful code.
void MainWindow::showProfile(const UserInfo *user) const {
    ui->profile_avatar->setPixmap(QPixmap::fromImage(user->getAvatar())
                                          .scaled(PROFILE_IMAGE_SIZE, PROFILE_IMAGE_SIZE, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
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

    if (currentUser->getID() == user->getID()) {
        ui->profile_avatar->setCursor(Qt::PointingHandCursor);
        currentState = MY_PROFILE;
        qDebug() << "Current state changed to MY_PROFILE";
    } else {
        ui->profile_avatar->setCursor(Qt::ArrowCursor);
        currentState = PROFILE;
        qDebug() << "Current state changed to PROFILE";
    }

    currentChat = nullptr;
    ui->main_stacked_widget->setCurrentIndex(PROFILE_PAGE);
}


void MainWindow::profile_button_released() const {
    showProfile(currentUser);
}