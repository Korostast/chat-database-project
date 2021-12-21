#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "SqlInterface.h"

// We opened a profile of the user. Awful code.
void MainWindow::showProfile(const UserInfo *user) const {
    const auto &avatar = user->getAvatar();
    ui->profile_avatar->setPixmap(QPixmap::fromImage(avatar.isNull() ? QImage(":user default avatar") : avatar)
                                          .scaled(PROFILE_IMAGE_SIZE,
                                                  PROFILE_IMAGE_SIZE,
                                                  Qt::IgnoreAspectRatio,
                                                  Qt::SmoothTransformation));
    ui->profile_username->setText(user->getUsername());
    ui->profile_status->setText(user->getStatus());
    QList<std::pair<std::pair<QLabel *, QLabel *>, QString> > profileFields{
            {{ui->profile_first_name,   ui->profile_first_name_const},   user->getFirstName()},
            {{ui->profile_last_name,    ui->profile_last_name_const},    user->getLastName()},
            {{ui->profile_phone_number, ui->profile_phone_number_const}, user->getPhoneNumber()},
    };
    for (auto &field: profileFields) {
        QLabel *valueLabel = field.first.first;
        QLabel *constLabel = field.first.second;
        QString &value = field.second;
        if (!value.isEmpty()) {
            valueLabel->setText(value);
            valueLabel->show();
            constLabel->show();
        } else {
            valueLabel->hide();
            constLabel->hide();
        }
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

    ui->profile_avatar->setPositiveIntData(user->getID());

    currentChat = nullptr;
    ui->main_stacked_widget->setCurrentIndex(PROFILE_PAGE);
}


void MainWindow::profile_button_released() {
    // Clear temp variables
    tempImage = QImage();

    UserInfo user = sqlLoadProfile(currentUser->getID());
    showProfile(&user);
}